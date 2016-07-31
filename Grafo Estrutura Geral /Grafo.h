#include "Vertice.h"
using namespace std;


#define MAX1 20000
#define PRECISAO 0.0001

class Grafo{
	private:
		int n;
		Vertice **lista_vertices; /*Lista (fixa) de vértice. É um vetor de ponteiros. Cada vértice possui uma lista de arestas adjacentes.*/
		vector <Aresta *> lista_allArestas; /*lista de todas as arestas do Grafo. É um vetor de ponteiros*/
	public:
		Grafo(int n1);
		void addVertice(int id); // lista_vertices[id] = vertice de identificador id (entre 0 e n-1)
		Vertice *getVertice(int id);
		Aresta *addAresta(int id, int origem, int destino, float peso1, float peso2);
		vector <Aresta *> get_allArestas();
		int getQuantArestas();
		int getQuantVertices();
};

