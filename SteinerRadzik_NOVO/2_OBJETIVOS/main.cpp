/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2018
#-----------------------------------------------------------------------
# This code implements the Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# This code utilises the SONRENSEN JANSSENS's (2003) algorithm
# to calculate the k-best tree
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>
#include <sys/times.h>
#include <unistd.h>
#include <queue>
#include <algorithm>
#include "SolucaoEdgeSet.cpp"
#include "param.h"
#include "SolucaoEdgeSet.cpp"
#include "LexKruskal.cpp"
using namespace std;


#define MAX_K_BEST 1000000 // este valor representa somenta a k-ésima best árvore encontrada e NAO O TAMANHO DA HEAP

// ATENCAO: esta implementençao é para GRAFOS COMPLETOS

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
// OBJETIVOS NUMVERTICES NUMVERTICES

vector<SolucaoEdgeSet *> suportadas;

struct twoint {
	int a;
	int b;
};

queue <struct twoint> fila;

auxEdgeStruct arestas [(NUMEROVERTICES*(NUMEROVERTICES-1))/2];
LexKruskal lkr;
int numIndGenerated = 0;
std::vector<SolucaoEdgeSet *> Heap;


void input(){
	int n;
	cin>>n; // quantidade de vertices
	int org, dest;
	for (int i=0;i<NUMEROVERTICES-1;i++) {
		for (int j=i+1;j<NUMEROVERTICES;j++) {
			cin>>org;
			cin>>dest;
			if (org!=i) cout<<"ERRO Leitura 1"<<endl;
			if (dest!=j) cout<<"ERRO Leitura 2"<<endl;
			for (int ob = 0; ob<2; ob++){
				cin>>custos[ob][i][j];
				custos[ob][j][i] = custos[ob][i][j]; // a parte superior da matriz é refletida na inferior
			}
		}
	}
	int arectr = 0;
	for (int i = 0; i < NUMEROVERTICES; i++) {
		for (int j = i+1; j < NUMEROVERTICES; j++) {
			arestas [arectr].a = i;
			arestas [arectr].b = j;
			arestas [arectr].c1 = custos [0][i][j];
			arestas [arectr].c2 = custos [1][i][j];
			arestas [arectr].fit = 0.0;
			arectr++;
		}
	}

}


inline bool auxEdgeStructLexCompObj1 (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.c1 < e2.c1) return true;
	else if (e1.c1 == e2.c1 && e1.c2 < e2.c2) return true;
	return false;
}

inline bool auxEdgeStructLexCompObj2 (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.c2 < e2.c2) return true;
	else if (e1.c2 == e2.c2 && e1.c1 < e2.c1) return true;
	return false;
}

inline bool auxEdgeStructCompFit (auxEdgeStruct e1, auxEdgeStruct e2) {

	if (e1.fit < e2.fit) return true;
	return false;
}
inline bool auxEdgeStructCompSolutions (SolucaoEdgeSet *a, SolucaoEdgeSet *b){
	return (a->getObj(0) < b->getObj(0) || (a->getObj(0) == b->getObj(0) && a->getObj(1) <= b->getObj(1)));
}

inline bool compareHeap (SolucaoEdgeSet *a, SolucaoEdgeSet *b){
	return a->objetivoGeral > b->objetivoGeral; // > mesmo
}



void borderSearch () {

	// cout << "Entrei em borderSearch" << endl;
	twoint atual;
	int i = numIndGenerated;
	// cout << "generated itens: " << endl;
	while (!fila.empty()) {
		atual.a = fila.front().a;
		atual.b = fila.front().b;
		fila.pop();
		// cout << "T3" << endl;
		for (int j = 0; j < (NUMEROVERTICES*(NUMEROVERTICES-1))/2; j++) { // GRAFOS COMPLETOS
			arestas[j].fit = arestas[j].c1*(suportadas[atual.a]->getObj(1)-suportadas[atual.b]->getObj(1)) +
					 arestas[j].c2*(suportadas[atual.b]->getObj(0)-suportadas[atual.a]->getObj(0));
		}
		// cout << "T4" << endl;
		stable_sort (arestas, arestas + (NUMEROVERTICES*(NUMEROVERTICES-1))/2, auxEdgeStructCompFit);
		// cout << "T5" << endl;
		SolucaoEdgeSet *pii = new SolucaoEdgeSet(NUMEROVERTICES-1);
		lkr.executar (*pii, arestas); //esta versao do kruskal considera que as arestas ja estao ordenadas
		// cout << "T6" << endl;
		// cout << i << endl;
		// cout<<suportadas[i]->getObj(0)<<" "<<suportadas[i]->getObj(1)<<endl;
		if (*pii == *suportadas[atual.a] || *pii == *suportadas[atual.b]);

		else {
			suportadas.push_back(pii);
			twoint prox;
			prox.a = atual.a;
			prox.b = i;
			fila.push (prox);
			twoint prox1;
			prox1.a = i;
			prox1.b = atual.b;
			fila.push (prox1);
			numIndGenerated += 1;
			i++;
		}
	}
	// while (!fila.empty())
	// 	fila.pop();

	// cout << "saindo de borderSearch" << endl;
}


int FirstPhase() {
	stable_sort (arestas, arestas + NUMARESTAS, auxEdgeStructLexCompObj1);
	SolucaoEdgeSet *p1 = new SolucaoEdgeSet(NUMEROVERTICES-1);
	lkr.executar (*p1, arestas);
	numIndGenerated = 1;
	SolucaoEdgeSet *p2 = new SolucaoEdgeSet(NUMEROVERTICES-1);
	stable_sort (arestas, arestas + NUMARESTAS, auxEdgeStructLexCompObj2);
	lkr.executar (*p2, arestas);
	if(!(*p1 == *p2)){
		suportadas.push_back(p1);
		suportadas.push_back(p2);
		numIndGenerated = 2;
		while (!fila.empty())
			fila.pop();
		twoint fstelement;
		fstelement.a = 0;
		fstelement.b = 1;
		fila.push(fstelement);
		borderSearch();	
	} else {
		suportadas.push_back(p1);
	}
	return numIndGenerated;

}

void Partition(SolucaoEdgeSet *particao, float xl, float yl, float xll, float yll){
	bool res = false;
	float custo;
	SolucaoEdgeSet P1 = *particao;

	// Heap sem limite
	for (int i=0; i<NUMEROVERTICES-1; i++){ // para cada aresta da soluçao corrente
		int orig = particao->edges[i][0];
		int dest = particao->edges[i][1];
		if (particao->status[orig][dest]==0){ // para cada aresta opcional
			P1.status[orig][dest] = 2; // proibida
			SolucaoEdgeSet *nova = new SolucaoEdgeSet(NUMEROVERTICES-1);
			*nova = P1; // copia também a partiçao (arestas proibidas e obrigatorias)
			res = lkr.executar_particao(*nova, arestas);
			if (res){
				custo = nova->getObj(0)*(yl-yll) + nova->getObj(1)*(xll-xl);
				nova->objetivoGeral = custo;
				Heap.push_back(nova);
				push_heap(Heap.begin(), Heap.end(), compareHeap);
				// insere na heap
			} else {
				delete nova;
			}
			P1.status[orig][dest] = 1;
		}
	}
}

SolucaoEdgeSet* AllSpaningTree(float xl, float yl, float xll, float yll){

	SolucaoEdgeSet *particao = Heap.front();
	std::pop_heap (Heap.begin(),Heap.end(),compareHeap); Heap.pop_back();

	Partition(particao, xl, yl, xll, yll);

	return particao;
}

int main(){

	input(); // ler instância
	// FirstPhase();
	// stable_sort (suportadas.begin(), suportadas.end(),auxEdgeStructCompSolutions);
	
	stable_sort (arestas, arestas + NUMARESTAS,auxEdgeStructLexCompObj1);

	SolucaoEdgeSet *pii = new SolucaoEdgeSet(NUMEROVERTICES-1);
	lkr.executar (*pii, arestas); //esta versao do kruskal considera que as arestas ja estao ordenadas
	
	pii->objetivoGeral = pii->getObj(0);
	Heap.push_back(pii);
	
	for (int i=0; i<100 && Heap.size()>0; i++){
		float xl  = 0;
		float yl  = 1;
		float xll = 0;
		float yll = 0;
		SolucaoEdgeSet *nova = AllSpaningTree(xl, yl, xll, yll);
		cout<<nova->getObj(0)<<" "<<nova->getObj(1)<<endl;
	}
	// for (int i=0; i<suportadas.size(); i++){
	// 	// suportadas[i]->printSolucao(stdout);
	// 	cout<<suportadas[i]->getObj(0)<<" "<<suportadas[i]->getObj(1)<<endl;
	// }
}