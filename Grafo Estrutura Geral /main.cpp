#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sys/time.h>
#include <vector>
#include "Grafo.h"
using namespace std;

int main(){
	int n, s, t;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	//cin>>s; // vértice origem do caminho
	//cin>>t; // vértice destino do caminho
	Grafo my_grafo(n);
	for (int i=0; i<n; i++){
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}
	/*==========TESTE==========*/
	
	for (int i=0; i<n; i++){
		cout<<"vertice "<<i<<": ";
		for (int j=0; j<my_grafo.getVertice(i)->getGrau(); j++){
			cout<<"("<< my_grafo.getVertice(i)->getAresta(j)->getOrigem()<<", "<<  my_grafo.getVertice(i)->getAresta(j)->getDestino()<<"), ";
		}
		cout<<endl;
	}
	
	cout<<"Lista de Arestas: "<<endl;
	vector <Aresta*> allArestas = my_grafo.get_allArestas();
	for (int i=0; i<my_grafo.getQuantArestas(); i++){
		cout<<"("<<allArestas[i]->getOrigem()<<", "<<allArestas[i]->getDestino()<<")"<<endl;
	}
	
	/*========END-TESTE========*/
	
	return 0;
}