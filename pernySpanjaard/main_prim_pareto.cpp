/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Perny and Spanjaard's (2003) algorithm (prim_like)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)
# It returns just only solution Pareto efficient
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include "Grafo.h"
#include <climits>
#include <sys/times.h>
#include <unistd.h>
using namespace std;

Aresta *omega_maximal(int* vertices, Grafo *my_grafo){
	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	Aresta * retorno;
	float min1 = INT_MAX, min2 = INT_MAX;
	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *a = arestas[i];
		if (vertices[a->getOrigem()] != vertices[a->getDestino()]){
			if (a->getPeso1()<min1 || (a->getPeso1()==min1 && a->getPeso2()<min2)){
				retorno = a;
				min1 = a->getPeso1();
				min2 = a->getPeso2();
			}
		}
	}
	return retorno;
	
}

pair <int *, int*> prim_like(Grafo *g, vector<pair<int, int> > relacao2){
	map<int, vector<pair <int *, int*> > > at;
	/*cada vector<pair <int *, int*> > funciona, na verdade, como uma lista de arvores de tamanho t, onde t é seu indice em map. Portanto, uma lista de arvores da forma T^(t) 
	I^(t), do algorito original, nada mais é senao os indices do t-ésimo vector do map*/
	//int *grausChegada = new int[relacao->getQuantVertices()];
	
	at[0].push_back(make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]));
	for (int i=0; i<g->getQuantVertices(); i++) (at[0][0].first)[i] = 0;
	(at[0][0].first)[0] = 1;
	for (int i=0; i<g->getQuantArestas(); i++) (at[0][0].second)[i] = 0;

	pair <int *, int*> corrente = at[0][0];
	for (int t=1; t<=g->getQuantVertices()-1; t++){
			Aresta * max = omega_maximal(corrente.first, g);
			
				//pair <int *, int*> arvore = make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]);
				// for (int p=0; p<g->getQuantVertices(); p++) (arvore.first)[p] = (corrente.first)[p];
				// for (int p=0; p<g->getQuantArestas(); p++) (arvore.second)[p] = (corrente.second)[p];
				(corrente.first)[max->getOrigem()] = 1;
				(corrente.first)[max->getDestino()] = 1;
				(corrente.second)[max->getId()] = 1;
				//at[t].push_back(corrente);
			
			//corrente = at[t][at[t].size()-1];
	}

	return corrente;
	
}

int main(){
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	
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
	times(&tempsInit);

	pair <int *, int*> arvore = prim_like(&my_grafo, relacao2);
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
	//for (int k = 0; k <arvores.size(); k++){ // cada arvore formada
    	pair <int *, int*>  arestas= arvore; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<endl;
    	float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if ((arestas.second)[a] == 1){
				cont1+= arestasPtr[a]->getPeso1();
				cont2+= arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
    	cout<<endl;
    //}

	return 0;
}