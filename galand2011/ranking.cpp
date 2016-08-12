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
#include "lambda.h"
using namespace std;

// bool a_domina_b( Aresta *a, Aresta *b){
// 	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2())){
// 		return true;
// 	} else return false;
// }

// bool a_domina_fracamente_b( Aresta *a, Aresta *b){
// 	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2()){
// 		return true;
// 	} else return false;
// }

// bool isEgal(int *t1, int *t2, int size){
// 	for (int i=0; i<size; i++){
// 		if (t1[i]!=t2[i]) return false;
// 	}
// 	return true;
// }

// bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){
// 	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
// 	for (int i=0; i<arestas.size(); i++){
// 		if (t1[i]==1){
// 			t1_peso1+=arestas[i]->getPeso1();
// 			t1_peso2+=arestas[i]->getPeso2();
// 		} 
// 		if (t2[i]==1){
// 			t2_peso1+=arestas[i]->getPeso1();
// 			t2_peso2+=arestas[i]->getPeso2();
// 		}
// 	}
// 	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
// 		return true;
// 	} else return false;
// }

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


/*
 * w(x) = x^2
 * w é a funçao nao utilitaria descrita em Galand et al (2010)
 * Claro que se quisermos utilisar outra funçao, barta subistui-la
 */
float w(float x){
	return x*x;
}

float psi(float x1, float x2, float v[4]){
	x1 = w(x1);
	x2 = w(x2);
	float x_1 = x1 <= x2 ? x1 : x2 ; // x_(1)
	float x_2 = x1 > x2 ? x1 : x2 ; // x_(2)
	// assert (x_1 <= x_2)
	float soma = 0;
	int vi = 3;
	// i = 1;
	soma += (x_1 - 0)*v[vi];

	// i = 2
	vi = 2;
	if (x2<x_2) vi = 1;
	soma += (x_2 - x_1)*v[vi];

	return soma;

}

/* 
Retorna o vetor de m 0's ou 1's
*/
int * algorithm2(Grafo *g, float v[4]){
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
		} else {
			//omegai = omegai_1;
		}
		//cout<<lambda.first*xi.first+lambda.second*xi.second<<endl;
		//cout<<w(lambda.first*xi.first+lambda.second*xi.second)<<endl;
	} while (it != result.end() && w(lambda.first*xi.first+lambda.second*xi.second)<best);
	cout<<omegai<<"-ésima arvore encontrada"<<endl;
	if (i>=k_best){
		cout<<"*** ATENCAO : O ALGORITMO2 VARREU MAIS QUE AS "<<k_best<<" PRIMEIRAS MELHORES ARVORES; TALVEZ PRECISE AUMENTAR ESSE VALOR"<<endl;
	}
	cout<<best<<endl;
	return Xbest;
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
	
	// pair<float, float> lambda = algorithm1(v);

	// cout<<lambda.first<<"   "<<lambda.second<<endl;
	// list<int *> result;
	// int cont;
	// AllSpaningTree(&my_grafo, lambda.first, lambda.second, result, cont, 500);
	int * arestas = algorithm2(&my_grafo, v);
	// cout<<"Quantidade de arvores encontradas : "<<result.size()<<endl;
	// for (std::list<int *>::iterator it = result.begin();  it != result.end(); ++it){
	// 	int * arestas = *it;
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