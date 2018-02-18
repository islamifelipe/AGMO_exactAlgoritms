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


#define MAX_K_BEST 3000000 // este valor representa somente a k-ésima best árvore encontrada e NAO O TAMANHO DA HEAP

// ATENCAO: esta implementençao é para GRAFOS COMPLETOS

double custos[NUMOBJETIVOS][NUMEROVERTICES][NUMEROVERTICES];
// OBJETIVOS NUMVERTICES NUMVERTICES

vector<SolucaoEdgeSet *> suportadas;
vector<SolucaoEdgeSet *> noSuportadas;

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

bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}


bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
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
		for (int j = 0; j < NUMARESTAS; j++) { // GRAFOS COMPLETOS
			arestas[j].fit = arestas[j].c1*(suportadas[atual.a]->getObj(1)-suportadas[atual.b]->getObj(1)) +
					 arestas[j].c2*(suportadas[atual.b]->getObj(0)-suportadas[atual.a]->getObj(0));
		}
		// cout << "T4" << endl;
		stable_sort (arestas, arestas + NUMARESTAS, auxEdgeStructCompFit);
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
			P1.status[dest][orig] = 2; //
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
			P1.status[dest][orig] = 1;
		}
	}
}

SolucaoEdgeSet* AllSpaningTree(float xl, float yl, float xll, float yll,float a, float bM){

	SolucaoEdgeSet *particao = Heap.front();
	std::pop_heap (Heap.begin(),Heap.end(),compareHeap); Heap.pop_back();

	float x = particao->getObj(0);
	float y = particao->getObj(1);
	
	if (maiorIgualQuefloat(y,(a*x+bM))==false){ // Caso o ponto minimo passa da linha maxima, nao precisa calcular novamente		
		Partition(particao, xl, yl, xll, yll);
	}
	return particao;
}



// /* 
// # retorna verdadeiro se o ponto kbest está dentro da regiao viável
// # se verdeiro, atualiza a regiao viável
// */	
bool isInViableRegion(list< pair<float, float> > &regiaoViavel, float x, float y){
	bool retorno = false; // por default, o ponto nao está na regiao viavel 
	//This check is done by a simple linear search through the list of the consecutive corner points defining the viable region.
	//cout<<"\nkbest = ("<<x<<"'"<<y<<")"<<endl;
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 

		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		
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
	float max = ((float)abs(a*pontoR.first + b*pontoR.second + c))/((float) sqrt(a*a + b*b));
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it);
		float distancia = abs(a*ponto.first + b*ponto.second + c)/sqrt(a*a + b*b); // distância de ponto a r
 		if (maiorQuefloat(distancia,max)){
 			max = distancia;
 			pontoR = ponto;
 		}
	}
	return pontoR;
}



// /* 
// Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// Assim, teremos separadas as solucoes suportadas e nao suportadas
// */
void phase2KB(){
	vector<SolucaoEdgeSet *>::iterator it = suportadas.begin(); 
	int contador = 0;
	int size = suportadas.size();
	while (contador<size-1){
		SolucaoEdgeSet *pp = *(it);
		SolucaoEdgeSet *pq = *(++it);
		// int *p = pp.first; 
		// int *q = pq.first;
		float yp, yq, xp, xq;
		list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	
		xp = pp->getObj(0); //pp.second.first;
		yp = pp->getObj(1); //pp.second.second;
		xq = pq->getObj(0); //pq.second.first;
		yq = pq->getObj(1); //pq.second.second;

		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		
		// determina a reta p-q (hipotenusa)
		float a, b; // pra determinar a equacao da reta p-q na forma ax+b = y
		a = ((float)(yp-yq))/((float) (xp-xq)); // coeficiente angular da reta p-q
		b = yq - a*xq; // coeficiente linear de p-q
		pair<float, float> maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
		//Agora determinamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
		//cout<<"Mais distante : "<<maisDistante.first<<", "<<maisDistante.second<<endl;
		float bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y

		for (int j = 0; j < NUMARESTAS; j++) { // GRAFOS COMPLETOS
			arestas[j].fit = arestas[j].c1*(yp-yq) + arestas[j].c2*(xq-xp);
		}

		stable_sort (arestas, arestas + NUMARESTAS, auxEdgeStructCompFit);

		SolucaoEdgeSet *pii = new SolucaoEdgeSet(NUMEROVERTICES-1);
		bool res = lkr.executar_particao (*pii, arestas); //esta versao do kruskal considera que as arestas ja estao ordenadas
		if (res){
			float x = pii->getObj(0);
			float y = pii->getObj(1);
			pii->objetivoGeral = x*(yp-yq)+y*(xq-xp);
			Heap.push_back(pii);
			make_heap(Heap.begin(),Heap.end(), compareHeap);
		} else delete pii;

		for (int k = 1; k<MAX_K_BEST && Heap.size()!=0; k++){
			SolucaoEdgeSet *nova = AllSpaningTree(xp,yp, xq,yq,a, bM);
			float x = nova->getObj(0);
			float y = nova->getObj(1);
			if (isInViableRegion(regiaoViavel, x, y)){ // se estiver na regiao viavel
				SolucaoEdgeSet * nn = new SolucaoEdgeSet(NUMEROVERTICES-1);
				*nn = *nova;
				noSuportadas.push_back(nn);
				//Agora atualizamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
				bM = (float)maisDistante.second - (float)a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
			
			} else if ( maiorIgualQuefloat(y,(a*x+bM))) { //s on or past maximum cost line 
			//	cout<<"K break = "<<k<<endl;
				k = MAX_K_BEST+1; // sai;

			}
			delete nova;
		}
		//LIMPA A HEAP
		for (std::vector<SolucaoEdgeSet* >::iterator ittt = Heap.begin() ; ittt != Heap.end(); ++ittt){
			delete *ittt;
		}
		Heap.clear();
		//LIMPA A HEAP
		contador++;
	}
}

int main(){

	input(); // ler instância
	FirstPhase();
	stable_sort (suportadas.begin(), suportadas.end(),auxEdgeStructCompSolutions);
	phase2KB();
	cout<<"TOTAL DE SOLUCOES = "<<suportadas.size()+noSuportadas.size()<<endl;
	cout<<"SUPORTADAS: "<<endl;
	for (int i=0; i<suportadas.size(); i++){
		cout<<suportadas[i]->getObj(0)<<" "<<suportadas[i]->getObj(1)<<endl;
	}
	cout<<"\nNAO SUPORTADAS: "<<endl;
	for (int i=0; i<noSuportadas.size(); i++){
		cout<<noSuportadas[i]->getObj(0)<<" "<<noSuportadas[i]->getObj(1)<<endl;
	}


	// stable_sort (arestas, arestas + NUMARESTAS,auxEdgeStructLexCompObj1);

	// SolucaoEdgeSet *pii = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// lkr.executar (*pii, arestas); //esta versao do kruskal considera que as arestas ja estao ordenadas
	
	// pii->objetivoGeral = pii->getObj(0);
	// Heap.push_back(pii);
	// std::vector<SolucaoEdgeSet * > sollsk;
	// for (int i=0; i<MAX_K_BEST && Heap.size()>0; i++){ // OBS.: NO MAXIMO 3000000 de 
	// 	float xl  = 0;
	// 	float yl  = 1;
	// 	float xll = 0;
	// 	float yll = 0;
	// 	SolucaoEdgeSet *nova = AllSpaningTree(xl, yl, xll, yll);
	// 	cout<<i<<" --> "<<nova->getObj(0)<<" "<<nova->getObj(1)<<" size = "<<Heap.size()<<endl;
	// 	if (i>100 && i<5000){
	// 		SolucaoEdgeSet * nn = new SolucaoEdgeSet(NUMEROVERTICES-1);
	// 		*nn = *nova;
	// 		sollsk.push_back(nn);
	// 	}
	// 	delete nova;
	// }
	// int i = 0;
	// for (std::vector<SolucaoEdgeSet* >::iterator it = Heap.begin() ; it != Heap.end(); ++it){
	// 	cout<<"Delete = "<<i<<endl;
	// 	delete *it;
	// 	i++;
	// }
	// Heap.clear();
	// cout<<"SOLUCOES"<<endl;
	// for (int i=0; i<sollsk.size(); i++){
	// 	cout<<i<<" --> "<<sollsk[i]->getObj(0)<<" "<<sollsk[i]->getObj(1)<<endl;	
	// }

	// while (true);
	// for (int i=0; i<suportadas.size(); i++){
	// 	// suportadas[i]->printSolucao(stdout);
	// 	cout<<suportadas[i]->getObj(0)<<" "<<suportadas[i]->getObj(1)<<endl;
	// }
}