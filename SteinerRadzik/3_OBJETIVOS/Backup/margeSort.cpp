#include "Conjunto.h"
#include "Grafo.h"
#include "margeSort.h"
#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>  
using namespace std;

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
/*A implementacao nao está tao legal, mas a complexidade nao é alterada, e por enquanto, vai assim mesmo*/
void intercala(float xl, float yl, float xll, float yll, int p, int q, int r, Aresta **v, int size, int direto, int pares){
	
	
	 /*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem do primeiro objetivo
		2 - se as arestas devem ser na ordem do segundo objetivo
		3 - se as arestas devem ser na ordem do terceiro objetivo
		4 - Ponderacao 
	*/
	/* O Parâmetro "pares" recebe:

		12 - se a combinacao for 1 e 2
		13 - se a combinacao for 1 e 3
		23 - se a combinacao for 2 e 3
	*/
	int i, j, k;
	Aresta **w = new Aresta *[size];
	float peso_i, peso_j; 
	i = p;
	j = q;
	k = 0;
	while (i < q && j < r) {
		if (direto==1 && pares == 12){
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
		} else if (direto==1 && pares == 13){
			if (!maiorIgualQuefloat(v[i]->getPeso1(), v[j]->getPeso1())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso1(), v[j]->getPeso1())){
					if (!maiorQuefloat(v[i]->getPeso3(), v[j]->getPeso3())){
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
		} else if(direto==2 && pares == 12) {
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
		} else if(direto==2 && pares == 23) {
			if (!maiorIgualQuefloat(v[i]->getPeso2(), v[j]->getPeso2())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso2(), v[j]->getPeso2())){
					if (!maiorQuefloat(v[i]->getPeso3(), v[j]->getPeso3())){
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
		}else if(direto==3 && pares == 23) {
			if (!maiorIgualQuefloat(v[i]->getPeso3(), v[j]->getPeso3())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso3(), v[j]->getPeso3())){
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
		}else if(direto==3 && pares == 13) {
			if (!maiorIgualQuefloat(v[i]->getPeso3(), v[j]->getPeso3())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso3(), v[j]->getPeso3())){
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
		} else if(direto==4){

			if (pares == 12){
				peso_i=v[i]->getPeso1()*(yl-yll)+v[i]->getPeso2()*(xll-xl);
				peso_j=v[j]->getPeso1()*(yl-yll)+v[j]->getPeso2()*(xll-xl);
			} else if (pares == 13){
				peso_i=v[i]->getPeso1()*(yl-yll)+v[i]->getPeso3()*(xll-xl);
				peso_j=v[j]->getPeso1()*(yl-yll)+v[j]->getPeso3()*(xll-xl);
			} else { // 23
				peso_i=v[i]->getPeso2()*(yl-yll)+v[i]->getPeso3()*(xll-xl);
				peso_j=v[j]->getPeso2()*(yl-yll)+v[j]->getPeso3()*(xll-xl);
			}

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

void mergesort(float xl, float yl, float xll, float yll, Aresta **v, int size, int direto, int pares){
//v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
//implementação interativa

	int p, r, b=1;;
	while (b<size){
		p=0;
		while (p+b < size){
			r = p + 2*b;
			if (r>size) r = size;
			intercala(xl, yl, xll, yll, p, p+b, r, v, size, direto, pares);
			p = p+2*b;
		}
		b = 2*b;
	}
}