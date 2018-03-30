#ifndef _GRAFO_H_
#define _GRAFO_H_

#include "Vertice.h"
#include <map>

#define MAX1 20000
//#define PRECISAO 0.0001

class Grafo{
	private:
		int n;
		Vertice **lista_vertices; /*Lista (fixa) de vértice. É um vetor de ponteiros. Cada vértice possui uma lista de arestas adjacentes.*/
		map <int, Aresta *> lista_allArestas; /*lista de todas as arestas do Grafo.*/
		Aresta ** arestasPtr;
		int particao[MAX1]; // vetor alocado dinamicamente no construtor. Size: quantidade de arestas 
		/*Este vetor (partição) é construído e interpretado da seguinte maneira:
			particao[i] = 0  ---> indica que a aresta de id=i é opcional (open)
			particao[i] = 1  ---> indica que a aresta de id=i é obrigatória 
			particao[i] = 2  ---> indica que a aresta de id=i é proibida
		*/
	public:
		Grafo();
		Grafo(int n1);
		void addVertice(int id); // lista_vertices[id] = vertice de identificador id (entre 0 e n-1)
		Vertice *getVertice(int id);
		Aresta *addAresta(int id, int origem, int destino, float peso1, float peso2);
		map <int, Aresta *> get_allArestas();
		Aresta ** getAllArestasPtr();
		void gerarArestasPtr();
		int getQuantArestas();
		int getQuantVertices();
		int getStatus(int i);
		void setStatus(int i, int valor);
		void setN(int n1);
		Grafo& operator=( Grafo& d);
};

#endif
