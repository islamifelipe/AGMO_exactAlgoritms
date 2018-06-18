#include "Conjunto.h"
#include "kruskal.h"
#include "Grafo.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
using namespace std;

#define PRECISAO 0.00001

bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}


/*Implementação do MargeSort para utilizar no algortimo de Kruskal*/
void intercala(float xl, float yl, float xll, float yll, int p, int q, int r, Aresta **v, int size, int direto){
	/* s'  = (x' , y' ) <--> (xl , yl )
	 * s'' = (x'', y'') <--> (xll , yll )
	 */
	 /*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem lexicográfica direta
		2 - se as arestas devem ser na ordem lexocográfica inversa  
		3 - se as arestas devem ser na ordem crescente da pondenraçao dos pesos das arestas (formula do método geometrico da busca dicotômica)
	  */
	int i, j, k;
	Aresta **w = new Aresta *[size];
	float peso_i, peso_j; 
	i = p;
	j = q;
	k = 0;
	while (i < q && j < r) {
		if (direto==1){
			if (!maiorIgualQuefloat(v[i]->getPeso1(), v[j]->getPeso1())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso1(), v[j]->getPeso1())){
					if (!maiorQuefloat(v[i]->getPeso2(), v[j]->getPeso2())){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		} else if(direto==2) {
			if (!maiorIgualQuefloat(v[i]->getPeso2(), v[j]->getPeso2())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso2(), v[j]->getPeso2())){
					if (!maiorQuefloat(v[i]->getPeso1(), v[j]->getPeso1())){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		} else {
			//peso_i=v[i]->getPeso1()*(yl-yll)+v[i]->getPeso2()*(xll-xl); //Steiner and Radzik
			//peso_j=v[j]->getPeso1()*(yl-yll)+v[j]->getPeso2()*(xll-xl); //Steiner and Radzik
			//peso_i = (v[i]->getPeso1()*(xll - xl)-v[i]->getPeso2()*(yll-yl)); // ramos algoritmo
			//peso_j = (v[j]->getPeso1()*(xll - xl)-v[j]->getPeso2()*(yll-yl));
			peso_i = (-1)*(v[i]->getPeso1()*(yll - yl)-v[i]->getPeso2()*(xll-xl)); // ramos texto
			peso_j = (-1)*(v[j]->getPeso1()*(yll - yl)-v[j]->getPeso2()*(xll-xl));
			//peso_i=(v[i]->getPeso1()*(yl-yll)-v[i]->getPeso2()*(xll-xl)); //Monteiro ERRADO (deve ser +)
			//peso_j=(v[j]->getPeso1()*(yl-yll)-v[j]->getPeso2()*(xll-xl)); //Monteiro ERRADO (deve ser +)
			//peso_i = v[i]->getPeso1()*(abs(yll - ysl))+v[i]->getPeso2()*(abs(xll-xl)); // Beth OK
			//peso_j = v[j]->getPeso1()*(abs(yll - yl))+v[j]->getPeso2()*(abs(xll-xl));
			
			if (!maiorIgualQuefloat(peso_i,peso_j)) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(peso_i, peso_j)){
					if (!maiorQuefloat(peso_i, peso_j)){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		}
		k++;
	}
	while (i < q) {
		w[k] = v[i];
		i++;
		k++;
	}
	while (j < r) {
		w[k] = v[j];
		j++;
		k++;
	}
	for (i = p; i < r; i++) v[i] = w[i-p];
	delete[] w;
}

void mergesort(float xl, float yl, float xll, float yll, Aresta **v, int size, int direto){
//v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
//implementação interativa
	/* s'  = (x' , y' ) <--> (xl , yl )
	 * s'' = (x'', y'') <--> (xll , yll )
	 */
	int p, r, b=1;;
	while (b<size){
		p=0;
		while (p+b < size){
			r = p + 2*b;
			if (r>size) r = size;
			intercala(xl, yl, xll, yll, p, p+b, r, v, size, direto);
			p = p+2*b;
		}
		b = 2*b;
	}
}
bool kruskal (Grafo *g, int  *A, float xl, float yl, float xll, float yll, long double &custo, float &x, float &y, int direto){
	/*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem lexicográfica direta
		2 - se as arestas devem ser na ordem lexocográfica inversa  
		3 - se as arestas devem ser na ordem crescente de "pesos ponderado"*/
	/* s'  = (x' , y' ) <--> (xl , yl )
	 * s'' = (x'', y'') <--> (xll , yll )
	 */
	 for (int i=0; i<g->getQuantArestas(); i++){
	 	A[i] = 0;
	 }
	Conjunto conjunto(g->getQuantVertices());
	int cont=0, i=0;
	x = 0;y= 0;
	custo =0;
	//float peso;
	Aresta **listaAresta = g->getAllArestasPtr();
	for (int k=0; k<g->getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		// cout<<g->getStatus(listaAresta[k]->getId())<<endl;
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	
		 	A[listaAresta[k]->getId()] = 1;
		
		 	cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			custo+=listaAresta[k]->getPeso1()*(yl-yll)+listaAresta[k]->getPeso2()*(xll-xl);
			x +=listaAresta[k]->getPeso1();
			y += listaAresta[k]->getPeso2();
		}
	}

	mergesort(xl, yl, xll, yll, listaAresta, g->getQuantArestas(), direto);
	i=0;
	while (cont<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
			A[listaAresta[i]->getId()] = 1;
			// cout<<listaAresta[i]->getPeso1()<<" "<<listaAresta[i]->getPeso2()<<endl;
			
			// cout<<xl<<" "<<yl<< " "<<xll<<" "<<yll<<endl;
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			custo+=listaAresta[i]->getPeso1()*(yl-yll)+listaAresta[i]->getPeso2()*(xll-xl);
			// cout<<"custo = "<<custo<<endl;
			x +=listaAresta[i]->getPeso1();
			y += listaAresta[i]->getPeso2();
		}
		i++;
	}
	conjunto.desaloca();
	
	if (cont==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}
