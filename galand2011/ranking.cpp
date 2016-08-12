/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Galand et al's (2010) algorithm 
# to resolve the Bi-objective Spanning Tree Problem
# This algorithm is a Choquet-based optimisation
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>      // std::pair
#include <climits>
#include "Grafo.h"
#include "lambda.h"
using namespace std;

// bool a_domina_b( Aresta *a, Aresta *b){
// 	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2() && (a->getPeso1() < b->getPeso1() || a->getPeso2() < b->getPeso2())){
// 		return true;
// 	} else return false;
// }

// bool a_domina_fracamente_b( Aresta *a, Aresta *b){
// 	if (a->getPeso1() <= b->getPeso1() && a->getPeso2() <= b->getPeso2()){
// 		return true;
// 	} else return false;
// }

// bool isEgal(int *t1, int *t2, int size){
// 	for (int i=0; i<size; i++){
// 		if (t1[i]!=t2[i]) return false;
// 	}
// 	return true;
// }

// bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){
// 	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
// 	for (int i=0; i<arestas.size(); i++){
// 		if (t1[i]==1){
// 			t1_peso1+=arestas[i]->getPeso1();
// 			t1_peso2+=arestas[i]->getPeso2();
// 		} 
// 		if (t2[i]==1){
// 			t2_peso1+=arestas[i]->getPeso1();
// 			t2_peso2+=arestas[i]->getPeso2();
// 		}
// 	}
// 	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
// 		return true;
// 	} else return false;
// }


int main(){
	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	cin>>m; // quantidade de arestas do grafo	
	Grafo my_grafo(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	for (id=0; id<m; id++){
		cin>>origem;
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
	}
	int i;
	float vi;
	float v[4] = {0,0,0,0};
	while (cin>>i){
		cin>>vi;
		v[i] = vi;
	}
	pair<float, float> lambda = algorithm1(v);

	cout<<lambda.first<<"   "<<lambda.second<<endl;
	return 0;
}