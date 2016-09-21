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
#include "MargeSort.h"
#include "Heap.h"
#include <list>
#include <climits>
#include "lambda.h"
#include <tuple>
#include <stack> 
#include <sys/times.h>
#include <unistd.h>  
using namespace std;


Aresta **listaAresta, **listaAresta1, **listaAresta2; 
pair<float, float> lambda;
int idMST = 0;
// listaAresta1 é ordenada de acordo com o primeiro objetivo
// listaAresta2 é ordenada de acordo com o segundo objetivo
// listaAresta é ordenada de acordo com o a ponderaçao de lambda1 e lambda2

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

void Partition(Grafo P, float lambda1, float lambda2, int *Pa,  Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes){

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
	
	for (int i=0; i<allArestas.size() && List.getSize()<MAX1; i++){
		if (Pa[i]==1){
			a = allArestas[i];	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				//A2 = new Aresta*[P.getQuantVertices()-1];
				A2 = new int[allArestas.size()];
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				
				//custo=0;cout<<"Antes1"<<endl;
				float x, y;
				res = kruskal(&P1, A2, listaAresta, lambda1,  lambda2,custo,x, y, 3);

				//cont++; 
				if (res){
					//cout<<custo<<endl;
					vetorParticoes.push_back(P1);
					list<Grafo>::iterator itt = vetorParticoes.end();
					itt--;
					MSTs[idMST] = make_pair(make_pair(A2,make_pair( x, y)),itt);//A2;
					List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
					// int quantMSTs = MSTs.size();
					// MSTs.push_back(A2);
					// List.insert(quantMSTs, custo); // o valor da variavel "custo" vem do kruskal
					// vetorParticoes.push_back(P1);
				} else {
					delete[] A2;
				}
				//cout<<"Depois1  "<<List.getSize()<<"   "<<res<<endl;// estatístico
				P1.setStatus(a->getId(), 1); 
				//P1 = P2;
			}
		}
	}
}

int AllSpaningTree(Grafo *g,float lambda1, float lambda2, list< pair<int*, pair<float, float> > > &resul, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs, list <Grafo > &vetorParticoes){ 
	
	int id = List.getId();
	pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > par = MSTs[id];
				
	pair<int*, pair<float, float> > it = par.first;//MSTs[id];
	Grafo Ps = *par.second;
			
	List.extract();
	resul.push_back(it);
	//MSTs.erase(id);
	//vetorParticoes.erase(id);
			
	Partition(Ps,lambda1, lambda2, it.first, List,MSTs,vetorParticoes);
			
			
	return MSTs.size();


	// int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
	// list<int*> MSTs; // usada para lista de árvores
	// float custoMinimo =0, x=0, y=0;
	// Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	// list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	// //A estrutura "List" do algortimo original é um heap chamada List
	// bool res = kruskal(g, A, listaAresta, lambda1, lambda2, custoMinimo, 3);
	// cont =1;
	// if (res){
	// 	int contMSTs = MSTs.size();
	// 	MSTs.push_back(A);
	// 	List.insert(contMSTs, custoMinimo);
	// 	vetorParticoes.push_back(*g);
	// }
	// do{ //List.getChave()<=custoMinimo
	// 	if (List.getSize()>0){
	// 		int id = List.getId();
	// 		//ElementGrafo *init = vetorParticoes->getInit();
	// 		//ElementArvore *initArvore = MSTs->getInit();
	// 		list<int*>::iterator it;
	// 		list<Grafo>::iterator itg = vetorParticoes.begin();
	// 		int k=0;
	// 		for (it=MSTs.begin(); k<id; ++it){
	// 			k++;
	// 			itg++;
	// 		}
	// 		Grafo Ps = *itg;
	// 		//cout<<"Size : "<<List.getSize()<<endl;
			
	// 		List.extract();
	// 		//cout<<"Size : "<<List.getSize()<<endl;
			
	// 		if (k_best>0){
	// 			Partition(Ps, lambda1,  lambda2, *it, List,MSTs, cont,vetorParticoes);
	// 			resul.push_back(*it);
	// 		}
	// 		k_best--;
			
	// 	}
	// }while (List.getSize()>0);
	// //cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
	// //cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
	// //cout<<contMSTs<<endl;
	// /*for (int i=0; i<vetorParticoes.size(); i++){ TODO
		
	// 		vetorParticoes->remove(vetorParticoes->getInit());
	// 		ElementArvore *initArvore = MSTs->getInit();
	// 		MSTs->remove(MSTs->getInit());
	// 		delete initArvore;
	// }*/
	
	// List.desaloca();
	// return MSTs.size();
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)



/* raking algoritm
Retorna o vetor de m 0's ou 1's e os valores objetivo associados
*/
pair<int*, pair<float, float> > algorithm2(Grafo *g, float v[4], float eplison){

	
	// list<int *> result;
	// int cont;
	// int k_best = 500;
	

		Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE UM VALOR
		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
		map<int, pair<pair<int*, pair<float, float> >, list<Grafo>::iterator > > MSTs; // usada para lista de árvores
		idMST = 0;
		//cout<<"REDEFINIU"<<endl;
		int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
		//for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
		//mergesort(xp,yp, xq,yq, arestasPtr, g->getQuantArestas(),3);
		float x, y, custoMinimo;
		bool res = kruskal(g, A, listaAresta, lambda.first, lambda.first, custoMinimo, x, y, 3);
		//float custoMinimo = x*(yp-yq)+y*(xq-xp);
		
		if (res){
			vetorParticoes.push_back(*g);
			list<Grafo>::iterator itt = vetorParticoes.end();
			itt--;
			MSTs[idMST] = make_pair(make_pair(A, make_pair(x, y)),itt);//A2;
			List.insert(idMST++, custoMinimo); // o valor da variavel "custo" vem do kruskal
		}

	//AllSpaningTree(g, lambda.first, lambda.second, result, cont, k_best); // esse valor de 500 pode variar conforme o tamanho da arvore
	//std::list<int *>::iterator it = result.begin();
	//int *X1 = *it, *Xi, *Xbest = X1;
	//it++;
	list< pair<int*, pair<float, float> > > resul1;
	AllSpaningTree(g,lambda.first, lambda.second, resul1, List,MSTs, vetorParticoes);
	pair<int*, pair<float, float> > resulTemp = *(resul1.begin());
	int *X1 = resulTemp.first, *Xi, *Xbest = X1;
	int i = 1;
	int omegai = 1;
	//pair<float, float> x1 = arvorePesos(X1, g->get_allArestas()), xi;
	pair<float, float> x1 = resulTemp.second, xbest = x1;
	pair<float, float> xi;
	float best = psi(x1.first, x1.second,v);
	//cout<<best<<endl;
	do{

		i = i+1;

		//list< pair<int*, pair<float, float> > > resul1;
		AllSpaningTree(g,lambda.first, lambda.second, resul1, List,MSTs, vetorParticoes);
		resulTemp = *(resul1.begin());
		Xi = resulTemp.first;

	
		//Xi = *it;
		//it++;
		//xi = arvorePesos(Xi, g->get_allArestas());
		xi = resulTemp.second;
		float psiwx = psi(xi.first, xi.second,v);
		if (psiwx<best){
			omegai = i;
			best = psiwx;
			Xbest = Xi;
			xbest = xi;

		} 
	} while (i<500 && !maiorIgualQuefloat((1.0+eplison)*w(lambda.first*xi.first+lambda.second*xi.second),best));
	cout<<omegai<<"-ésima arvore encontrada"<<endl;
	return make_pair(Xbest, xbest);
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
	float x2, y2;
	bool a2 = kruskal(g, f1, listaAresta1, lambda.first,  lambda.second,custo,x2, y2, 1); // lambda indiferente
	float x3, y3;
	bool a3 = kruskal(g, f2, listaAresta2, lambda.first,  lambda.second,custo, x3, y3, 2); // lambda indiferente
	float x1, y1;
	bool a1 = kruskal(g, f0, listaAresta, lambda.first,  lambda.second,custo, x1, y1, 3);
	
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

int *BB(Grafo g, float v[4], float eplison){
	pair<int*, pair<float, float> > first_tree_pair = algorithm2(&g, v, eplison);
	int * first_tree = first_tree_pair.first;
	pair<float, float> f0n  = first_tree_pair.second;//= arvorePesos(first_tree, g.get_allArestas());
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
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	

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

	listaAresta = my_grafo.getAllArestasPtr();

	listaAresta1 = new Aresta*[my_grafo.getQuantArestas()];
	listaAresta2 = new Aresta*[my_grafo.getQuantArestas()];

	for (int i=0; i<my_grafo.getQuantArestas(); i++){
		listaAresta1[i] = listaAresta[i];
		listaAresta2[i] = listaAresta[i];
	}

	lambda = algorithm1(v);

	cout<<lambda.first<<"   "<<lambda.second<<endl;

	mergesort( lambda.first,  lambda.second, listaAresta, my_grafo.getQuantArestas(), 3);
	mergesort( lambda.first,  lambda.second, listaAresta1, my_grafo.getQuantArestas(), 1);
	mergesort( lambda.first,  lambda.second, listaAresta2, my_grafo.getQuantArestas(), 2);
		

	times(&tempsInit);
	int * arestas = BB(my_grafo, v, 0.4);//// algorithm2(&my_grafo, v,0.3);
		
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
  


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