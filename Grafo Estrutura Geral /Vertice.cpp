#include "Vertice.h"
using namespace std;


Vertice::Vertice (int id2){ // n = número de vértices do grafo
	id = id2;
}
Vertice::Vertice(){};
		
int Vertice::getId(){
	return id;
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