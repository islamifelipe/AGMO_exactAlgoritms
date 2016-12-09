/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Pugliese et al's (2015) algorithm 
# to resolve the 4-objective Spanning Tree Problem
# The user time is returned (in seconds)
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <vector>
#include <utility>      // std::pair
#include <string>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdlib.h>
using namespace std;

pthread_mutex_t mutex1; /*le mutex*/

struct tms tempsInit, tempsFinal; // para medir o tempo
Grafo my_grafo;
map <int, vector< pair<int*, int*> > > L ;//= new map <int, vector< pair<int*, int*> > >[n-1]; // n níveis (1 até n) // nao confundir com a enumeracao dos vertices
int nA;
int n;
	pthread_t thread_executar; 
	pthread_attr_t attr;
	pthread_t thread_time; 

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

/* Recebe os vetores de vértices
Determina se eles sao equivalentes
*/
bool isEquivalente(int *t1, int *t2, int n){ // 
	for (int i=0; i<n; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}
bool t1_domina_t2(int *t1, int *t2, map <int, Aresta *> arestas){ // fracamente
	float t1_peso1=0, t1_peso2=0, t1_peso3=0, t1_peso4=0, t2_peso1=0, t2_peso2=0, t2_peso3=0,t2_peso4=0;
	for (int i=0; i<arestas.size(); i++){
		if (t1[i]==1){
			t1_peso1+=arestas[i]->getPeso1();
			t1_peso2+=arestas[i]->getPeso2();
			t1_peso3+=arestas[i]->getPeso3();
			t1_peso4+=arestas[i]->getPeso4();
		} 
		if (t2[i]==1){
			t2_peso1+=arestas[i]->getPeso1();
			t2_peso2+=arestas[i]->getPeso2();
			t2_peso3+=arestas[i]->getPeso3();
			t2_peso4+=arestas[i]->getPeso4();
		}
	}
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && t1_peso3 <= t2_peso3 && t1_peso4 <= t2_peso4 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2 || t1_peso3 < t2_peso3 || t1_peso4 < t2_peso4)){
		return true;
	} else return false;
}

/*retorna true se Tqh1 é dominada por alguma arvore parcial equivalente pertecente à Lq1 
 Tqh1 é um vetor de 0's e 1's com o comprimento igual ao numero de ARESTAS
 Lq1 DEVE SER UMA REFERÊNCIA
*/
bool isDominada(pair<int*, int*> Tqh1, vector< pair<int*, int*> > &Lq1){
	// verificacao se Tqh1 é dominada por alguém
	for (int t = 0; t<Lq1.size(); t++){
		int *arvore_parcial = Lq1[t].second; 
		if (isEquivalente(Tqh1.first,Lq1[t].first, my_grafo.getQuantVertices()) && t1_domina_t2(arvore_parcial, Tqh1.second, my_grafo.get_allArestas())){
			return true; // é dominada
		}
	} 

	// se Tqh1 nao é dominada por ninguém equivalente, entao verificamos se Tqh1 domina algum t em Lq1. Se sim, removemos t de Lq1.
	for (int t = 0; t<Lq1.size(); t++){
		int *arvore_parcial = Lq1[t].second; 
		if ((isEquivalente(Tqh1.first,Lq1[t].first,  my_grafo.getQuantVertices()) && t1_domina_t2(Tqh1.second, arvore_parcial, my_grafo.get_allArestas())) || isEgal(Tqh1.second, arvore_parcial, my_grafo.getQuantArestas())){
			
			Lq1.erase(Lq1.begin()+t); 
			t--;
		}
	}   
	return false; // nao é dominada
	
}

void printResultado(){
	
	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
	
	cout<<L[n].size()<<endl;
	for (int k = 0; L.find(n)!=L.end() && k < L[n].size(); k++){ // cada arvore formada
    	//cout<<"Depois"<<endl;
    	int *arestas  = L[n][k].second; 
    	cout<<"Arvore "<<k+1<<endl;
    	float cont1 = 0, cont2=0, cont3=0, cont4=0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			if (arestas[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
				cont3+=arestasPtr[a]->getPeso3();
				cont4+=arestasPtr[a]->getPeso4();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << " ";
    			cout<<arestasPtr[a]->getPeso3() <<" ";
    			cout<<arestasPtr[a]->getPeso4() <<endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<", "<<cont3<<", "<<cont4<<")"<<endl;
    	cout<<endl;
    }
}

/* Um estado S é um par de vetores (um de vertices, outro de arestas)
 Lq é um vector de pares, ou seja, estados de S do tipo Sq (nivel q)
 L é um map (cuja chave é q) de Lq's

 A funçao retorna Ln

 OBS.: os autores numeram os vértices de 1 à n; nós porém enumeramos de 0 à n-1
*/
void algoritmoPD(){
	int n = my_grafo.getQuantVertices();
	int *X = new int[n];
	for (int i=0; i<n; i++) X[i] = 0;
	X[0] = 1; // pros autores é o indice 1. Nós enumeramos de 0  
	int *E = new int[my_grafo.getQuantArestas()]; // inicialmente, vazio
	for (int i=0; i<my_grafo.getQuantArestas(); i++) E[i] = 0;
	pair<int*, int*> S11 = make_pair(X, E); // o primeiro par (o primeiro par do nivel 1) 
	vector< pair<int*, int*> > L1;
	L1.push_back(S11);
	L[1] = L1;

	for (int q = 1; q<=n-1; q++) { // para cada nivel. Deve ser no maximo n-1, porque q+1 deve ser no máximo n 
		vector< pair<int*, int*> > Lq = L[q];
		for (int h = 0; h<Lq.size(); h++){ // como Lq é um vector, começaremos do 0
			pair<int*, int*> Sq_h = Lq[h];
			for (int i=0; i<n; i++){
				if ((Sq_h.first)[i] == 1){
					Vertice *v = my_grafo.getVertice(i); //pega o vertice de id=i
					for (int j = 0; j<v->getGrau(); j++){ // aqui, j nao é o id da aresta ij
						Aresta *ij = v->getAresta(j); // a priori, nao há como saber quem é a origem ou o destino (i ou j )
						if ((Sq_h.first)[ij->getOrigem()] != (Sq_h.first)[ij->getDestino()]){ // nao gerar ciclo
							int *Xq1 = new int[n];
							for (int o=0; o<n; o++) Xq1[o] = (Sq_h.first)[o];
							//ij->getOrigem()==i ? Xq1[ij->getDestino()] = 1 : Xq1[ij->getOrigem()] = 1;
							Xq1[ij->getDestino()] = 1;
							Xq1[ij->getOrigem()] = 1;
							int *Eqhij = new int[my_grafo.getQuantArestas()]; 
							for (int o=0; o<my_grafo.getQuantArestas(); o++) Eqhij[o] = (Sq_h.second)[o];
							Eqhij[ij->getId()] = 1;
							pair<int*, int*> Sq1 = make_pair(Xq1, Eqhij);
							
							pthread_mutex_lock(&mutex1); 
								//cout<<"q = "<<q<<endl;
								if (isDominada(Sq1, L[q+1]) == false){
									L[q+1].push_back(Sq1);
								}
							pthread_mutex_unlock(&mutex1);
								
						} 
					}
				}
			}
		}
	}
}

void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec==3600){  //3600
			cout<<"RESULTADO AO FIM DE 1H"<<endl;
			pthread_mutex_lock(&mutex1); 
				printResultado();
			pthread_mutex_unlock(&mutex1);
			sleep(3590); // 3590 é importante pra nao ficar verificando todo o tempo
		} else if (sec==7200){ //7200
			cout<<"RESULTADO AO FIM DE 2H"<<endl;
			pthread_mutex_lock(&mutex1); 
				printResultado();
			pthread_mutex_unlock(&mutex1);
			sleep(3590); // 3590 é importante pra nao ficar verificando todo o tempo
		} else if (sec==10800){// 10800 se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			
			cout<<"RESULTADO AO FIM DE 3H"<<endl;
			cout<<"TEMPO LIMITE ATINGIDO..."<<endl;
			pthread_mutex_lock(&mutex1); 
				printResultado();
			//pthread_mutex_unlock(&mutex1); // NAO LIBERAR, POIS NAO HA NECESSIDADE
			//cout<<"depois"<<endl;
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			exit(EXIT_FAILURE);
			//exit(-1);
		}
	}
}


void *executar(void *nnnn){
	//vector< pair<int*, int*> > arvores = algoritmoPD(&my_grafo);
	algoritmoPD();

	times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time<<endl;
	cout<<(float) user_time / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	 
   	 cout<<"RESULTADO FINAL : "<<endl;
	printResultado();

	exit (EXIT_SUCCESS);
}

int main(){
	
	float peso1, peso2, peso3, peso4;
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
		cin>>peso3;
		cin>>peso4;
		my_grafo.addAresta(id, origem, destino, peso1, peso2,peso3, peso4);
		id++;
	}
	nA = id; // quantidade de arestas do grafo	


	times(&tempsInit);  // pega o tempo do clock inicial


	 // para  executar o algoritmo
	int nmmmmmnn=0;
	if(pthread_create(&thread_executar, NULL, &executar, (void*)nmmmmmnn)){ 
         cout<<"Error to create the thread"<<endl;
        exit(EXIT_FAILURE);
    }
    //

	// para medir o tempo em caso limite
	int nnnnnnnn=0;
	if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ 
         cout<<"Error to create the thread"<<endl;
        exit(EXIT_FAILURE);
      }
    //

    pthread_join(thread_time,NULL);
    pthread_join(thread_executar,NULL);
	
	return 0;
}