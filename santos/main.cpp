/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2018
#-----------------------------------------------------------------------
# This code implements the Santos and Pugliese's (2018) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The user time is returned;
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h> 
#include <list>
using namespace std;

struct tms tempsInit, tempsFinal; // para medir o tempo
Grafo my_grafo; // o grafo 
int n; // quantidade de vértices do grafo

typedef struct pp
{
	list<Aresta *> sequenciaDeArestas{};
	int peso1 = 0;
	int peso2 = 0;
	list<pair<int, int> >ultimaAresta;

}Path; // representa um caminho, simplesmente com a lista de arestas que o compoem a sequência

typedef struct ss
{
	list<Path* > listaCaminhos{}; // lista de caminhos
	bool *X; // lista de nos associados ao caminho (X \in N) = O mesmo X do Algoritmo 3 do artigo
	// vértices marcados (true) e nao marcados (false)
}Stipo;
// Vai ser criada uma lista de Stipo, com no maximo Z^{n-1}

std::vector<Stipo* > S; // o S do algoritmo 3 do artigo
list<Path* > U; //o  U do algoritmo 3 do artigo

map <int, Aresta *> allArestas;

bool *Xaux = new bool[n]; // um X auxiliar

bool equalSets(bool *X1, bool *X2){ // retorna true se os conjuntos X1 e X2 forem iguais
	for (int i=0; i<n; i++){ // X1 in X2
		if (X1[i]!=X2[i]) return false;
	} 
	return true;
}


// pair<int, int> getEp(std::vector<int> seq){
// 	if (seq.size()>1){
// 		int i = seq[seq.size()-2];
// 		int j = seq[seq.size()-1];
// 		return make_pair(i,j);
// 	} else {
// 		return make_pair(0,0);
// 	}
// }

// pair<int, int> getEp(Path *p){ // aquele e(p) do algoritmo 3
// 	if (p->sequenciaDeArestas.size()==0){
// 		return make_pair(0,0); // [1,1];
// 	} else {
// 		list<Aresta *>::iterator it = p->sequenciaDeArestas.end();
// 		Aresta *ultima = *it;
// 		it--;
// 		Aresta *antipenultima = *it;
// 		if (antipenultima->getDestino() == ultima->getOrigem() || antipenultima->getOrigem() == ultima->getOrigem()) {
// 			return make_pair(ultima->getOrigem(), ultima->getDestino());
// 		} else{ //if (antipenultima->getDestino() == ultima->getDestino() || antipenultima->getOrigem() == ultima->getDestino()){
// 			return make_pair(ultima->getDestino(), ultima->getOrigem());
// 		}
// 		// Aresta* a = p->sequenciaDeArestas.back();

		
// 	}
// }

// std::vector<int> getSequence(Path *p){
// 	std::vector<int> aux;
// 	list<Aresta *>::iterator it = p->sequenciaDeArestas.begin();
// 	aux.push_back(0);
// 	int ant = 0;
// 	while (it!=p->sequenciaDeArestas.end()){
// 		Aresta *a = *it;
// 		int dest = a->getDestino(); //getEp(Path p)
// 		if (ant != dest){
// 			aux.push_back(dest);
// 			ant = dest;
// 		}else {
// 			aux.push_back(a->getOrigem());
// 			ant = a->getOrigem();
// 		}
// 		it++;
// 	}
	
// 	return aux;

// }




vector<int> getSequence(Path *p){
	std::vector<int> aux;
	list<pair<int, int> >::iterator itt = p->ultimaAresta.begin();
	int ant = 0;
	// aux.push_back(ant);
	while (itt!=p->ultimaAresta.end()){
		if ((*itt).second!=ant){
			ant = (*itt).second;
		} else {
			ant = (*itt).first;
		}
		aux.push_back(ant);
		itt++;
	}
	return aux;

}


Aresta *getAresta(int origem, int destino){
	for (int i=0; i<allArestas.size(); i++){
		if ((allArestas[i]->getOrigem()==origem && allArestas[i]->getDestino()==destino) || (allArestas[i]->getOrigem()==destino && allArestas[i]->getDestino()==origem) ) {
			return allArestas[i];
		} 
	}
	return NULL;
}

Stipo * getSX(bool *Y){
	for (int i=0; i<S.size(); i++){
		if (equalSets(S[i]->X,Y)) {
			return S[i];
		}
	}
	Stipo *ret = new Stipo();
	ret->X = new bool[n];
	for (int i=0; i<n; i++) ret->X[i] = Y[i]; // cria um novo Stipo com copia de Y
	ret->listaCaminhos.clear();
	S.push_back(ret);
	return ret;
}

// retorna true se for dominado por alguém em SY. 
// retorna false se nao for dominado por ninguém
bool isDominado(Path *p, Stipo *SY){
	list<Path* > listaCaminhos = SY->listaCaminhos;
	list<Path* >::iterator it =listaCaminhos.begin() ;
	while (it!=listaCaminhos.end()){
		Path *pyy = (*it);
		if (pyy->peso1 <= p->peso1 && pyy->peso2 <= p->peso2 && (pyy->peso1 < p->peso1 || pyy->peso2 < p->peso2)){
			return true;
		}
		it++;
	}
	return false;
}

void removeFromU(Path *p){
	list<Path* >::iterator it = U.begin() ;
	list<list<Path* >::iterator> remover;
	while (it!=U.end()){
		if ((*it) == p) remover.push_back(it);
		// if ((*it)->peso1==p->peso1 && (*it)->peso2==p->peso2){
		// 	remover.push_back(it);
		// }
		it++;
	}
	list<list<Path* >::iterator>::iterator re = remover.begin();
	while (re!=remover.end()){
		// delete **re;
		U.erase(*re);
		re++;
	}

}

void removeDominadas(Path *p, Stipo *SY){
	list<Path* >::iterator it =SY->listaCaminhos.begin() ;
	list<list<Path* >::iterator> remover;
	while (it!=SY->listaCaminhos.end()){
		Path *pyy = (*it); // procura por alguém dominado por p
		if (p->peso1 <= pyy->peso1 && p->peso2 <= pyy->peso2  && (p->peso1 < pyy->peso1 || p->peso2 < pyy->peso2)){
			// remove pyy de SY
			remover.push_back(it);
		}
		it++;
	}
	list<list<Path* >::iterator>::iterator re = remover.begin();
	while (re!=remover.end()){
		// delete **re;
		removeFromU(**re);
		SY->listaCaminhos.erase(*re);
		re++;
	}

}

// cria uma copia de a
Path * copia(Path *a){
	Path *pl = new Path();
	pl->peso1 = a->peso1; 
	pl->peso2 = a->peso2;
	// pl->ultimaAresta = make_pair(a->ultimaAresta.first, a->ultimaAresta.second);
	list<Aresta *>::iterator it = a->sequenciaDeArestas.begin();
	while (it!=a->sequenciaDeArestas.end()){
		pl->sequenciaDeArestas.push_back((*it));
		it++;
	}
	list<pair<int, int> >::iterator itt = a->ultimaAresta.begin();
	while (itt!=a->ultimaAresta.end()){
		pl->ultimaAresta.push_back(*itt);
		itt++;
	}
	return pl;
}



void searchNewMinimalPaths(Path *p, int i, int j, bool *Y){
	for (int w = j+1; w<n; w++){ // j+1 mesmo?
		Aresta *a = getAresta(i, w);
		if (a!=NULL){ // Se NULL, entao nao existe aresta i,w	
			if (Y[w]==false){
				Y[w] = true; // a gente marca w em Y so para poder utilizar mais adiante. Depois desmarca, senao ele ficar marcado em Y fora desta sub-rotina
				Path *pl = copia(p);
				pl->sequenciaDeArestas.push_back(a); // adiciona aresta ao caminho NA ULTIMA POSICAO;
				pl->peso1 += a->getPeso1();
				pl->peso2 += a->getPeso2();
				pl->ultimaAresta.push_back(make_pair(i,w));
				Stipo *SY = getSX(Y);
				U.push_back(pl); // fila
				SY->listaCaminhos.push_back(pl);


				Y[w] = false;
			}
		}
	}	
}

void searchNewNDminimalPaths(Path *p, int i, int j, bool *Y){
	for (int w = j+1; w<n; w++){ // j+1 mesmo?
		if (Y[w]==false){
			Aresta *a = getAresta(i, w);
			if (a!=NULL){ // Se NULL, entao nao existe aresta i,w
				Y[w] = true; // a gente marca w em Y so para poder utilizar mais adiante. Depois desmarca, senao ele ficar marcado em Y fora desta sub-rotina
				Path *pl = copia(p);
				pl->sequenciaDeArestas.push_back(a); // adiciona aresta ao caminho NA ULTIMA POSICAO;
				pl->peso1 += a->getPeso1();
				pl->peso2 += a->getPeso2();
				pl->ultimaAresta.push_back(make_pair(i,w));
				Stipo *SY = getSX(Y);
				if (isDominado(pl,SY)==false){
					
  					removeDominadas(pl, SY);//remove de SY as dominadas por pl
  					U.push_back(pl); // fila
					SY->listaCaminhos.push_back(pl);

				} else delete  pl;
  				
				Y[w] = false;
			}
		}
	}	
}


void printResultado(){
	for (int i=0; i<n; i++) Xaux[i] = true;
	Stipo *SN = getSX(Xaux);
	int cont=1;
	cout<<"Quantidade de solucoes: "<<(SN->listaCaminhos).size()<<endl;
	cout<<"Resultado Final"<<endl;
	for (list<Path* >::iterator iptl=(SN->listaCaminhos).begin(); iptl!=(SN->listaCaminhos).end(); iptl++){
		list<Aresta *>::iterator iiiittt = (*iptl)->sequenciaDeArestas.begin();
		cout<<"Arvore "<<cont++<<endl;
		while (iiiittt!= (*iptl)->sequenciaDeArestas.end()){
			cout<<(*iiiittt)->getOrigem()<<" "<<(*iiiittt)->getDestino()<<" "<<(*iiiittt)->getPeso1()<<" "<<(*iiiittt)->getPeso2()<<endl;
			iiiittt++;	
		}
		cout<<"("<<(*iptl)->peso1<<","<<(*iptl)->peso2<<")"<<endl;
		cout<<endl;
	}	
}

void *tempo(void *nnnn){
        while (true){
	        times(&tempsFinal);   /* current time */ // clock final
	        clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	        float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);

                   
           if (sec>=10800){// se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
            // if(sec>30){

                cout<<"RESULTADO AO FIM DE 3H"<<endl;
                cout<<"TEMPO LIMITE ATINGIDO..."<<endl;

                exit(EXIT_FAILURE); // TALVEZ ELE DÊ UMA FALHA DE SEGMENTACAO AQUI; MAS ISSO NAO AFETA O POSSIVEL RESULTADO ANTES DAS 3H
            }
        }
}


int main(){
	
	times(&tempsInit);  // pega o tempo do clock inical

	// para medir o tempo em caso limite
	pthread_t thread_time; 
	pthread_attr_t attr;
	int nnnnnnnn=0;
	if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
        printf("Error to create the thread");
        exit(EXIT_FAILURE);
    }
    
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	my_grafo.setN(n);
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

	allArestas = my_grafo.get_allArestas();


	Stipo *S0 = new Stipo();
	S0->X = new bool[n];
	for (int i=0; i<n; i++) S0->X[i] = false; // inicialmente, todos falsos
	S0->X[0] = true; // X = {1} // inicialmente, apenas o primeiro vertice.
	


	Path *p = new Path(); // inicialmente, sem arestas;
	p->ultimaAresta.push_back(make_pair(0,0));
	p->peso1 = 0;
	p->peso2 = 0;
	S0->listaCaminhos.push_back(p);
	U.push_back(p); 
	S.push_back(S0);
	list<Path* >::iterator it = U.begin();
	// U funciona como uma FILA: primeiro sai quem primeiro entrou. Os elementos sao inseridos no fim da fila
	

	Xaux = new bool[n]; // um X auxiliar

	// Aresta *a01 = getAresta(0,2);
	// Aresta *a12 = getAresta(2,1);
	// Aresta *a13 = getAresta(1,3);
	// Aresta *a14 = getAresta(3,4);
	// p->sequenciaDeArestas.push_back(a01);
	// p->ultimaAresta.push_back(make_pair(0,2));
	// p->sequenciaDeArestas.push_back(a12);
	// p->ultimaAresta.push_back(make_pair(2,1));
	// p->sequenciaDeArestas.push_back(a13);
	// p->ultimaAresta.push_back(make_pair(1,3));
	// p->sequenciaDeArestas.push_back(a14);
	// p->ultimaAresta.push_back(make_pair(3,4));
	// std::vector<int> seq = getSequence(p);
	// for (int i=0; i<seq.size(); i++) cout<<"seq["<<i<<"] = "<<seq[i]<<endl;
	// 	pair<int, int> eij = p->ultimaAresta.back();;
	// 	cout<<eij.first<<" "<<eij.second<<endl;
	
	while (it!=U.end()){
		p = *it;
		for (int i=0; i<n; i++) Xaux[i] = false;
		std::vector<int> seq = getSequence(p);
		for (int i=0; i<seq.size(); i++) {
			// cout<<seq[i]<<" ";
			Xaux[seq[i]] = true;
		}
		// cout<<endl;
			
		pair<int, int> eij = p->ultimaAresta.back();//= getEp(	seq ); //[i,j]
	
		// cout<<"["<< eij.first<<" "<<eij.second<<"]"<<endl;
		int l; // o index de eij.first em seq
		for (int i=0; i<seq.size(); i++){
			if(seq[i]==eij.first){
				l = i;
				break;
			}
		}
		searchNewNDminimalPaths(p, eij.first, eij.second,Xaux);
		for (int x=l+1; x<seq.size(); x++){
			searchNewNDminimalPaths(p, seq[x], -1, Xaux); // acho que é -1, porque nossa faixa de indeces vai de 0 a n-1
		}

		it++; //última linha do while
	}


	times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	// cout<<user_time<<endl;
	cout<<"Tempo total: "<<(float) user_time / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	

	


    printResultado();



	return 0;
}