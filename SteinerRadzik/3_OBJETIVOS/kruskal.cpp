#include "Conjunto.h"
#include "kruskal.h"
#include "Grafo.h"
#include "margeSort.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
using namespace std;

#define PRECISAO 0.00001



bool kruskal (Grafo *g, Aresta **listaAresta,  int  *A, float &x, float &y, float &z){
	
	Conjunto conjunto(g->getQuantVertices());
	int cont=0; //listaAresta[k]->getId()
	int i=0;
	//float peso;
	x=0;y=0;z=0;
	//Aresta **listaAresta = g->getAllArestasPtr();
	for (int k=0; k<g->getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	
		 	//A[listaAresta[k]->getId()] = 1;
			A[cont++] = listaAresta[k]->getId();
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			//custo+=listaAresta[k]->getPeso1()*(yl-yll)+listaAresta[k]->getPeso2()*(xll-xl);
			x+=listaAresta[k]->getPeso1();
			y+=listaAresta[k]->getPeso2();
			z+=listaAresta[k]->getPeso3();
		}
	}
	//mergesort(xl, yl, xll, yll, listaAresta, g->getQuantArestas(), direto);
	i=0;

	while (cont<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			//A[listaAresta[i]->getId()] = 1;
			A[cont++] = listaAresta[i]->getId();
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			x+=listaAresta[i]->getPeso1();
			y+=listaAresta[i]->getPeso2();
			z+=listaAresta[i]->getPeso3();
		}
		i++;
	}
	conjunto.desaloca();
	
	if (cont==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}