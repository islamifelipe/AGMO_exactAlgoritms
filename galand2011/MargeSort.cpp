#include "Grafo.h"
#include "MargeSort.h"
#include <cmath>
using namespace std;

//#define PRECISAO 0.00001

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
void intercala(float lambda1, float lambda2, int p, int q, int r, Aresta **v, int size, int direto){
	
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

			peso_i=v[i]->getPeso1()*lambda1 + v[i]->getPeso2()*lambda2;
			peso_j=v[j]->getPeso1()*lambda1 + v[j]->getPeso2()*lambda2;
			
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

void mergesort(float lambda1, float lambda2, Aresta **v, int size, int direto){
//v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
//implementação interativa
	
	int p, r, b=1;;
	while (b<size){
		p=0;
		while (p+b < size){
			r = p + 2*b;
			if (r>size) r = size;
			intercala(lambda1, lambda2, p, p+b, r, v, size, direto);
			p = p+2*b;
		}
		b = 2*b;
	}
}
