/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2018
#-----------------------------------------------------------------------
# This code implements the Santos and Pugliese's (2018) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The user time is returned;
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h> 
#include <list>
using namespace std;

struct tms tempsInit, tempsFinal; // para medir o tempo
Grafo my_grafo; // o grafo 
int n; // quantidade de vértices do grafo

typedef struct pp
{
	list<Aresta *> sequenciaDeArestas;
	int peso1 = 0;
	int peso2 = 0;
}Path; // representa um caminho, simplesmente com a lista de arestas que o compoem a sequência

typedef struct ss
{
	list<Path* > listaCaminhos; // lista de caminhos
	bool *X; // lista de nos associados ao caminho (X \in N) = O mesmo X do Algoritmo 3 do artigo
	// vértices marcados (true) e nao marcados (false)
}Stipo;
// Vai ser criada uma lista de Stipo, com no maximo Z^{n-1}

std::vector<Stipo* > S; // o S do algoritmo 3 do artigo
list<Path* > U; //o  U do algoritmo 3 do artigo

map <int, Aresta *> allArestas;

bool equalSets(bool *X1, bool *X2){ // retorna true se os conjuntos X1 e X2 forem iguais
	for (int i=0; i<n; i++){ // X1 in X2
		if (X1[i]!=X2[i]) return false;
	} 
	return true;
}

pair<int, int> getEp(Path *p){ // aquele e(p) do algoritmo 3
	if (p->sequenciaDeArestas.size()==0){
		return make_pair(0,0); // [1,1];
	} else {
		Aresta* a = p->sequenciaDeArestas.back();
		return make_pair(a->getOrigem(), a->getDestino());
	}
}

std::vector<int> getSequence(Path *p){
	std::vector<int> aux;
	list<Aresta *>::iterator it = p->sequenciaDeArestas.begin();
	aux.push_back(0);
	while (it!=p->sequenciaDeArestas.end()){
		Aresta *a = *it;
		int dest = a->getDestino(); //getEp(Path p)
		aux.push_back(dest);
		it++;
	}
	
	return aux;

}

Aresta *getAresta(int origem, int destino){
	for (int i=0; i<allArestas.size(); i++){
		if (allArestas[i]->getOrigem()==origem && allArestas[i]->getDestino()==destino){
			return allArestas[i];
		}
	}
	return NULL;
}

Stipo * getSX(bool *Y){
	for (int i=0; i<S.size(); i++){
		if (equalSets(S[i]->X,Y)) {
			return S[i];
		}
	}
	Stipo *ret = new Stipo();
	ret->X = new bool[n];
	for (int i=0; i<n; i++) ret->X[i] = Y[i]; // cria um novo S com copia de Y
	S.push_back(ret);
	return ret;
}

// retorna true se for dominado por alguém em SY. 
// retorna false se nao for dominado por ninguém
bool isDominado(Path *p, Stipo *SY){
	list<Path* > listaCaminhos = SY->listaCaminhos;
	list<Path* >::iterator it =listaCaminhos.begin() ;
	while (it!=listaCaminhos.end()){
		Path *pyy = (*it);
		if (pyy->peso1 <= p->peso1 && pyy->peso2 <= p->peso2 && (pyy->peso1 < p->peso1 || pyy->peso2 < p->peso2)){
			return true;
		}
		it++;
	}
	return false;
}

void removeDominadas(Path *p, Stipo *SY){
	list<Path* > listaCaminhos = SY->listaCaminhos;
	list<Path* >::iterator it =listaCaminhos.begin() ;
	list<list<Path* >::iterator> remover;
	while (it!=listaCaminhos.end()){
		Path *pyy = (*it); // procura por alguém dominado por p
		if (p->peso1 <= pyy->peso1 && p->peso2 <= pyy->peso2  && (p->peso1 < pyy->peso1 || p->peso2 < pyy->peso2)){
			// remove pyy de SY
			remover.push_back(it);
		}
	}
	list<list<Path* >::iterator>::iterator re = remover.begin();
	while (re!=remover.end()){
		delete **re;
		SY->listaCaminhos.erase(*re);
		re++;
	}

}

void searchNewNDminimalPaths(Path *p, int i, int j, bool *Y){
	for (int w = j+1; w<n; w++){ // j+1 mesmo?
		if (Y[w]==false){
			Aresta *a = getAresta(i, w);
			if (a!=NULL){ // Se NULL, entao nao existe aresta i,w
				Y[w] = true; // a gente marca w em Y so para poder utilizar mais adiante. Depois desmarca, senao ele ficar marcado em Y fora desta sub-rotina
				Path *pl = new Path();
				*pl = *p; // ATECAO: isso copia a lista também?
				pl->sequenciaDeArestas.push_back(a); // adiciona aresta ao caminho NA ULTIMA POSICAO;
				pl->peso1 += a->getPeso1();
				pl->peso2 += a->getPeso2();
				Stipo *SY = getSX(Y);
				if (isDominado(pl,SY)==false){
					U.push_back(pl); // fila
					SY->listaCaminhos.push_back(pl);
  					// removeDominadas(pl, SY);//remove de SY as dominadas por pl

				} else delete  pl;
  				
				Y[w] = false;
			}
		}
	}	
}

int main(){
	
	times(&tempsInit);  // pega o tempo do clock inical

	// // para medir o tempo em caso limite
	// pthread_t thread_time; 
	// pthread_attr_t attr;
	// int nnnnnnnn=0;
	// if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
 //        printf("Error to create the thread");
 //        exit(-1);
 //    }
    
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	my_grafo.setN(n);
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

	allArestas = my_grafo.get_allArestas();


	Stipo *S0 = new Stipo();
	S0->X = new bool[n];
	for (int i=0; i<n; i++) S0->X[i] = false; // inicialmente, todos falsos
	S0->X[0] = true; // X = {1} // inicialmente, apenas o primeiro vertice.
	


	Path *p = new Path(); // inicialmente, sem arestas;
	S0->listaCaminhos.push_back(p);
	U.push_back(p); 
	S.push_back(S0);
	list<Path* >::iterator it = U.begin();
	// U funciona como uma FILA: primeiro sai quem primeiro entrou. Os elementos sao inseridos no fim da fila
	

	bool *Xaux = new bool[n]; // um X auxiliar

	// Aresta *a01 = getAresta(0,2);
	// Aresta *a12 = getAresta(2,3);
	// p->sequenciaDeArestas.push_back(a01);
	// p->sequenciaDeArestas.push_back(a12);
	// std::vector<int> seq = getSequence(p);
	// for (int i=0; i<seq.size(); i++) cout<<"seq["<<i<<"] = "<<seq[i]<<endl;

	while (it!=U.end()){
		p = *it;
		for (int i=0; i<n; i++) Xaux[i] = false;
		std::vector<int> seq = getSequence(p);
		for (int i=0; i<seq.size(); i++) Xaux[seq[i]] = true;
		pair<int, int> eij = getEp(p); //[i,j]
		int l; // o index de eij.first em seq
		for (int i=0; i<seq.size(); i++){
			if(seq[i]==eij.first){
				l = i;
				break;
			}
		}
		searchNewNDminimalPaths(p, eij.first, eij.second,Xaux);
		for (int x=l+1; x<seq.size(); x++){
			searchNewNDminimalPaths(p, seq[x], -1, Xaux); // acho que é -1, porque nossa faixa de indeces vai de 0 a n-1
		}

		it++; //última linha do while
	}


	for (int i=0; i<n; i++) Xaux[i] = true;
	Stipo *SN = getSX(Xaux);
	cout<<SN->listaCaminhos.size()<<endl;
	for (list<Path* >::iterator iptl=SN->listaCaminhos.begin(); iptl!=SN->listaCaminhos.end(); iptl++){
		cout<<(*iptl)->peso1<<" "<<(*iptl)->peso2<<endl;
	}	


    times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time<<endl;
	cout<<(float) user_time / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	// printResultado();

	return 0;
}