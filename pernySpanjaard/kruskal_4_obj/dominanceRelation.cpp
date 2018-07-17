/*
# Este programa é utilizado somente para gerar o grafo de relaçao binaria de dominância (oligarchic preference relations)
# que será fornecido como entrada ao Perny e Spanjaard (2003). Seu objetivo é somente evitar escrever a relaçao de prefência na mao ;-) 
# Caso o Perny e Spanjaard (2003) seja utilizado com outra relaçao de preferência 
# Nao há necessidade de se executar este programa (basta fornecer a relaçao desejada como entrada ao Perny e Spanjaard (2003)) 
# Entrada : instância do grafo G(V, E) conexo nao direcionado que será fornecido ao  Perny e Spanjaard (2003)
# Saida : grafo G'(V', E') direcionado (possivelmente desconexo) representando a relacao de dominância (e que deve também ser fornecido como entrada ao Perny e Spanjaard (2003)) 

existe (i->j) em E' <----> i domina j 

*/
#include <iostream>
#include <map> 
#include <string>
#include "Grafo.h"
using namespace std;

bool a_domina_fracamente_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && a->getPeso3() <= b->getPeso3() && a->getPeso4() <= b->getPeso4()){
		return true;
	} else return false;
}
bool a_domina_b( Aresta *a, Aresta *b){
	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && a->getPeso3() <= b->getPeso3() && a->getPeso4() <= b->getPeso4() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2() || a->getPeso3() < b->getPeso3() || a->getPeso4() < b->getPeso4())){
		return true;
	} else return false;
}

int main(){

	int n, m;
	float peso1, peso2, peso3, peso4;
	int origem, destino; // vértices para cada aresta;
	int id = 0; 
	cin>>n; // nao sera utilizao
	
	map <int, Aresta *> arestas; 
	
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		cin>>peso3;
		cin>>peso4;
		Aresta *nova = new Aresta(id, origem, destino, peso1, peso2, peso3, peso4);
		arestas[id] = nova;
		id++;

	}
	//fim de leitura
	cout<<n<<" "<<id<<endl;
	for (int i=0; i<arestas.size(); i++){
	cout<<arestas[i]->getOrigem()<<" "<<arestas[i]->getDestino()<<" "<<arestas[i]->getPeso1()<<" "<<arestas[i]->getPeso2()<<" "<<arestas[i]->getPeso3()<<" "<<arestas[i]->getPeso4()<<endl;
	}

	// a seguir construimos o vetor da relacao de dominância
	id = 0;
	for (int i=0; i<arestas.size(); i++){
		for (int j=0; j<arestas.size(); j++){
			if (i!=j){
				if (a_domina_b(arestas[i], arestas[j])){
					cout<<i<<" "<<j<<endl;
					//my_grafo.addAresta(id++, i, j, 0,0);
				}
			}
		}
	}
	return 0;
}