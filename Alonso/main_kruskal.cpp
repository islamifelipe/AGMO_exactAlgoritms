/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Alonso et al's (2005) algorithm (optimalcutset_K)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)

#=======================================================================
*/


#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include <stack>
#include "Grafo.h"
#include "Conjunto.h"
using namespace std;


vector <Aresta *> maximal(int* T, Conjunto conjunto, Grafo *my_grafo,vector<pair<int, int> > relacao2){
	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	int *forComparaison = new int[arestas.size()]; // vetor dos id das arestas
	vector <Aresta *> ret;
	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *e = arestas[i];
		if (T[e->getId()]==0 && conjunto.compare(e->getDestino(), e->getOrigem()))
			forComparaison[e->getId()] = 1;
		else 
			forComparaison[e->getId()] = 0;
	}
	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		int id = arestas[i]->getId();
		if (forComparaison[id] == 1){
			bool dominada = false;
			for (int j=0; j<relacao2.size(); j++){ 
				if (id==relacao2[j].second){ // se existe alguém que domina v
					if (forComparaison[relacao2[j].first]==1) dominada=true;
				}
			}
			if (dominada==false){
				ret.push_back(arestas[i]);
			}

		}
	}
	delete[] forComparaison;
	return ret;
}

vector< int* > optimalcutset_K(Grafo *g,vector<pair<int, int> > relacao2){
	vector< int* > resul;
	stack<pair<int *, Conjunto> > pilhaT;
	int *Tinit = new int [g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) Tinit[i] =0; // tudo vazio
	Conjunto Tcinit(g->getQuantVertices());
	pilhaT.push(make_pair(Tinit, Tcinit));

	while (pilhaT.size()){
		pair<int *, Conjunto> s = pilhaT.top();
		pilhaT.pop();
		int *T = s.first;
		Conjunto c = s.second;

		vector <Aresta *> E0 = maximal(T, c, g, relacao2);
		
		Conjunto copie(g->getQuantVertices());
		copie = c;

		// T U E0
		for (int i=0; i<E0.size(); i++) copie.union1(g->get_allArestas()[i]->getOrigem(), g->get_allArestas()[i]->getDestino());
		int aux[g->getQuantVertices()];
		int k0 = 0;
		for (int i=0; i<g->getQuantVertices(); i++) aux[i] =0; 
		for (int i=0; i<g->getQuantVertices(); i++){
			if (aux[copie.find_set(i)]!=1){
				k0++;
				aux[copie.find_set(i)] = 1; // um conjunto diferente
			}

		} 
		cout<<"k0 = "<<k0<<endl;
	
		

	}
	return resul;
}

int main(){
	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	cin>>m;
	Grafo my_grafo(n);
	Grafo relacao(m); // como a relacao se dá no conjunto de arestas, entao a quantidade de vértices do Grafo relacao será a mesma quantidade de arestas de my_grafo  
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	for (id = 0; id<m; id++){
		cin>>origem;
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
	}
	int nA = id; // quantidade de arestas do grafo	
	//cout<<m<<endl;
	id = 0;
	for (int i=0; i<m; i++){ // PADRAO : vértices numerados de 0 à n-1
		relacao.addVertice(i);
	}
	vector< pair<int, int> > relacao2; // primeira aresta domina a segunda
	while (cin>>origem){
		cin>>destino;
		relacao.addArestaDirecionada(id++, origem, destino); // nao nos preocupamos com os pesos para o grafo relacao
		relacao2.push_back(make_pair(origem, destino));
		// origem R destino
	}


	vector<int* > arvores = optimalcutset_K(&my_grafo, relacao2);
	
	return 0;
}