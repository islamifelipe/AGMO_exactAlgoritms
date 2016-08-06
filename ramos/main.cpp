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
# retorna verdadeiro se o ponto (x,y) está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y, list< pair<float, float> >::iterator &it){
	bool retorno = false; // por default, o ponto (x,y) nao está na regiao viavel 
	
	for ( it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o x deve ser menor que o ponto_x   E   o y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o (x,y) nao está na regiao viavel*/ 
		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		//cout<<"kbest = ("<<x<<"'"<<y<<")"<<endl;
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			retorno = true;
			break; // se o ponto está regiao viavel, nao há mais rezao para percorrer-la 
		}
	}
	return retorno;
}


pair<float, float> min_f_g(vector<Aresta*> adjacentes){ // retorna f barra e g barra {f, g}
	float f = adjacentes[0]->getPeso1(), g = adjacentes[0]->getPeso2();
	for (int i=1; i<adjacentes.size(); i++){
		if (adjacentes[i]->getPeso1()<f) f = adjacentes[i]->getPeso1();
		if (adjacentes[i]->getPeso2()<g) g = adjacentes[i]->getPeso2();
		
	}
	pair<float, float> result = make_pair(f, g); //{x,y} <--> {f, g} 
	return result;
}
/* O parâmetro conjunto serve para detectar a formaçao de possíveis ciclo em T
cada vertice de T, deve também pertencer ao conjunto.
OBS.: na volta das chamadas recursivas, o conjunto deve readequirir seu estado anterior
ou seja, fora da chamada da funcao, o conjunto deve permanecer inalterado 
*/
void EBST_BrB(int *T, float fBound, float gBound, int step, Grafo *g, list<int*> &noSuportadas, Conjunto conjunto, list< pair<float, float> > &regiaoViavel){
	vector<Aresta*> Astep = g->getVertice(step)->getAdjacentes();

	for (int a=0; a<Astep.size(); a++){
		Aresta *e = Astep[a];
		//cout<<T[e->getId()]<<"   "<< conjunto.compare(e->getOrigem(), e->getDestino())<<endl;
		if (T[e->getId()]!=1 && !conjunto.compare(e->getOrigem(), e->getDestino())){ // se nao formar ciclo
			float tf, tg;
			getXandY(T, g->get_allArestas(), tf, tg);
			list< pair<float, float> >::iterator it;
			//cout<<isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)<<endl;
			if (isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)){ // se estiver na regiao viavel
				int *T_aux = new int[g->getQuantArestas()];
				for (int i=0; i<g->getQuantArestas(); i++) T_aux[i] = T[i]; // copia
				Conjunto copie(g->getQuantVertices());
				copie = conjunto;
				T_aux[e->getId()] = 1;
				copie.union1(e->getOrigem(), e->getDestino());
				cout<<step<<endl;
				if (step+1 == g->getQuantVertices()-1){
					noSuportadas.push_back(T_aux); // armazena T
					// TODO :  remove any solutions dominated by T
					
					// atualiza a regiao viavel
					pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
					float ponto_x = ponto.first, ponto_y = ponto.second;
					pair<float, float> ponto1 = make_pair(tf+e->getPeso1(), ponto_y);
					pair<float, float> ponto2 = make_pair(ponto_x, tg+e->getPeso2());
					regiaoViavel.insert(it, ponto1);
					regiaoViavel.insert(it, ponto2);
					regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
						
				} else {
					pair<float, float> fg_step1 = min_f_g(g->getVertice(step+1)->getAdjacentes());
					EBST_BrB(T_aux, fBound-fg_step1.first, gBound-fg_step1.second, step+1, g, noSuportadas, copie, regiaoViavel);
				}
				
			}
		}
	}
}  


list<int*> efficientBiobjectiveSTinENB(Grafo *g, list<int*> extremas){
	float fBound =0, gBound=0;
	for (int i=1; i<g->getQuantVertices()-1; i++){ 
	// o algoritmo diz que o somatorio deve ser de 2 até n-1. Como nossa numeracao começa do 0 até n-1, nos vamos do 1 até n-2
		pair<float, float> fg = min_f_g(g->getVertice(i)->getAdjacentes());
		fBound+= fg.first;
		gBound+= fg.second;
	}
	int *T = new int[g->getQuantArestas()];
	list<int*> noSuportadas;
	Conjunto conjunto(g->getQuantVertices());
	// Calcular a regiao viavel inicial (delimitada somente pelos pontos suportados)
	list< pair<float, float> > regiaoViavel;
	list<int*>::iterator it = extremas.begin();
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		int *p = *(it); 
		int *q = *(++it);
		float yp, yq, xp, xq;
		getXandY(p, g->get_allArestas(),xp, yp); // p
	 	getXandY(q, g->get_allArestas(), xq, yq);	 //q
	 	//cout<<"("<<xq<<","<< yp<<")"<<endl;
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		contador++;
	}

	EBST_BrB(T, fBound,gBound,0,g, noSuportadas,conjunto, regiaoViavel);
	
	return noSuportadas;

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
    list<int*> noSuportadas = efficientBiobjectiveSTinENB(&my_grafo, arvores);
    cout<<"Fim da segunda fase ... "<<endl;
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

	cout<<"\nNAO SUPORTADAS"<<endl;
    for (list<int*>::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
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