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

vector< pair<float, float> > min_f_g_vector;

bool isEgalObjetive(float xl, float yl,float xll, float yll){
	float t1_peso1=xl, t1_peso2=yl, t2_peso1=xll, t2_peso2=yll;
	
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);

}
bool t1_domina_t2(float xl, float yl,float xll, float yll){
	float t1_peso1=xl, t1_peso2=yl, t2_peso1=xll, t2_peso2=yll;
	
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
		return true;
	} else return false;
}

// void getXandY(int *t, map <int, Aresta *> arestas, float &X, float &Y ){
// 	X = 0; Y = 0;
// 	for (int i=0; i<arestas.size(); i++){
// 		if (t[i]==1){
// 			X+=arestas[i]->getPeso1();
// 			Y+=arestas[i]->getPeso2();
// 		} 
// 	}
// 	contAA++;
// }


void UniobjectiveSearch(Grafo *g, list<pair<int*, pair<float, float> > > &resul, int * sl, pair<float, float> psl, int *sll, pair<float, float> psll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<int,list<pair<int*, pair<float, float> > >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	stack<pair<float, float> > pilhaF; // f de uma solucao, f da outra
	stack<pair<float, float> > pilhaG; // idem
	float xl, yl, xll, yll;

	pilha.push(make_pair(s1, s2));
	pilhaIt.push(make_pair(2, resul.begin()));
	pilhaF.push(make_pair(psl.first, psll.first));
	pilhaG.push(make_pair(psl.second, psll.second));


	while (pilha.size()!=0){
		//cout<<"size = "<<pilha.size()<<endl;
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list< pair<int*, pair<float, float> > >::iterator> it = pilhaIt.top();
		pilhaIt.pop();

		pair<float, float> f = pilhaF.top();
		xl = f.first;
		xll = f.second;
		pair<float, float> gg = pilhaG.top();
		yl = gg.first;
		yll = gg.second;

		pilhaF.pop();
		pilhaG.pop();


		//getXandY(s1, g->get_allArestas(), xl, yl);
		//getXandY(s2, g->get_allArestas(), xll, yll);
		A2 = new int[g->getQuantArestas()];
		for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
		float cont; // nao utilisazado nesse caso
		float x, y;
		kruskal(g, A2, xl, yl, xll, yll,cont,x,y, 3);
		
		if( !( (isEgalObjetive(x,y,xl,yl)) || (isEgalObjetive(x,y, xll,yll)) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, make_pair(A2, make_pair(x,y))); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, make_pair(A2, make_pair(x,y)));
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push(make_pair(A2, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilha.push(make_pair(s1, A2));  // L'
			pilhaIt.push(make_pair(1,it.second)); 

			pilhaF.push(make_pair(x, xll));
			pilhaG.push(make_pair(y, yll));

			pilhaF.push(make_pair(xl, x));
			pilhaG.push(make_pair(yl, y));
			
		} else delete[] A2; // NOVO
	}
}


/*
#### Primeira fase ####
# A funcao E􏳇cffientBiobjectiveSTinEB retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list<pair<int*, pair<float, float> > > efficientBiobjectiveSTinEB(Grafo *g){
	list<pair<int*, pair<float, float> >  > result;
	int *s1 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) s1[i] = 0;
	float cont; // nao utilisazado nesse caso
	float xl, yl;
	kruskal(g, s1, 0, 0, 0, 0,cont, xl, yl,  1); // arvore para o primeiro objetivo
	result.push_back(make_pair(s1, make_pair(xl, yl)));
	int* s2 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) s2[i] = 0;
	float xll, yll;
	kruskal(g, s2, 0, 0, 0, 0,cont, xll, yll, 2); // arvore para o segundo objetivo
	list<pair<int*, pair<float, float> > >::iterator it = result.end();
	if (isEgalObjetive(xl,yl,xll, yll)==false){
		UniobjectiveSearch(g, result, s1, make_pair(xl, yl), s2, make_pair(xll, yll));
		result.push_back(make_pair(s2, make_pair(xll, yll)));
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

pair<float, float> min_f_g_construcao(vector<Aresta*> adjacentes){ // retorna f barra e g barra {f, g}

	float f = adjacentes[0]->getPeso1(), g = adjacentes[0]->getPeso2();
	for (int i=1; i<adjacentes.size(); i++){
		if (adjacentes[i]->getPeso1()<f) f = adjacentes[i]->getPeso1();
		if (adjacentes[i]->getPeso2()<g) g = adjacentes[i]->getPeso2();
		
	}
	pair<float, float> result = make_pair(f, g); //{x,y} <--> {f, g} 
	return result;
}


void retiraDominadas(list<pair<int*, pair<float, float> > > &noSuportadas, pair<int*, pair<float, float> > T){
	
	for (std::list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it != noSuportadas.end(); ++it){
		if (t1_domina_t2(T.second.first, T.second.second, (*it).second.first, (*it).second.second)){
			int *qux =  (*it).first;
			it = noSuportadas.erase(it);
			delete[] qux;  
			it--;
		} else if (t1_domina_t2((*it).second.first, (*it).second.second, T.second.first, T.second.second)){
			noSuportadas.remove(T);
			delete[] T.first;
			return;
		}
	}
}

void EBST_BrB(int *T, float fBound, float gBound, int step, Grafo *g, list< pair<int*, pair<float, float> > > &noSuportadas, Conjunto conjunto, list< pair<float, float> > &regiaoViavel){
	
	stack<Conjunto> pilhaCojunto; // cojuntos
	stack<pair<float, float> > pilhaLimites; // fBound, gBound
	stack<pair<int *, int> > pilha; // solucao parcial , step
	stack<pair<float, float> >pilhaFG; // f, g da solucao do topo da pilha
	
	pilhaCojunto.push(conjunto);
	pilhaLimites.push(make_pair(fBound, gBound));
	pilha.push(make_pair(T, step));
	pilhaFG.push(make_pair(0,0));

	float tf, tg;
	pair<int *, int> p1;
	pair<float, float> fg;
	pair<float, float> p2;
	list< pair<float, float> >::iterator it;
	pair<float, float> ponto, ponto1,ponto2,fg_step1; 
	pair<int*, pair<float, float> > novo;
	float novo_tf,novo_tg,ponto_x,ponto_y;
	int *T_aux ;
	Aresta *e;
	vector<Aresta*> Astep;
	while (pilha.size()!=0){
		p1 = pilha.top();
		T = p1.first;
		step = p1.second;
		pilha.pop();

		fg = pilhaFG.top();
		tf = fg.first;
		tg = fg.second; 
		pilhaFG.pop();

		p2 = pilhaLimites.top();
		fBound = p2.first;
		gBound = p2.second;
		pilhaLimites.pop();

		conjunto = pilhaCojunto.top();
		pilhaCojunto.pop();

		 Astep = g->getVertice(step)->getAdjacentes();

		//cout<<fBound<<", "<<gBound<<endl;
		//cout<<"regiaoViavel = "<<regiaoViavel.size()<<endl;
		//cout<<"pilha = "<<pilha.size()<<endl;
		for (int a=0; a<Astep.size(); a++){
			e = Astep[a];
			//cout<<T[e->getId()]<<"   "<< conjunto.compare(e->getOrigem(), e->getDestino())<<endl;
			if (!conjunto.compare(e->getOrigem(), e->getDestino())){ // se nao formar ciclo

				//getXandY(T, g->get_allArestas(), tf, tg);
				//list< pair<float, float> >::iterator it;
				//cout<<isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)<<endl;
				if (isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)){ // se estiver na regiao viavel
					T_aux = new int[g->getQuantArestas()];
					for (int i=0; i<g->getQuantArestas(); i++) T_aux[i] = T[i]; // copia
					Conjunto copie(g->getQuantVertices());
					copie.copia(conjunto);
					T_aux[e->getId()] = 1;
					novo_tf = tf+e->getPeso1();
					novo_tg = tg+e->getPeso2();
					copie.union1(e->getOrigem(), e->getDestino());
					if (step+1 == g->getQuantVertices()-1){
						novo = make_pair(T_aux,make_pair(novo_tf,novo_tg));
						noSuportadas.push_back(novo); // armazena T
						retiraDominadas(noSuportadas, novo); //remove any solutions dominated by T
						
						// atualiza a regiao viavel
						ponto = (*it); // um ponto extremo que delimita a regiao viável
						 ponto_x = ponto.first, ponto_y = ponto.second;
						ponto1 = make_pair(tf+e->getPeso1(), ponto_y);
						ponto2 = make_pair(ponto_x, tg+e->getPeso2());
						regiaoViavel.insert(it, ponto1);
						regiaoViavel.insert(it, ponto2);
						regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
							
					} else {
						 fg_step1 = min_f_g_vector[step+1];//min_f_g(g->getVertice(step+1)->getAdjacentes());
						pilhaCojunto.push(copie);
						pilhaLimites.push(make_pair(fBound-fg_step1.first,gBound-fg_step1.second));
						pilha.push(make_pair(T_aux, step+1));
						pilhaFG.push(make_pair(novo_tf, novo_tg));
					}
				}
			}
		}
		delete[] T;
		conjunto.desaloca();
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


list<pair<int*, pair<float, float> > > efficientBiobjectiveSTinENB(Grafo *g, list<pair<int*, pair<float, float> > > extremas){
	float fBound =0, gBound=0;
	for (int i=1; i<g->getQuantVertices()-1; i++){ 
	// o algoritmo diz que o somatorio deve ser de 2 até n-1. Como nossa numeracao começa do 0 até n-1, nos vamos do 1 até n-2
		pair<float, float> fg = min_f_g_vector[i]; //min_f_g(g->getVertice(i)->getAdjacentes());
		fBound+= fg.first;
		gBound+= fg.second;
	}
	int *T = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) T[i] = 0;
	list<pair<int*, pair<float, float> > > noSuportadas;
	Conjunto conjunto(g->getQuantVertices());
	// Calcular a regiao viavel inicial (delimitada somente pelos pontos suportados)
	list< pair<float, float> > regiaoViavel;
	list<pair<int*, pair<float, float> > >::iterator it = extremas.begin();
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		float yp, yq, xp, xq;
		int *p = (*it).first; 
		xp = (*it).second.first;
		yp = (*it).second.second;
		int *q = (*(++it)).first;
		xq = (*it).second.first;
		yq = (*it).second.second;
		//getXandY(p, g->get_allArestas(),xp, yp); // p
	 	//getXandY(q, g->get_allArestas(), xq, yq);	 //q
	 	//cout<<"("<<xq<<","<< yp<<")"<<endl;
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		contador++;
	}

	EBST_BrB(T, fBound,gBound,0,g, noSuportadas,conjunto, regiaoViavel);
	
	return noSuportadas;

}


int main(){
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	
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
	
	for (int i=0; i<my_grafo.getQuantVertices(); i++){
		min_f_g_vector.push_back(min_f_g_construcao(my_grafo.getVertice(i)->getAdjacentes()));
	}

	times(&tempsInit);  // pega o tempo do clock inical
	
	list<pair<int*, pair<float, float> > > arvores = efficientBiobjectiveSTinEB(&my_grafo);
	cout<<"Fim da primeira fase ... "<<endl;
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);

	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    list<pair<int*, pair<float, float> > > noSuportadas = efficientBiobjectiveSTinENB(&my_grafo, arvores);
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
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it).first[a] == 1){
				cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}

	cout<<"Nao Suportada"<<endl;

	for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it).first[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}
	return 0;
}
