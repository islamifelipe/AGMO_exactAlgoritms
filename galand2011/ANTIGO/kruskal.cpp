#include "Conjunto.h"
#include "kruskal.h"
#include "Grafo.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
#include "MargeSort.h"
using namespace std;

bool kruskal (Grafo *g, int  *A, Aresta **listaAresta, float lambda1, float lambda2, float &custo, float &x, float &y, int direto){
	/*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem lexicográfica direta
		2 - se as arestas devem ser na ordem lexocográfica inversa  
		3 - se as arestas devem ser na ordem crescente de "pesos ponderado"*/
	
	Conjunto conjunto(g->getQuantVertices());
	int cont=0, i=0;
	//float peso;
	custo =0;
	//Aresta **listaAresta = g->getAllArestasPtr();
	for (int k=0; k<g->getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	if (conjunto.compare(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino())) return false;
		 	A[listaAresta[k]->getId()] = 1;
		
		 	cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			
			custo+=listaAresta[k]->getPeso1()*(lambda1)+listaAresta[k]->getPeso2()*(lambda2);
			x+=listaAresta[k]->getPeso1();
			y+=listaAresta[k]->getPeso2();
		}
	}
	//mergesort( lambda1,  lambda2, listaAresta, g->getQuantArestas(), direto);
	i=0;
	while (cont<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
			A[listaAresta[i]->getId()] = 1;
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			custo+=listaAresta[i]->getPeso1()*(lambda1)+listaAresta[i]->getPeso2()*(lambda2);
			x+=listaAresta[i]->getPeso1();
			y+=listaAresta[i]->getPeso2();
		}
		i++;
	}
	conjunto.desaloca();
	
	if (cont==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}
