/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Pugliese et al's (2014) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The user time is returned (in seconds)
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <vector>
#include <utility>      // std::pair
#include <string>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;


bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

/* Recebe os vetores de vértices
Determina se eles sao equivalentes
*/
bool isEquivalente(int *t1, int *t2, int n){ // 
	for (int i=0; i<n; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}
bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){ // fracamente
	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
		} 
		if (t2[i]==1){
			t2_peso1+=arestas[i]->getPeso1();
			t2_peso2+=arestas[i]->getPeso2();
		}
	}
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
		return true;
	} else return false;
}

/*retorna true se Tqh1 é dominada por alguma arvore parcial equivalente pertecente à Lq1 
 Tqh1 é um vetor de 0's e 1's com o comprimento igual ao numero de ARESTAS
 Lq1 DEVE SER UMA REFERÊNCIA
*/
bool isDominada(pair<int*, int*> Tqh1, vector< pair<int*, int*> > &Lq1, Grafo *g){
	// verificacao se Tqh1 é dominada por alguém
	for (int t = 0; t<Lq1.size(); t++){
		int *arvore_parcial = Lq1[t].second; 
		if (isEquivalente(Tqh1.first,Lq1[t].first, g->getQuantVertices()) && t1_domina_t2(arvore_parcial, Tqh1.second, g->get_allArestas())){
			return true; // é dominada
		}
	} 

	// se Tqh1 nao é dominada por ninguém equivalente, entao verificamos se Tqh1 domina algum t em Lq1. Se sim, removemos t de Lq1.
	for (int t = 0; t<Lq1.size(); t++){
		int *arvore_parcial = Lq1[t].second; 
		if ((isEquivalente(Tqh1.first,Lq1[t].first,  g->getQuantVertices()) && t1_domina_t2(Tqh1.second, arvore_parcial, g->get_allArestas())) || isEgal(Tqh1.second, arvore_parcial, g->getQuantArestas())){
			
			Lq1.erase(Lq1.begin()+t); 
			t--;
		}
	}   
	return false; // nao é dominada
	
}
/* Um estado S é um par de vetores (um de vertices, outro de arestas)
 Lq é um vector de pares, ou seja, estados de S do tipo Sq (nivel q)
 L é um map (cuja chave é q) de Lq's

 A funçao retorna Ln

 OBS.: os autores numeram os vértices de 1 à n; nós porém enumeramos de 0 à n-1
*/
vector< pair<int*, int*> > algoritmoPD(Grafo *g){
	int n = g->getQuantVertices();
	map <int, vector< pair<int*, int*> > > L ;//= new map <int, vector< pair<int*, int*> > >[n-1]; // n níveis (1 até n) // nao confundir com a enumeracao dos vertices
	int *X = new int[n];
	for (int i=0; i<n; i++) X[i] = 0;
	X[0] = 1; // pros autores é o indice 1. Nós enumeramos de 0  
	int *E = new int[g->getQuantArestas()]; // inicialmente, vazio
	for (int i=0; i<g->getQuantArestas(); i++) E[i] = 0;
	pair<int*, int*> S11 = make_pair(X, E); // o primeiro par (o primeiro par do nivel 1) 
	vector< pair<int*, int*> > L1;
	L1.push_back(S11);
	L[1] = L1;

	for (int q = 1; q<=n-1; q++) { // para cada nivel. Deve ser no maximo n-1, porque q+1 deve ser no máximo n 
		vector< pair<int*, int*> > Lq = L[q];
		for (int h = 0; h<Lq.size(); h++){ // como Lq é um vector, começaremos do 0
			pair<int*, int*> Sq_h = Lq[h];
			for (int i=0; i<n; i++){
				if ((Sq_h.first)[i] == 1){
					Vertice *v = g->getVertice(i); //pega o vertice de id=i
					for (int j = 0; j<v->getGrau(); j++){ // aqui, j nao é o id da aresta ij
						Aresta *ij = v->getAresta(j); // a priori, nao há como saber quem é a origem ou o destino (i ou j )
						if ((Sq_h.first)[ij->getOrigem()] != (Sq_h.first)[ij->getDestino()]){ // nao gerar ciclo
							int *Xq1 = new int[n];
							for (int o=0; o<n; o++) Xq1[o] = (Sq_h.first)[o];
							//ij->getOrigem()==i ? Xq1[ij->getDestino()] = 1 : Xq1[ij->getOrigem()] = 1;
							Xq1[ij->getDestino()] = 1;
							Xq1[ij->getOrigem()] = 1;
							int *Eqhij = new int[g->getQuantArestas()]; 
							for (int o=0; o<g->getQuantArestas(); o++) Eqhij[o] = (Sq_h.second)[o];
							Eqhij[ij->getId()] = 1;
							pair<int*, int*> Sq1 = make_pair(Xq1, Eqhij);

							// Teste de dominância 
							if (isDominada(Sq1, L[q+1], g) == false){
								L[q+1].push_back(Sq1);
							}
								
						} 
					}
				}
			}
		}
	}

	return L[n];
}


int main(){
	struct tms tempsInit, tempsFinal; // para medir o tempo
	times(&tempsInit);  // pega o tempo do clock inical
	
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

	vector< pair<int*, int*> > arvores = algoritmoPD(&my_grafo);
	 

	times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time<<endl;
	cout<<(float) user_time / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	 
	for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	int *arestas  = arvores[k].second; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<k+1<<endl;
    	float cont1 = 0, cont2=0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			if (arestas[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
    	cout<<endl;
    }
	return 0;
}