/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Ramos et al's (1998) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The user time is returned;
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include <sys/times.h>
#include <unistd.h>
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


void UniobjectiveSearch(Grafo *g, list<int*> &resul, int * sl, int *sll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<int,list<int*>::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	float xl, yl, xll, yll;

	pilha.push(make_pair(s1, s2));
	pilhaIt.push(make_pair(2, resul.begin()));


	while (pilha.size()!=0){
		//cout<<"size = "<<pilha.size()<<endl;
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list<int*>::iterator> it = pilhaIt.top();
		pilhaIt.pop();

		getXandY(s1, g->get_allArestas(), xl, yl);
		getXandY(s2, g->get_allArestas(), xll, yll);
		A2 = new int[g->getQuantArestas()];
		for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
		float cont; // nao utilisazado nesse caso
		kruskal(g, A2, xl, yl, xll, yll,cont, 3);
		//cout<<pilha.size()<<endl;

		// if (it.first == 1){ // antes
		// 		resul.insert(it.second, A2); 
		// 		it.second--;// it agora aponta para o item  A2
		// 	} else if (it.first == 2) { // depois
		// 		it.second++;
		// 		resul.insert(it.second, A2);
		// 		it.second--;// it agora aponta para o item  A2
		// 	}

		// if( !( isEgalObjetive(A2, s2, g->get_allArestas()))) {
		// 		pilha.push(make_pair(A2, s2)); // L''
		// 	pilhaIt.push(make_pair(2,it.second)); 
		// }
		// if( !( isEgalObjetive(A2, s1, g->get_allArestas()))) {
		// 		pilha.push(make_pair(s1, A2));  // L'
		// 	pilhaIt.push(make_pair(1,it.second)); 
		// }
		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, A2); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, A2);
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push(make_pair(A2, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilha.push(make_pair(s1, A2));  // L'
			pilhaIt.push(make_pair(1,it.second)); 
			
		}
	}
}


/*
#### Primeira fase ####
# A funcao E􏳇cffientBiobjectiveSTinEB retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list<int*> efficientBiobjectiveSTinEB(Grafo *g){
	list<int*> result;
	int *s1 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) s1[i] = 0;
	float cont; // nao utilisazado nesse caso
	kruskal(g, s1, 0, 0, 0, 0,cont, 1); // arvore para o primeiro objetivo
	result.push_back(s1);
	int* s2 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) s2[i] = 0;
	kruskal(g, s2, 0, 0, 0, 0,cont, 2); // arvore para o segundo objetivo
	list<int*>::iterator it = result.end();
	if (isEgalObjetive(s1, s2,g->get_allArestas())==false){
		UniobjectiveSearch(g, result, s1, s2);
		result.push_back(s2);
	}
	return result;
}
/* 
# retorna verdadeiro se o ponto (x,y) está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y, list< pair<float, float> >::iterator &it){
	
	for ( it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o x deve ser menor que o ponto_x   E   o y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o (x,y) nao está na regiao viavel*/ 
		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		//cout<<"kbest = ("<<x<<"'"<<y<<")"<<endl;
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			return true;
		}
	}
	return false;
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

void retiraDominadas(list<int*> &noSuportadas, int* T, Grafo *g){
	
	for (std::list<int*>::iterator it=noSuportadas.begin(); it != noSuportadas.end(); ++it){
		if (t1_domina_t2(T, *it, g->get_allArestas())){
			it = noSuportadas.erase(it);
			it--;
		} else if (t1_domina_t2(*it, T, g->get_allArestas())){
			noSuportadas.remove(T);
			return;
		}
	}
}

void EBST_BrB(int *T, float fBound, float gBound, int step, Grafo *g, list<int*> &noSuportadas, Conjunto conjunto, list< pair<float, float> > &regiaoViavel){
	
	stack<Conjunto> pilhaCojunto; // cojuntos
	stack<pair<float, float> > pilhaLimites; // fBound, gBound
	stack<pair<int *, int> > pilha; // solucao parcial , step

	pilhaCojunto.push(conjunto);
	pilhaLimites.push(make_pair(fBound, gBound));
	pilha.push(make_pair(T, step));

	while (pilha.size()!=0){
		pair<int *, int> p1 = pilha.top();
		T = p1.first;
		step = p1.second;
		pilha.pop();

		pair<float, float> p2 = pilhaLimites.top();
		fBound = p2.first;
		gBound = p2.second;
		pilhaLimites.pop();

		conjunto = pilhaCojunto.top();
		pilhaCojunto.pop();

		vector<Aresta*> Astep = g->getVertice(step)->getAdjacentes();

		//cout<<fBound<<", "<<gBound<<endl;
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
					if (step+1 == g->getQuantVertices()-1){
						noSuportadas.push_back(T_aux); // armazena T
						retiraDominadas(noSuportadas, T_aux, g); //remove any solutions dominated by T
						
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
						pilhaCojunto.push(copie);
						pilhaLimites.push(make_pair(fBound-fg_step1.first,gBound-fg_step1.second));
						pilha.push(make_pair(T_aux, step+1));
						//EBST_BrB(T_aux, fBound-fg_step1.first, gBound-fg_step1.second, step+1, g, noSuportadas, copie, regiaoViavel);
		

					}
				}
			}
		}
	}
}

// /* O parâmetro conjunto serve para detectar a formaçao de possíveis ciclo em T
// cada vertice de T, deve também pertencer ao conjunto.
// OBS.: na volta das chamadas recursivas, o conjunto deve readequirir seu estado anterior
// ou seja, fora da chamada da funcao, o conjunto deve permanecer inalterado 
// */
// void EBST_BrB(int *T, float fBound, float gBound, int step, Grafo *g, list<int*> &noSuportadas, Conjunto conjunto, list< pair<float, float> > &regiaoViavel){
// 	vector<Aresta*> Astep = g->getVertice(step)->getAdjacentes();
// 	//cout<<fBound<<", "<<gBound<<endl;
// 	for (int a=0; a<Astep.size(); a++){
// 		Aresta *e = Astep[a];
// 		//cout<<T[e->getId()]<<"   "<< conjunto.compare(e->getOrigem(), e->getDestino())<<endl;
// 		if (T[e->getId()]!=1 && !conjunto.compare(e->getOrigem(), e->getDestino())){ // se nao formar ciclo
// 			float tf, tg;
// 			getXandY(T, g->get_allArestas(), tf, tg);
// 			list< pair<float, float> >::iterator it;
// 			//cout<<isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)<<endl;
// 			if (isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)){ // se estiver na regiao viavel
// 				int *T_aux = new int[g->getQuantArestas()];
// 				for (int i=0; i<g->getQuantArestas(); i++) T_aux[i] = T[i]; // copia
// 				Conjunto copie(g->getQuantVertices());
// 				copie = conjunto;
// 				T_aux[e->getId()] = 1;
// 				copie.union1(e->getOrigem(), e->getDestino());
// 				if (step+1 == g->getQuantVertices()-1){
// 					noSuportadas.push_back(T_aux); // armazena T
// 					retiraDominadas(noSuportadas, T_aux, g); //remove any solutions dominated by T
					
// 					// atualiza a regiao viavel
// 					pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
// 					float ponto_x = ponto.first, ponto_y = ponto.second;
// 					pair<float, float> ponto1 = make_pair(tf+e->getPeso1(), ponto_y);
// 					pair<float, float> ponto2 = make_pair(ponto_x, tg+e->getPeso2());
// 					regiaoViavel.insert(it, ponto1);
// 					regiaoViavel.insert(it, ponto2);
// 					regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
						
// 				} else {
// 					pair<float, float> fg_step1 = min_f_g(g->getVertice(step+1)->getAdjacentes());
// 					EBST_BrB(T_aux, fBound-fg_step1.first, gBound-fg_step1.second, step+1, g, noSuportadas, copie, regiaoViavel);
// 				}
				
// 			}
// 		}
// 	}
// }  


list<int*> efficientBiobjectiveSTinENB(Grafo *g, list<int*> extremas){
	float fBound =0, gBound=0;
	for (int i=1; i<g->getQuantVertices()-1; i++){ 
	// o algoritmo diz que o somatorio deve ser de 2 até n-1. Como nossa numeracao começa do 0 até n-1, nos vamos do 1 até n-2
		pair<float, float> fg = min_f_g(g->getVertice(i)->getAdjacentes());
		fBound+= fg.first;
		gBound+= fg.second;
	}
	int *T = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) T[i] = 0;
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
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	times(&tempsInit);  // pega o tempo do clock inical
	
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
	

	list<int*> arvores = efficientBiobjectiveSTinEB(&my_grafo);
	cout<<"Fim da primeira fase ... "<<endl;
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);

	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    list<int*> noSuportadas = efficientBiobjectiveSTinENB(&my_grafo, arvores);
    cout<<"Fim da segunda fase ... "<<endl;
    times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	cout<<user_time2<<endl;
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total ..."<<endl;
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;


    int i = 1, cont=0;
    //cout<<"saiu2"<<endl;
    //cout<<resul.size()<<endl;
   // list<int*>::iterator it=resul.begin();
   cout<<"Resultado \n SUPORTADAS"<<endl;
    for (list<int*>::iterator it=arvores.begin(); it!=arvores.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
    	i++;
	}

	cout<<"Nao Suportada"<<endl;

	for (list<int*>::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		cout<<"Arvore "<<i<<endl;
		float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
				
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
    	i++;
	}
	return 0;
}