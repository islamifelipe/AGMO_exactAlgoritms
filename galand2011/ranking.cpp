/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Galand et al's (2010) algorithm 
# to resolve the Bi-objective Spanning Tree Problem
# This algorithm is a Choquet-based optimisation
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>      // std::pair
#include <climits>
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
#include <list>
#include <climits>
#include "lambda.h"
#include <tuple>
#include <stack>   
using namespace std;

pair<float, float> arvorePesos(int *t1, map <int, Aresta *> arestas){
	float t1_peso1=0, t1_peso2=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
		} 
	}
	return make_pair(t1_peso1, t1_peso2);
}

///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, float lambda1, float lambda2, int *Pa, Heap &List, list <int *> &MSTs, int &cont, list<Grafo> &vetorParticoes){
	/*Pa = vetor de arestas = correspondente à partição P
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P, P2 = P;
	//cout<<List.getSize()<<endl;
	bool res = false;
	float custo, x, y;
	Aresta *a; 
	int *A2;
	map <int, Aresta *> allArestas = P.get_allArestas();
	//for (int i=0; i<P.getQuantVertices()-1; i++){
	
	for (int i=0; i<allArestas.size(); i++){
		if (Pa[i]==1){
			a = allArestas[i];	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				//A2 = new Aresta*[P.getQuantVertices()-1];
				A2 = new int[allArestas.size()];
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				
				//custo=0;cout<<"Antes1"<<endl;
				res = kruskal(&P1, A2, lambda1,  lambda2,custo, 3);

				cont++; 
				if (res){
					//cout<<custo<<endl;
					int quantMSTs = MSTs.size();
					MSTs.push_back(A2);
					List.insert(quantMSTs, custo); // o valor da variavel "custo" vem do kruskal
					vetorParticoes.push_back(P1);
				} else {
					delete[] A2;
				}
				//cout<<"Depois1  "<<List.getSize()<<"   "<<res<<endl;// estatístico
				
				P1 = P2;
			}
		}
	}
}

int AllSpaningTree(Grafo *g,float lambda1, float lambda2, list<int*> &resul, int &cont, int k_best){ 
	int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
	list<int*> MSTs; // usada para lista de árvores
	float custoMinimo =0, x=0, y=0;
	Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	//A estrutura "List" do algortimo original é um heap chamada List
	bool res = kruskal(g, A, lambda1, lambda2, custoMinimo, 3);
	cont =1;
	if (res){
		int contMSTs = MSTs.size();
		MSTs.push_back(A);
		List.insert(contMSTs, custoMinimo);
		vetorParticoes.push_back(*g);
	}
	do{ //List.getChave()<=custoMinimo
		if (List.getSize()>0){
			int id = List.getId();
			//ElementGrafo *init = vetorParticoes->getInit();
			//ElementArvore *initArvore = MSTs->getInit();
			list<int*>::iterator it;
			list<Grafo>::iterator itg = vetorParticoes.begin();
			int k=0;
			for (it=MSTs.begin(); k<id; ++it){
				k++;
				itg++;
			}
			Grafo Ps = *itg;
			//cout<<"Size : "<<List.getSize()<<endl;
			
			List.extract();
			//cout<<"Size : "<<List.getSize()<<endl;
			
			if (k_best>0){
				Partition(Ps, lambda1,  lambda2, *it, List,MSTs, cont,vetorParticoes);
				resul.push_back(*it);
			}
			k_best--;
			
		}
	}while (List.getSize()>0);
	//cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
	//cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
	//cout<<contMSTs<<endl;
	/*for (int i=0; i<vetorParticoes.size(); i++){ TODO
		
			vetorParticoes->remove(vetorParticoes->getInit());
			ElementArvore *initArvore = MSTs->getInit();
			MSTs->remove(MSTs->getInit());
			delete initArvore;
	}*/
	
	List.desaloca();
	return MSTs.size();
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)



/* raking algoritm
Retorna o vetor de m 0's ou 1's
*/
int * algorithm2(Grafo *g, float v[4], float eplison){
	pair<float, float> lambda = algorithm1(v);

	cout<<lambda.first<<"   "<<lambda.second<<endl;
	
	list<int *> result;
	int cont;
	int k_best = 500;
	AllSpaningTree(g, lambda.first, lambda.second, result, cont, k_best); // esse valor de 500 pode variar conforme o tamanho da arvore
	std::list<int *>::iterator it = result.begin();
	int *X1 = *it, *Xi, *Xbest = X1;
	it++;
	int i = 1;
	int omegai = 1;
	pair<float, float> x1 = arvorePesos(X1, g->get_allArestas()), xi;
	float best = psi(x1.first, x1.second,v);
	//cout<<best<<endl;
	do{
		i = i+1;
		Xi = *it;
		it++;
		xi = arvorePesos(Xi, g->get_allArestas());
		float psiwx = psi(xi.first, xi.second,v);
		if (psiwx<best){
			omegai = i;
			best = psiwx;
			Xbest = Xi;
		} 
		//cout<<lambda.first*xi.first+lambda.second*xi.second<<endl;
		//cout<<w(lambda.first*xi.first+lambda.second*xi.second)<<endl;
	} while (it != result.end() && !maiorIgualQuefloat((1.0+eplison)*w(lambda.first*xi.first+lambda.second*xi.second),best));
	cout<<omegai<<"-ésima arvore encontrada"<<endl;
	if (i>=k_best){
		cout<<"*** ATENCAO : O ALGORITMO2 VARREU MAIS QUE AS "<<k_best<<" PRIMEIRAS MELHORES ARVORES; TALVEZ PRECISE AUMENTAR ESSE VALOR"<<endl;
	}
	//cout<<best<<endl;
	return Xbest;
}

int * LB(Grafo *g, float v[4], float &psiMin, float &lb){
	pair<float, float> lambda = algorithm1(v);
	int *f0 = new int[g->getQuantArestas()];
	int *f1 = new int[g->getQuantArestas()];
	int *f2 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++){
		f0[i] = 0;
		f1[i] = 0;
		f2[i] = 0;
	}
	float custo = 0;
	bool a1 = kruskal(g, f0, lambda.first,  lambda.second,custo, 3);
	bool a2 = kruskal(g, f1, lambda.first,  lambda.second,custo, 1); // lambda indiferente
	bool a3 = kruskal(g, f2, lambda.first,  lambda.second,custo, 2); // lambda indiferente
	lb = INT_MAX;
	if (a1 == true && a2==true && a3 == true){
		pair<float, float> f0n = arvorePesos(f0, g->get_allArestas());
		pair<float, float> f1n = arvorePesos(f1, g->get_allArestas());
		pair<float, float> f2n = arvorePesos(f2, g->get_allArestas());
		float psiwx = psi(f1n.first, f2n.second, v);
		
		float wf0 = w(lambda.first*f0n.first + lambda.second*f0n.second);
		if (maiorQuefloat(psiwx,wf0)) lb = psiwx;
		else lb = wf0;


		float melhor = psi(f1n.first, f1n.second, v);
		int *bestT = f1;
		float psiwv = psi(f2n.first, f2n.second, v);
		if (!maiorIgualQuefloat(psiwv,melhor)) {
			bestT = f2;
			melhor = psiwv;
		}
		psiwv = psi(f0n.first, f0n.second, v);
		if (!maiorIgualQuefloat(psiwv,melhor)) {
			bestT = f0;
			melhor = psiwv;
		}

		psiMin = melhor;
		if (bestT!=f0) delete[] f0;

		if (bestT!=f1) delete[] f1;
		
		if (bestT!=f2) delete[] f2;
		return bestT;

		
	}
		
	return NULL;
}

// float BB_recursive(Grafo g, float UB, int in_size, int *resul, float v[4]){
// 	/* Bounding here */
// 	float auxUB;
// 	float lb;
// 	//cout<<"\n\nUB = "<<UB<<" ";
// 	cout<<in_size<<endl;
// 	int *bestT = LB(&g, v, auxUB,lb);
// 	//cout<<"\nbestT : ";
// 	//for (int i=0; i<g.getQuantArestas(); i++) cout<<bestT[i]<<" ";
	
// 	//cout<<"\nLB = "<<lb<<endl;
// 	//for (int i=0; i<g.getQuantArestas(); i++) cout<<g.getStatus(i)<<" ";
// 	//cout<<endl;
// 	if (!maiorQuefloat(lb, UB)){
// 		//cout<<in_size<<endl;
// 		if (in_size == g.getQuantVertices() - 1){
// 			float peso1 = 0, peso2 = 0;

// 			for (int i=0; i<g.getQuantArestas(); i++){
// 				if (g.getStatus(g.get_allArestas()[i]->getId()) == 1){

// 					peso1 += g.get_allArestas()[i]->getPeso1();
// 					peso2 += g.get_allArestas()[i]->getPeso2();
// 					resul[g.get_allArestas()[i]->getId()] = 1;
// 				}
// 				else resul[g.get_allArestas()[i]->getId()] = 0;
// 			}
// 			UB = psi(peso1, peso2, v);
// 			return UB;
// 		} else if ((!equalfloat(lb, UB))){
// 			pair<float, float> lambda = algorithm1(v);
// 			/* Updating the incumbent */
// 			if (!maiorIgualQuefloat(auxUB, UB)){ UB = auxUB;}
// 			/* Branching */
// 			//pair<float, float> lambda = algorithm1(v);
// 			float min = INT_MAX;
// 			int min_i;
// 			//cout<<endl;
// 			for (int i=0; i<g.getQuantArestas(); i++){
// 				if (bestT[i] == 1 && g.getStatus(g.get_allArestas()[i]->getId()) != 1 && g.getStatus(g.get_allArestas()[i]->getId()) != 2){ // se a aresta nao for nem obrigatoria nem proibida
// 					float ll = (g.get_allArestas()[i]->getPeso1()*lambda.first + g.get_allArestas()[i]->getPeso2()*lambda.second);
// 					//cout<<min_i<<" "<<min<<" "<<ll<<endl;
// 					if (!maiorIgualQuefloat(ll,min)){
// 						min = ll;
// 						min_i = g.get_allArestas()[i]->getId();
// 					}
// 				}
// 			}
			
// 			g.setStatus(min_i, 1); // seta a aresta como obrigatoria
// 			cout<<"AQUI1"<<endl;
// 			float best = BB_recursive(g, UB, in_size+1, resul, v);
			
// 			if (!maiorIgualQuefloat(best,UB)) UB = best;
// 			cout<<"AQUI2"<<endl;
// 			g.setStatus(min_i, 2); // seta a aresta como proibida
// 			best = BB_recursive(g, UB, in_size, resul, v);
// 			//g.setStatus(min_i, 0); 
			
// 			//delete[] f1;

// 			//delete[] f2;
// 			cout<<"AQUI3"<<endl;
// 			if (!maiorIgualQuefloat(best,UB)) return best;
// 			else return UB;
// 		}
// 	}
// 	cout<<"AQUI4"<<endl;
	
// 	return UB;
	
// }


int *BB(Grafo g, float v[4], float eplison){
	int * first_tree = algorithm2(&g, v, eplison);
	pair<float, float> f0n = arvorePesos(first_tree, g.get_allArestas());
	float UB = psi(f0n.first,  f0n.second, v);
	int in_size  = 0;
	
	float auxUB;
	float lb;
	
	stack<pair <Grafo, int> >  pilha; // o grafo, e o in_size
	stack<float> pilhaUB;
	
	pair <Grafo, int> S0 = make_pair(g, 0);
	pilha.push(S0);
	pilhaUB.push(UB);
	while (pilha.size()!=0){
		pair <Grafo, int> s2 = pilha.top();
		float auxUB = pilhaUB.top();
		if (auxUB<UB) UB = auxUB;
		pilha.pop();
		pilhaUB.pop();
		float auxUB4;
		Grafo g1 = s2.first;
		in_size = s2.second;
		int *bestT = LB(&g1, v, auxUB4,lb);
		if (!maiorQuefloat(lb, UB)){
			if (in_size == g1.getQuantVertices() - 1){
				float peso1 = 0, peso2 = 0;
				for (int i=0; i<g1.getQuantArestas(); i++){
					if (g1.getStatus(g1.get_allArestas()[i]->getId()) == 1){

						peso1 += g1.get_allArestas()[i]->getPeso1();
						peso2 += g1.get_allArestas()[i]->getPeso2();
						first_tree[g1.get_allArestas()[i]->getId()] = 1;
					}
					else first_tree[g1.get_allArestas()[i]->getId()] = 0;
				}
				UB = psi(peso1, peso2, v);
			} else if ((!equalfloat(lb, UB))){
				pair<float, float> lambda = algorithm1(v);
				
				if (!maiorIgualQuefloat(auxUB4, UB)){ UB = auxUB4;}
				
				//pair<float, float> lambda = algorithm1(v);
				float min = INT_MAX;
				int min_i;

				for (int i=0; i<g1.getQuantArestas(); i++){
					if (bestT[i] == 1 && g1.getStatus(g1.get_allArestas()[i]->getId()) != 1 && g1.getStatus(g1.get_allArestas()[i]->getId()) != 2){ // se a aresta nao for nem obrigatoria nem proibida
						float ll = (g1.get_allArestas()[i]->getPeso1()*lambda.first + g1.get_allArestas()[i]->getPeso2()*lambda.second);
						if (!maiorIgualQuefloat(ll,min)){
							min = ll;
							min_i = g1.get_allArestas()[i]->getId();
						}
					}
				}
				
				g1.setStatus(min_i, 1); // seta a aresta como obrigatoria
				pair <Grafo, int> avec = make_pair (g1, in_size+1);
				g1.setStatus(min_i, 2); // seta a aresta como proibida
				pair <Grafo, int> sans = make_pair (g1, in_size);
				
				pilha.push(sans);
				pilhaUB.push(UB);
				pilha.push(avec);
				pilhaUB.push(UB);
				
			}
		} 
	}
	return first_tree;

}
int main(){
	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	cin>>m; // quantidade de arestas do grafo	
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	for (id=0; id<m; id++){
		cin>>origem;
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
	}
	int i;
	float vi;
	float v[4] = {0,0,0,0};
	while (cin>>i){
		cin>>vi;
		v[i] = vi;
	}
	my_grafo.gerarArestasPtr();
	
	int * arestas = BB(my_grafo, v, 0.4);//// algorithm2(&my_grafo, v,0.3);
		for (int j=0; j<my_grafo.getQuantArestas(); j++){
			
			if (arestas[j] == 1){
				cout<<my_grafo.get_allArestas()[j]->getOrigem()<<" ";
				cout<<my_grafo.get_allArestas()[j]->getDestino()<<" ";
				cout<<my_grafo.get_allArestas()[j]->getPeso1()<<" ";
				cout<<my_grafo.get_allArestas()[j]->getPeso2()<<endl;
			}
		}
		cout<<endl;
	// }

	return 0;
}