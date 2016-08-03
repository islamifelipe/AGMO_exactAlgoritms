/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include "Grafo.h"
#include "Conjunto.h"
using namespace std;

#define PRECISAO 0.00001

inline bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

inline bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

inline bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}


bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

bool isEgalObjetive(int *t1, int *t2, map <int, Aresta *> arestas){
	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
		} 
		if (t2[i]==1){
			t2_peso1+=arestas[i]->getPeso1();
			t2_peso2+=arestas[i]->getPeso2();
		}
	}
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);

}
bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){
	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
		} 
		if (t2[i]==1){
			t2_peso1+=arestas[i]->getPeso1();
			t2_peso2+=arestas[i]->getPeso2();
		}
	}
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
		return true;
	} else return false;
}

void getXandY(int *t, map <int, Aresta *> arestas, float &X, float &Y ){
	X = 0; Y = 0;
	for (int i=0; i<arestas.size(); i++){
		if (t[i]==1){
			X+=arestas[i]->getPeso1();
			Y+=arestas[i]->getPeso2();
		} 
	}
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
			peso_i=v[i]->getPeso1()*(yl-yll)+v[i]->getPeso2()*(xll-xl);
			peso_j=v[j]->getPeso1()*(yl-yll)+v[j]->getPeso2()*(xll-xl);
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
bool kruskal (Grafo *g, int  *A, float xl, float yl, float xll, float yll, int direto){
	/*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem lexicográfica direta
		2 - se as arestas devem ser na ordem lexocográfica inversa  
		3 - se as arestas devem ser na ordem crescente de "pesos ponderado"*/
	/* s'  = (x' , y' ) <--> (xl , yl )
	 * s'' = (x'', y'') <--> (xll , yll )
	 */
	Conjunto conjunto(g->getQuantVertices());
	int cont=0, i=0;
	//float peso;
	Aresta **listaAresta = g->getAllArestasPtr();
	for (int k=0; k<g->getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	
		 	A[listaAresta[k]->getId()] = 1;
		 	//A[cont++] = listaAresta[k];
		 	cout<<"AQUI1"<<endl;
		 	cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			//peso=listaAresta[k]->getPeso1()*(g->getYl()-g->getYll())+listaAresta[k]->getPeso2()*(g->getXll()-g->getXl());
			//custo+=peso;
		}
	}
	mergesort(xl, yl, xll, yll, listaAresta, g->getQuantArestas(), direto);
	i=0;
	while (cont<g->getQuantVertices()-1 && i<g->getQuantArestas()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			//A[cont++] = listaAresta[i];
			cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
			A[listaAresta[i]->getId()] = 1;
			//cout<<"AQUI2"<<endl;
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			//peso=listaAresta[i]->getPeso1()*(g->getYl()-g->getYll())+listaAresta[i]->getPeso2()*(g->getXll()-g->getXl());
			//custo+=peso;
		}
		i++;
	}
	conjunto.desaloca();
	
	if (cont==g->getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
}

void borderSearch(Grafo *g, list<int*> &resul, int * sl, int *sll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<int* >  pilha;
	bool avanca = true;
	float xl, yl, xll, yll;
	do{
		getXandY(s1, g->get_allArestas(), xl, yl);
		getXandY(s2, g->get_allArestas(), xll, yll);
		A2 = new int[g->getQuantArestas()];
		
		kruskal(g, A2, xl, yl, xll, yll, 3);
		
		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
			//s_new= new ElementArvore(A2, x, y);
			pilha.push(s2);
			s2 = A2;
			avanca = true;
			cout<<"aqui1"<<endl;
		} else {
			if (pilha.size()==0){ //se pilha está fazia
				avanca = false;
				cout<<"aqui2"<<endl;
			} else {
				cout<<"aqui3"<<endl;
				avanca = true;
				s1 = s2;
				resul.push_back(s2);
				s2 = pilha.top();
				pilha.pop();
			}
			//delete[] A2;
		}	
	} while (avanca);
}


/*
#### Primeira fase ####
# A funcao phase1GM retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list<int*> phase1GM(Grafo *g){
	list<int*> result;
	int *s1 = new int[g->getQuantArestas()];
	kruskal(g, s1, 0, 0, 0, 0, 1); // arvore para o primeiro objetivo
	result.push_back(s1);
	int* s2 = new int[g->getQuantArestas()];
	kruskal(g, s2, 0, 0, 0, 0, 2); // arvore para o segundo objetivo
	list<int*>::iterator it = result.end();
	if (isEgalObjetive(s1, s2,g->get_allArestas())==false){
		borderSearch(g, result, s1, s2);
		result.push_back(s2);
		//resul->insert2(s2);
	}
	return result;
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
	my_grafo.gerarArestasPtr();
	

	list<int*> arvores = phase1GM(&my_grafo);
	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    int i = 1;
    for (list<int*>::iterator it=arvores.begin(); it!=arvores.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<endl;
    	i++;
	}
	return 0;
}