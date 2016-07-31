#include <iostream>
#include <map> 
#include <string>
#include "Grafo.h"
using namespace std;

bool a_domina_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2())){
		return true;
	} else return false;
}

bool a_domina_fracamente_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2()){
		return true;
	} else return false;
}

int* vmin(int* vertices, Grafo &my_grafo){
	map <int, Aresta *> arestas = my_grafo.get_allArestas();
	int *retorno = new int[arestas.size()]; // vetor dos id das arestas

	// primeiramente, seleciona-se as arestas conforme a regra classica de conjunto dijunto de prim   

	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *a = arestas[i];
		if (vertices[a->getOrigem()] != vertices[a->getDestino()]){
			retorno[a->getId()] = 1; 
		} else {
			retorno[a->getId()] = 0; 
		}
	}

	// depois verificamos a dominância entre as arestas

	for (int i = 0; i<arestas.size(); i++){ // O(m^2)
		if (retorno[i] == 1){ // se a aresta de id=i está contida no conjunto... 
			Aresta *ai = arestas[i];
			for (int j = i+1; j<arestas.size(); j++){
				if (retorno[j] == 1){
					if (a_domina_b(ai, arestas[j])){
						retorno[j] = 0;
					} else if (a_domina_b(arestas[j], ai)){
						retorno[i] = 0;
					}
				}
			}
		}
	}
	return retorno;
}

int main(){
	int n;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}
	int nA = id; // quantidade de arestas do grafo	

	map<int, int* > *X = new map<int, int* >[n];
	/* X é um vetor de map's. 
	 * cada índice do vetor (0...n-1), representa o valor das iteracoes r do algoritmo do Corley
	 * cada item o map é um valor de k = 1 ... mr
	 * O tamanho de X nao aumenta. Somente aumenta o tamanho do map
     * dado um id do map com m elementos, busca é feita em O(mlogm)
     * O valor do map é um vator de **** n **** inteiros que representam os vértices 
     incluidos na arvore. Se um indice i for 1, entao o vertice 1 está na arvore  
	 */

	 map<int, int* > *A = new map<int, int* >[n];
	 /* Semelhante ao vetor X, mas aqui o int* é um vetor de nA inteiros 
	  * se um indice i do int* for 1, entao a i-esima aresta (0 à nA-1) está presente na arvore   	
	  */
     int m[n+1]; // de 1 à n : nao utilisar o 0
     m[1] = 1;


     X[1][1][0] = 1; 
     /* primeiro inidce r = 1 
        segundo indice k = 1
        terceiro indice vertice 0 incluso
     */ 
    for (int r = 1; r<n; r++){
    	map<int, int* > Wr; 
    	for (int k = 1; k<m[r]; k++){
    		Wr[k] = vmin(X[r][k], my_grafo);
    	}
	     /* pra guardar o retorno do Step2
	     O indice do map é o valor de K (que será reinicializado a cada execucao do passo 2)
	     o int* é o conjunto de id de arestas 
	     */
    }
	return 0;
}