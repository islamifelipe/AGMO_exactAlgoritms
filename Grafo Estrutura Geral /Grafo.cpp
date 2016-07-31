#include "Grafo.h"

using namespace std;

Grafo::Grafo(int n1){
	int n = n1;
	lista_vertices = new Vertice*[n];
}

void Grafo::addVertice(int id){
	Vertice *novo = new Vertice(id);
	lista_vertices[id] = novo; /*PADR�O: A faixa de id's dos v�rtices � de 0 at� n-1*/
}

Vertice *Grafo::getVertice(int id){
	return lista_vertices[id]; 		
}

Aresta *Grafo::addAresta(int id, int origem, int destino, float peso1, float peso2){
	Aresta *nova = new Aresta(id, origem, destino, peso1, peso2);
	lista_allArestas.push_back(nova); /*deve ser passado o ponteiro, isto �, a refer�ncia*/
	lista_vertices[origem]->adicionaAresta(nova);
	lista_vertices[destino]->adicionaAresta(nova);
	return nova;
}

vector<Aresta*> Grafo::get_allArestas(){ /*retorna a primeira posi��o do vetor lista_allArestas*/
	return lista_allArestas;
}
int Grafo::getQuantArestas(){
	return lista_allArestas.size();
}

int Grafo::getQuantVertices(){	
	return n;	
}