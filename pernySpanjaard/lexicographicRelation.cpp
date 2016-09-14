/*
# Este programa é utilizado somente para gerar o grafo de relaçao binaria lexicografic
# que será fornecido como entrada ao Perny e Spanjaard (2003). Seu objetivo é somente evitar escrever a relaçao lexicografica na mao ;-) 
# Caso o Perny e Spanjaard (2003) seja utilizado com outra relaçao de preferência 
# Nao há necessidade de se executar este programa (basta fornecer a relaçao desejada como entrada ao Perny e Spanjaard (2003)) 
# Entrada : instância do grafo G(V, E) conexo nao direcionado que será fornecido ao  Perny e Spanjaard (2003)
# Saida : grafo G'(V', E') direcionado (possivelmente desconexo) representando a relacao lexicografica (e que deve também ser fornecido como entrada ao Perny e Spanjaard (2003)) 

existe (i->j) em E' <----> i R j 

*/

#include <iostream>
#include <map> 
#include <string>
#include "Grafo.h"
#include <cmath>
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

// void intercala(int p, int q, int r, Aresta **v, int size, int direto){
	
// 	 /*O parâmetro "direto" recebe:
// 		1 - se as arestas devem ser na ordem lexicográfica direta
// 		2 - se as arestas devem ser na ordem lexocográfica inversa  
// 	 */
// 	int i, j, k;
// 	Aresta **w = new Aresta *[size];
// 	float peso_i, peso_j; 
// 	i = p;
// 	j = q;
// 	k = 0;
// 	while (i < q && j < r) {
// 		if (direto==1){
// 			if (!maiorIgualQuefloat(v[i]->getPeso1(), v[j]->getPeso1())) {
// 				w[k] = v[i];
// 				i++;
// 			} else {
// 				if (equalfloat(v[i]->getPeso1(), v[j]->getPeso1())){
// 					if (!maiorQuefloat(v[i]->getPeso2(), v[j]->getPeso2())){
// 						w[k] = v[i];
// 						i++;
// 					} else {
// 						w[k] = v[j];
// 						j++;	
// 					}
// 				} else {
// 					w[k] = v[j];
// 					j++;
// 				}
// 			}
// 		} else if(direto==2) {
// 			if (!maiorIgualQuefloat(v[i]->getPeso2(), v[j]->getPeso2())) {
// 				w[k] = v[i];
// 				i++;
// 			} else {
// 				if (equalfloat(v[i]->getPeso2(), v[j]->getPeso2())){
// 					if (!maiorQuefloat(v[i]->getPeso1(), v[j]->getPeso1())){
// 						w[k] = v[i];
// 						i++;
// 					} else {
// 						w[k] = v[j];
// 						j++;	
// 					}
// 				} else {
// 					w[k] = v[j];
// 					j++;
// 				}
// 			}
// 		}
// 		k++;
// 	}
// 	while (i < q) {
// 		w[k] = v[i];
// 		i++;
// 		k++;
// 	}
// 	while (j < r) {
// 		w[k] = v[j];
// 		j++;
// 		k++;
// 	}
// 	for (i = p; i < r; i++) v[i] = w[i-p];
// 	delete[] w;
// }

// void mergesort( Aresta **v, int size, int direto){
// //v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
// //implementação interativa

// 	int p, r, b=1;;
// 	while (b<size){
// 		p=0;
// 		while (p+b < size){
// 			r = p + 2*b;
// 			if (r>size) r = size;
// 			intercala(p, p+b, r, v, size, direto);
// 			p = p+2*b;
// 		}
// 		b = 2*b;
// 	}
// }
int main(){

	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; 
	cin>>n; // nao sera utilizao
	//cin>>m;
	Aresta ** arestas = new Aresta*[m]; 
	
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		Aresta *nova = new Aresta(id, origem, destino, peso1, peso2);
		arestas[id] = nova;
		id++;
	}

	//cout<<id<<endl;
	//fim de leitura
	//mergesort(arestas, m, 1);
	cout<<n<<" "<<id<<endl;
	for (int i=0; i<id; i++){
		Aresta *e = arestas[i];
		cout<<e->getOrigem()<<" "<<e->getDestino()<<" "<<e->getPeso1()<<" "<<e->getPeso2()<<endl;
	}
	for (int i=0; i<id; i++){
		Aresta *e = arestas[i];
		for (int j=0; j<id; j++){
			if (i!=j){
				Aresta *e2 = arestas[j];
				if (e->getPeso1()<e2->getPeso1() || (e->getPeso1()==e2->getPeso1() && e->getPeso2()<e2->getPeso2())){
					cout<<e->getId()<<" "<<e2->getId()<<endl;
				}
			}
		}
	}
	// for (int i=0; i<m-1; i++){
	// 	cout<<arestas[i]->getId()<<" "<<arestas[i+1]->getId()<<endl;
	// }
	return 0;
}