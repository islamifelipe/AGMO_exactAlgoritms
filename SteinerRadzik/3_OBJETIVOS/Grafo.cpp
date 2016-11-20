#include "Grafo.h"
#include <iostream>
using namespace std;

Grafo::Grafo(){};
Grafo::Grafo(int n1){
	n = n1;
	lista_vertices = new Vertice*[n];
	for (int i=0; i<lista_allArestas.size(); i++){
		particao[i] = 0; /*inicialmente opcional*/
	}
}

void Grafo::addVertice(int id){
	Vertice *novo = new Vertice(id);
	lista_vertices[id] = novo; /*PADR�O: A faixa de id's dos v�rtices � de 0 at� n-1*/
}

Vertice *Grafo::getVertice(int id){
	return lista_vertices[id]; 		
}

Aresta *Grafo::addAresta(int id, int origem, int destino, float peso1, float peso2, float peso3){
	Aresta *nova = new Aresta(id, origem, destino, peso1, peso2, peso3);
	//lista_allArestas.push_back(nova); /*deve ser passado o ponteiro, isto �, a refer�ncia*/
	lista_allArestas[id] = nova;
	lista_vertices[origem]->adicionaAresta(nova);
	lista_vertices[destino]->adicionaAresta(nova);
	return nova;
}

Aresta * Grafo::getArestas(int i){ /*retorna a primeira posi��o do vetor lista_allArestas*/
	return lista_allArestas[i];
}
int Grafo::getQuantArestas(){
	return lista_allArestas.size();
}
map <int, Aresta *> Grafo::get_allArestas(){
	return lista_allArestas;

}

int Grafo::getQuantVertices(){	
	return n;	
}
Aresta ** Grafo::getAllArestasPtr(){
	Aresta **arestasPtr = new Aresta*[lista_allArestas.size()];
	for (int i=0; i<lista_allArestas.size(); i++){
		arestasPtr[i] = lista_allArestas[i];
	}
	return arestasPtr;
}
// void Grafo::gerarArestasPtr(){
// 	Aresta **arestasPtr = new Aresta*[lista_allArestas.size()];
// 	for (int i=0; i<lista_allArestas.size(); i++){
// 		arestasPtr[i] = lista_allArestas[i];
// 	}
// }
int Grafo::getStatus(int i){
	return particao[i];
}
void Grafo::setStatus(int i, int valor){
	particao[i] = valor;
}

Grafo& Grafo::operator=( Grafo& d){
	
	if (this == &d) return *this;
	n = d.getQuantVertices();
	cout<<"ok"<<endl;
	for (int i=0; i<d.lista_allArestas.size(); i++){
		particao[i] = d.getStatus(i);
	}
	lista_allArestas = d.lista_allArestas;
	return *this;
	
}

