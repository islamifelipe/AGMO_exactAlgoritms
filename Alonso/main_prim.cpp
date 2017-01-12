/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Alonso et al's (2005) algorithm (optimalcutset_P)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)

#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include <stack>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

struct tms tempsInit, tempsFinal; // para medir o tempo
Grafo my_grafo;
vector<pair <int *, int*> > arvores;

vector <Aresta *> maximal(int* S, Grafo *my_grafo,vector<pair<int, int> > relacao2){
	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	//int *retorno = new int[arestas.size()]; // vetor dos id das arestas
	
	// for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
	// 	Aresta *a = arestas[i];
	// 	if (S[a->getOrigem()] != S[a->getDestino()]){
	// 		retorno[a->getId()] = 1; 
	// 	//	cout<<"("<<a->getOrigem()+1<<", "<<a->getDestino()+1<<")"<<endl;
	// 	} else {
	// 		retorno[a->getId()] = 0; 
	// 	}
	// }
	vector <Aresta *> ret;
	for (int i = 0; i<arestas.size(); i++){ 
		int v = arestas[i]->getId();
		bool dominada = false;
		//if (retorno[v] == 1){ // verifia se v é dominada
		if (S[arestas[i]->getOrigem()] != S[arestas[i]->getDestino()]){
			for (int j=0; j<relacao2.size(); j++){ 
				if (v==relacao2[j].second){ // se existe alguém que domina v
					//if (retorno[relacao2[j].first]==1) dominada=true;
					if (S[arestas[relacao2[j].first]->getOrigem()] != S[arestas[relacao2[j].first]->getDestino()]) dominada=true;
				}
			}
			if (dominada==false){
				ret.push_back(arestas[i]);
			}
		}
	}
	return ret;
}


void optimalcutset_P(Grafo *g,vector< pair<int, int> > relacao2){
	//vector<pair <int *, int*> > arvores;
	int *index = new int[g->getQuantArestas()]; //index é um vetor, onde cada indice representa o id da aresta, e o valor representa o level
	
	stack <pair <int*, int*> > pilha; // para simular a recursao. Primeiro elemento é o vetor de vertices (id's) e o segundo de arestas
	stack <int> sizeT; // usada pra guardar o tamanho de T (QUANTIDADE DE ARESTAS INSERIDAS)
	stack <int> pilha_level; // para guardar o level da recursao 

	int * initV = new int[g->getQuantVertices()];
	for (int i=0; i<g->getQuantVertices(); i++)initV[i] =0;
	
	int * initT = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) {
		initT[i] =0; // tudo vazio
		index[i] = 0; 
	}	
	initV[0] = 1; // vertice inicial v1;

	pilha.push(make_pair(initV, initT));
	pilha_level.push(0); // inicialmente, o level é zero
	sizeT.push(0);
	while (pilha.size()!=0){
		//cout<<"Size = "<<pilha.size()<<endl;
		pair <int*, int*> s = pilha.top();
		int * S = s.first; // vetor de vértices
		int * T = s.second; // vetor de arestas (g->getQuantArestas)
		int level = pilha_level.top();
		int size = sizeT.top();
		pilha_level.pop();
		pilha.pop();
		sizeT.pop();
		
		vector<Aresta *> E0 =  maximal(S, g, relacao2);
		int level0 = level;
		//for (int oo=0; oo<g->getQuantArestas(); oo++) cout<<T[oo]<<" ";
		//			cout<<endl;
		//cout<<"size E0 = "<<E0.size()<<endl;

		for (int i=0; i<E0.size(); i++){ // para cada e in E0 ...
			
			Aresta *e = E0[i];
			if ((index[e->getId()] > level) || (index[e->getId()] == 0)){
				level0++;
				index[e->getId()] = level0;
			}
		}

		//cout<<"level0 = " <<level0<<endl;
		//cout<<"level = "<<level<<endl;
			int max = -1;
			for (int aa = 0; aa<g->getQuantArestas(); aa++){
				if (T[aa] == 1){
					if (index[aa] > max) max = index[aa];
				}
			}

//		cout<<"max = "<<max<<endl;
		for (int i=0; i<E0.size(); i++){ // para cada e in E0 ...
			Aresta *e = E0[i];
			//cout<<"index[e->getId()] = "<<index[e->getId()]<<endl;
			//cout<<e->getOrigem()<<" "<<e->getDestino()<<" "<<e->getPeso1()<<" "<<e->getPeso2()<<endl;
			if (index[e->getId()] > max || size == 0){
				int * newS = new int[g->getQuantVertices()];
				for (int oo=0; oo<g->getQuantVertices(); oo++)newS[oo] =S[oo];
				int * newT = new int[g->getQuantArestas()];
				for (int oo=0; oo<g->getQuantArestas(); oo++) newT[oo] = T[oo];
		
				newT[e->getId()] = 1;
				newS[e->getOrigem()] = 1;
				newS[e->getDestino()] = 1;
				size++;

				if (size == g->getQuantVertices() -1){
					//cout<<"ADD"<<endl;
					arvores.push_back(make_pair(newS, newT));
				} else{
					//cout<<"recursive"<<endl;
					pilha.push(make_pair(newS, newT));
					pilha_level.push(level0); 
					sizeT.push(size);
				}
				size--;	
			}
		}
	}
	//return arvores;
}

void printResultado(){

	for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	float cont1 = 0, cont2 = 0;
    	pair <int *, int*>  arestas= arvores[k]; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<k+1<<endl;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if ((arestas.second)[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();

    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}

    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
    	cout<<endl;
    }

}

void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec==3600){ //3600
			cout<<"RESULTADO AO FIM DE 1H"<<endl;
			printResultado();
			sleep(3400); // 3400 é importante pra nao ficar verificando todo o tempo
		} else if (sec==7200){//7200
			cout<<"RESULTADO AO FIM DE 2H"<<endl;
			printResultado();
			sleep(3400); // é importante pra nao ficar verificando todo o tempo
		} else if (sec==10800){// 10800 se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			
			cout<<"RESULTADO AO FIM DE 3H"<<endl;
			cout<<"TEMPO LIMITE ATINGIDO..."<<endl;

			printResultado();
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			//exit(-1);
			exit(EXIT_FAILURE);
		}
	}
}

int main(){

	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	cin>>m;
	my_grafo.setN(n);
	Grafo relacao(m); // como a relacao se dá no conjunto de arestas, entao a quantidade de vértices do Grafo relacao será a mesma quantidade de arestas de my_grafo  
	
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	for (id = 0; id<m; id++){
		cin>>origem;
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
	}
	int nA = id; // quantidade de arestas do grafo	
	//cout<<m<<endl;
	id = 0;
	for (int i=0; i<m; i++){ // PADRAO : vértices numerados de 0 à n-1
		relacao.addVertice(i);
	}
	vector< pair<int, int> > relacao2; // primeira aresta domina a segunda
	while (cin>>origem){
		cin>>destino;
		relacao.addArestaDirecionada(id++, origem, destino); // nao nos preocupamos com os pesos para o grafo relacao
		relacao2.push_back(make_pair(origem, destino));
		// origem R destino
	}


	times(&tempsInit);
	
	// para medir o tempo em caso limite
	pthread_t thread_time; 
	pthread_attr_t attr;
	int nnnnnnnn=0;
	if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
        cout<<"Error to create the thread"<<endl;
        //exit(-1);
        exit(EXIT_FAILURE);
    }
    //

	optimalcutset_P(&my_grafo, relacao2);
	
	times(&tempsFinal);
	clock_t user_time1 = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
   	cout<<"RESULTADO FINAL"<<endl;
   	printResultado();
	// for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
 //    	float cont1 = 0, cont2 = 0;
 //    	pair <int *, int*>  arestas= arvores[k]; 
 //    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
 //    	cout<<"Arvore "<<k+1<<endl;
 //    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
	// 		if ((arestas.second)[a] == 1){
	// 			cont1+=arestasPtr[a]->getPeso1();
	// 			cont2+=arestasPtr[a]->getPeso2();

 //    			cout<<arestasPtr[a]->getOrigem() << " ";
 //    			cout<<arestasPtr[a]->getDestino() << " ";
 //    			cout<<arestasPtr[a]->getPeso1() << " ";
 //    			cout<<arestasPtr[a]->getPeso2() << endl;
 //    		}
 //    	}

 //    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
 //    	cout<<endl;
 //    }

	return 0;
}