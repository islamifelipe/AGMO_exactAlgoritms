#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include <sys/times.h>
#include <unistd.h>
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
using namespace std;

void AllSpanningTree(Grafo *g, list<pair<int*, pair<float, float> > >&result){

	// for (int lll = 0; lll<g->getQuantArestas(); lll++){
	// 	cout<<lll<<" "<<g->getStatus(lll)<<endl;
	// }

	int aux[g->getQuantVertices()];
	for (int ii=0; ii<g->getQuantVertices(); ii++){
		aux[ii]=0;
	}
	int i = 3; // 
	int size = 0;
	//int 
	stack<pair<int, int*> > pilha; // i e T
	stack<pair<float, float> > pilhaPesos; // pesos da arvore corrente
	stack<pair<Conjunto, int> > pilhaConjunto; // o conjunto de vertices (para indentificar ciclo) e o indice que varre a lista de arestas incidentes no vértice
	stack<int> sizeTree; // no algoritmo original, o tamanho da arvore é indicado por i, mas aqui, como estamos trabalhando com arestas obrigatoria (inseriadas previamente), precisamos separar. ISSO NAO MUDA A COMPLEXIDADE DO ALGORITMO

	int *T = new int[g->getQuantArestas()];
	float x=0, y=0;
	Conjunto conjunto(g->getQuantVertices());

	for (int ii=0; ii<g->getQuantArestas(); ii++){
		if (g->getStatus(ii)!=1 && g->getStatus(ii)!=2){
			T[ii] = 0;
		} else if (g->getStatus(ii)==1){
			x+=g->get_allArestas()[ii]->getPeso1();
			y+=g->get_allArestas()[ii]->getPeso2();
			//i++;
			aux[g->get_allArestas()[ii]->getOrigem()] = 1;
			aux[g->get_allArestas()[ii]->getDestino()] = 1;

			size++;
			conjunto.union1(g->get_allArestas()[ii]->getOrigem(), g->get_allArestas()[ii]->getDestino());
			T[ii] = 1;
		}
	}

	pair<int, int*> p1;
	pair<float, float> p2;



	int aa = 0; // varrer o conjunto de arestas Ai (Mi)

	//if (size == 0){
		pilha.push(make_pair(i, T));
		pilhaPesos.push(make_pair(x,y));
		pilhaConjunto.push(make_pair(conjunto, aa));
		sizeTree.push(size);
	// } else if (size<g->getQuantVertices()-1){
	// 	for (int ii=0; ii<g->getQuantVertices(); ii++){
	// 		if (aux[ii] == 1){
	// 			//cout<< ii <<endl;
	// 			int *TT = new int[g->getQuantArestas()];
	// 			for (int jj=0; jj<g->getQuantArestas(); jj++)TT[jj] = T[jj];
	// 			i = ii;
				
	// 			pilha.push(make_pair(i, TT));
	// 			pilhaPesos.push(make_pair(x,y));
	// 			Conjunto novo(g->getQuantVertices());
	// 			novo.copia(conjunto);

	// 			pilhaConjunto.push(make_pair(novo, aa));
	// 			sizeTree.push(size);

	// 		}
	// 	}
	// }else {
	// 	result.push_back(make_pair(T, make_pair(x, y)));
	// }

	while (pilha.size()!=0){
		p1 = pilha.top();
		i = p1.first;
		int iiiiii= i;
		T = p1.second;
		pilha.pop();

		p2 = pilhaPesos.top();
		x = p2.first;
		y = p2.second;
		pilhaPesos.pop();

		pair<Conjunto, int>  p3 = pilhaConjunto.top();
		conjunto = p3.first;
		aa = p3.second;
		pilhaConjunto.pop();

		size = sizeTree.top();
		sizeTree.pop();

		vector<Aresta*> M = g->getVertice(i)->getAdjacentes();

		for(;aa<M.size();aa++){
			Aresta *e = M[aa];
			// if (iiiiii == 0 && e->getOrigem()==0 && e->getDestino()==3){
			// 	cout << "size: "<<size<<endl;
			// 	cout <<"Status: "<<g->getStatus(e->getId())<<endl;
			// 	cout<<"conjunto: "<<conjunto.compare(e->getOrigem(), e->getDestino())<<endl;
			// }
			if (g->getStatus(e->getId())!=1 && g->getStatus(e->getId())!=2){
				if (conjunto.compare(e->getOrigem(), e->getDestino())==false){
					if (size==g->getQuantVertices()-2){ // -2, pois o proximo é inserido logo abaixo
						int *T2 = new int[g->getQuantArestas()];
						for (int ii=0; ii<g->getQuantArestas(); ii++) T2[ii] = T[ii];
						int xx = x+ e->getPeso1();
						int yy = y+ e->getPeso2();
						T2[e->getId()]=1;
						//size
						result.push_back(make_pair(T2, make_pair(xx, yy)));
						
					}else if (i+1<g->getQuantVertices()){
						//salva o contexto
						int *T2 = new int[g->getQuantArestas()];
						for (int ii=0; ii<g->getQuantArestas(); ii++) T2[ii] = T[ii];
						Conjunto novo(g->getQuantVertices());
						novo.copia(conjunto);
						pilha.push(make_pair(i, T2));
						pilhaPesos.push(make_pair(x, y));
						pilhaConjunto.push(make_pair(novo,aa+1));
						sizeTree.push(size);
						//
						T[e->getId()]=1;
						size++;
						i++;
						aa = -1;
						M = g->getVertice(i)->getAdjacentes();
						conjunto.union1(e->getOrigem(), e->getDestino());
						x+=e->getPeso1();
						y+=e->getPeso2();

					}
				}
			}
		}


	}
}

void AllMST(Grafo *g, list<pair<int*, pair<float, float> >  > &result, float xl, float yl, float xll, float yll, int direto){
	Aresta **listaAresta = g->getAllArestasPtr();
	mergesort(xl, yl, xll, yll, listaAresta, g->getQuantArestas(), direto);
	float *c = new float[g->getQuantArestas()];
	for (int ii=0; ii<g->getQuantArestas(); ii++){
		if (direto==1){
			c[ii] = listaAresta[ii]->getPeso1();
		}else if (direto==2){
			c[ii] = listaAresta[ii]->getPeso2();
		} else {
			c[ii]= (-1)*(listaAresta[ii]->getPeso1()*(yll - yl)-listaAresta[ii]->getPeso2()*(xll-xl));
		}
	}

	stack<pair<int, int> > pilha1; //index e NEdges
	stack<pair<int*, Conjunto> > pilha2; // arvore

	int NEdges = 0, index = -1;
	int *T = new int[g->getQuantArestas()];

	for (int ii=0; ii<g->getQuantArestas(); ii++) T[ii] = 0;
	Conjunto conjunto(g->getQuantVertices());


	pilha1.push(make_pair(index, NEdges));
	pilha2.push(make_pair(T, conjunto));

	pair<int, int> p1;

	while (pilha1.size()!=0){

		p1 = pilha1.top();
		pilha1.pop();
		index = p1.first;
		NEdges = p1.second;

		pair<int *, Conjunto> p2 = pilha2.top();
		pilha2.pop();
		T = p2.first;
		conjunto = p2.second;
		int *LocalEdgeSet = new int[g->getQuantArestas()];
		for (int ii=0; ii<g->getQuantArestas(); ii++) LocalEdgeSet[ii] = 0;
		Conjunto T_U_localedges(g->getQuantVertices());
		T_U_localedges.copia(conjunto);
		float StageWeight = c[index+1];
		bool MoreThanOne = false;

		do{
			index++;
			if (index < g->getQuantArestas() && conjunto.compare(listaAresta[index]->getOrigem(), listaAresta[index]->getDestino()) == false){ // T U u_index nao forma ciclo
				if (T_U_localedges.compare(listaAresta[index]->getOrigem(), listaAresta[index]->getDestino())==false){
					NEdges++;
				} else {
					MoreThanOne = true;
				}

				LocalEdgeSet[listaAresta[index]->getId()] = 1;
				T_U_localedges.union1(listaAresta[index]->getOrigem(), listaAresta[index]->getDestino());

			}
		
		}while(index < g->getQuantArestas() && !(c[index+1] > StageWeight));// até (c[index+1] > StageWeight)

		if (MoreThanOne){
			cout<<"MoreThanOne"<<endl;
			for (int ii=0; ii<g->getQuantArestas(); ii++){
				if (T[ii]==1){
					g->setStatus(ii, 1);
				}else if (LocalEdgeSet[ii] == 1) {
					cout << ii<<endl;
					g->setStatus(ii, 0);
				} else  g->setStatus(ii, 0);
				// }else{
				// 	//g->setStatus(ii, 2);
				// }
			}
			list<pair<int*, pair<float, float> > > AST;
			AllSpanningTree(g, AST);
			for (std::list<pair<int*, pair<float, float> > >::iterator itt = AST.begin(); itt != AST.end(); itt++){
					cout<<"for"<<endl;

				for (int ii=0; ii<g->getQuantArestas(); ii++){
					if (T[ii]!=1 && LocalEdgeSet[ii] != 1) (*itt).first[ii] = 0;
				}
				if (NEdges == g->getQuantVertices()-1){
						cout<<"if"<<endl;
					result.push_back(make_pair((*itt).first, make_pair(0,0)));
				} else{
						
					// int *T2 = new int[g->getQuantArestas()];
					// for (int ii=0; ii<g->getQuantArestas(); ii++) T2[ii] = (*itt).first[ii];
					Conjunto novo(g->getQuantVertices());
					cout<<"else 1"<<endl;	
					for (int ii=0; ii<g->getQuantArestas(); ii++){
						if ((*itt).first[ii]==1) novo.union1(g->get_allArestas()[ii]->getOrigem(),g->get_allArestas()[ii]->getDestino());
					}
					cout<<"else 3"<<endl;
					pilha1.push(make_pair(index, NEdges));
					pilha2.push(make_pair((*itt).first, novo));
					cout<<"else 4"<<endl;
				}
			}
		} else {
		
			if (NEdges == g->getQuantVertices()-1){
				//cout<<"if"<<endl;
				for (int ii=0; ii<g->getQuantArestas(); ii++) {
					if (LocalEdgeSet[ii] == 1) T[ii] = LocalEdgeSet[ii];
				}
				result.push_back(make_pair(T, make_pair(0,0)));

			} else{
				//cout<<"else"<<endl;
				Conjunto novo(g->getQuantVertices());
				novo.copia(T_U_localedges);
				int *T2 = new int[g->getQuantArestas()];
				for (int ii=0; ii<g->getQuantArestas(); ii++){
					T2[ii] = T[ii] ;
					if (LocalEdgeSet[ii] == 1) T2[ii] = LocalEdgeSet[ii];
				}
				pilha1.push(make_pair(index, NEdges));
				pilha2.push(make_pair(T2, novo));
			}

		}


	}

}

// void AllMST_aux(int *T, Conjunto conjunto, int i, int NEdges){
// 	int * T_U_localedges = new int[]
// }
int main(){
	int n;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}

	int nA = id; // quantidade de arestas do grafo	
	//my_grafo.setStatus(0,1);
	//my_grafo.setStatus(1,1);
	//my_grafo.setStatus(4,2);
	my_grafo.gerarArestasPtr();

	list<pair<int*, pair<float, float> > >result;
	AllMST(&my_grafo, result, 0,0,0,0,1);
	
	//AllSpanningTree(&my_grafo, result);

	cout<<"Quantidade de arvoes encontradas = "<<result.size()<<endl;
	cout<<endl;
	for (std::list<pair<int*, pair<float, float> > >::iterator it = result.begin(); it != result.end(); it++){
		int *T = (*it).first;
		float x = (*it).second.first;
		float y = (*it).second.second;
		for (int aa=0; aa<my_grafo.getQuantArestas(); aa++){
			if (T[aa]==1){
				Aresta* e = my_grafo.get_allArestas()[aa];
				cout<<e->getOrigem()<<" "<<e->getDestino()<<" "<<e->getPeso1()<<" "<<e->getPeso2()<<" "<<endl;
			}
		}
		cout<<"("<<x<<", "<<y<<")\n"<<endl;
	}
	
}