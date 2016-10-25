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
	int i = 1;

	stack<pair<int, int*> > pilha; // i e T
	stack<pair<float, float> > pilhaPesos; // pesos da arvore corrente
	stack<pair<Conjunto, int> > pilhaConjunto; // o conjunto de vertices (para indentificar ciclo) e o indice que varre a lista de arestas incidentes no vértice

	int *T = new int[g->getQuantArestas()];
	for (int ii=0; ii<g->getQuantArestas(); ii++) T[ii] = 0;

	pilha.push(make_pair(i, T));
	pilhaPesos.push(make_pair(0,0));

	float x=0, y=0;

	pair<int, int*> p1;
	pair<float, float> p2;

	int aa = 0; // varrer o conjunto de arestas Ai (Mi)
	Conjunto conjunto(g->getQuantVertices());
	pilhaConjunto.push(make_pair(conjunto, aa));

	while (pilha.size()!=0){
		p1 = pilha.top();
		i = p1.first;
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

		vector<Aresta*> M = g->getVertice(i)->getAdjacentes();

		for(;aa<M.size();aa++){
			Aresta *e = M[aa];
			if (conjunto.compare(e->getOrigem(), e->getDestino())==false){
				if (i==g->getQuantVertices()-1){
					int *T2 = new int[g->getQuantArestas()];
					for (int ii=0; ii<g->getQuantArestas(); ii++) T2[ii] = T[ii];
					int xx = x+ e->getPeso1();
					int yy = y+ e->getPeso2();
					T2[e->getId()]=1;
					result.push_back(make_pair(T2, make_pair(xx, yy)));
					
				}else {
					//salva o contexto
					int *T2 = new int[g->getQuantArestas()];
					for (int ii=0; ii<g->getQuantArestas(); ii++) T2[ii] = T[ii];
					Conjunto novo(g->getQuantVertices());
					novo.copia(conjunto);
					pilha.push(make_pair(i, T2));
					pilhaPesos.push(make_pair(x, y));
					pilhaConjunto.push(make_pair(novo,aa+1));
					//
					T[e->getId()]=1;
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

// MUDAR PARAMETROS?
void AllMST(Grafo *g, list<pair<int*, pair<float, float> >  > result, float xl, float yl, float xll, float yll, int direto){

}

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
	my_grafo.gerarArestasPtr();

	list<pair<int*, pair<float, float> > >result;
	AllSpanningTree(&my_grafo, result);

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