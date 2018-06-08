#include "Grafo.h"

using namespace std;

Grafo::Grafo(int n1){
	n = n1;
	lista_vertices = new Vertice*[n];
}


Grafo::Grafo(){}
void Grafo::setN(int n1){
	n = n1;
	lista_vertices = new Vertice*[n];
}

void Grafo::addVertice(int id){
	Vertice *novo = new Vertice(id);
	lista_vertices[id] = novo; /*PADRÃO: A faixa de id's dos vértices é de 0 até n-1*/
}

Vertice *Grafo::getVertice(int id){
	return lista_vertices[id]; 		
}

Aresta *Grafo::addAresta(int id, int origem, int destino, float peso1, float peso2, float peso3, float peso4){
	Aresta *nova = new Aresta(id, origem, destino, peso1, peso2, peso3, peso4);
	//lista_allArestas.push_back(nova); /*deve ser passado o ponteiro, isto é, a referência*/
	lista_allArestas[id] = nova;
	lista_vertices[origem]->adicionaAresta(nova);
	lista_vertices[destino]->adicionaAresta(nova);
	return nova;
}

map <int, Aresta *> Grafo::get_allArestas(){ /*retorna a primeira posição do vetor lista_allArestas*/
	return lista_allArestas;
}
int Grafo::getQuantArestas(){
	return lista_allArestas.size();
}

int Grafo::getQuantVertices(){	
	return n;	
}