#include <vector>
#include "Aresta.h"
using namespace std;
 
class Vertice{
	private: 
		int id; /*identificador do vértice*/
		vector<Aresta*> adjacentes; /*é um vetor (de ponteiros) de arestas que saem do vértice*/
		int grau_saida, grau_chegada;
	public:
		Vertice (int id2); 
		
		Vertice();
		
		int getId();
		
		void adicionaAresta(Aresta *a);
		
		Aresta* getAresta(int i);
		
		int getGrau();

		int getGrau_saida(); 
		int getGrau_chegada();
		void incrementaGrau_saida(); 
		void incrementaGrau_chegada();
		void decrementaGrau_saida(); 
		void decrementaGrau_chegada();


		
};
