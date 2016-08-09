#ifndef _VERTICE_H_
#define _VERTICE_H_


#include "Vertice.h"
#include "Aresta.h"


Vertice::Vertice (int id2){ // n = número de vértices do grafo
	id = id2;
	grau_saida = 0;
	grau_chegada = 0;

}
Vertice::Vertice(){};
		
int Vertice::getId(){
	return id;
}

int Vertice::getGrau_saida(){
	return grau_saida;
}

int Vertice::getGrau_chegada(){
	return grau_chegada;
}

void Vertice::incrementaGrau_saida(){
	grau_saida++;
}

void Vertice::incrementaGrau_chegada(){
	grau_chegada++;
}

void Vertice::decrementaGrau_saida(){
	if (grau_saida>0) grau_saida--;
}

void Vertice::decrementaGrau_chegada(){
	if (grau_chegada>0) grau_chegada--;
}

void Vertice::adicionaAresta(Aresta *a){
	adjacentes.push_back(a);			
}
		
Aresta* Vertice::getAresta(int i){ /*retorna um ponteiro para a i-ésima aresta insidente no vértice*/
	return adjacentes[i];		
}
		
int Vertice::getGrau(){ 
	return adjacentes.size();
}

#endif