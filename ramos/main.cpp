/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Ramos et al's (1998) algorithm 
# to resolve the Biobjective Spanning Tree Problem
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
using namespace std;

#define MAX1 20000 // TODO : Aumentar

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
		float cont; // nao utilisazado nesse caso
		kruskal(g, A2, xl, yl, xll, yll,cont, 3);
		
		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
			pilha.push(s2);
			s2 = A2;
			avanca = true;
		} else {
			if (pilha.size()==0){ //se pilha está fazia
				avanca = false;
			} else {
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
	float cont; // nao utilisazado nesse caso
	kruskal(g, s1, 0, 0, 0, 0,cont, 1); // arvore para o primeiro objetivo
	result.push_back(s1);
	int* s2 = new int[g->getQuantArestas()];
	kruskal(g, s2, 0, 0, 0, 0,cont, 2); // arvore para o segundo objetivo
	list<int*>::iterator it = result.end();
	if (isEgalObjetive(s1, s2,g->get_allArestas())==false){
		borderSearch(g, result, s1, s2);
		result.push_back(s2);
	}
	return result;
}
/* 
# retorna verdadeiro se o ponto kbest está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y){
	bool retorno = false; // por default, o ponto nao está na regiao viavel 
	//This check is done by a simple linear search through the list of the consecutive corner points defining the viable region.
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 
		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		//cout<<"kbest = ("<<x<<"'"<<y<<")"<<endl;
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			pair<float, float> ponto1 = make_pair(x, ponto_y);
			pair<float, float> ponto2 = make_pair(ponto_x, y);
			regiaoViavel.insert(it, ponto1);
			regiaoViavel.insert(it, ponto2);
			regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
			retorno = true;
			break; // se o ponto está regiao viavel, nao há mais rezao para percorrer-la 
		}
	}
	return retorno;
}

pair<float, float> getMaiorDistante(float a, float b, float c, list< pair<float, float> > regiaoViavel){
	pair<float, float>  pontoR = (*regiaoViavel.begin());
	float max = abs(a*pontoR.first + b*pontoR.second + c)/sqrt(a*a + b*b);
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it);
		float distancia = abs(a*ponto.first + b*ponto.second + c)/sqrt(a*a + b*b); // distância de ponto a r
 		if (distancia>max){
 			max = distancia;
 			pontoR = ponto;
 		}
	}
	return pontoR;
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
	cout<<"Fim da primeira fase ... "<<endl;
	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    
    
    int i = 1, cont=0;
    //cout<<"saiu2"<<endl;
    //cout<<resul.size()<<endl;
   // list<int*>::iterator it=resul.begin();
   cout<<"Resultado \n SUPORTADAS"<<endl;
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