/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Perny and Spanjaard's (2003) algorithm (prim_like)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)
# It returns the superset of the maximal solutions
#=======================================================================
*/



#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

struct tms tempsInit, tempsFinal; // para medir o tempo
Grafo my_grafo;
map<int, vector<pair <int *, int*> > > at;

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

/* recebe um vetor de inteiros, onde o valor do indice i é 1 se a aresta de id=i deve ser contabilizada
retorna um vetor de aresta do grafo normal 
*/
vector <Aresta *> maximal(Grafo *g, int *arestas, vector<pair<int, int> > relacao2){ 
	vector <Aresta *> retorno;
	for (int i=0; i<g->getQuantArestas(); i++){
		Aresta *aresta = (g->get_allArestas())[i];
		if (arestas[aresta->getId()]==1){
			bool esta = false;
			for (int j=0; j<relacao2.size(); j++){
				if (relacao2[j].second == aresta->getId() && arestas[relacao2[j].first] == 1) {
					esta = true;
					break;
				}
			}
			if (esta == false){
				retorno.push_back(aresta);
			//	return retorno;
			}
		}
	}
	
	return retorno;
}

int *omega(int* vertices, Grafo *my_grafo){
	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	int *retorno = new int[arestas.size()]; // vetor dos id das arestas

	// primeiramente, seleciona-se as arestas conforme a regra classica de conjunto dijunto de prim   

	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *a = arestas[i];
		if (vertices[a->getOrigem()] != vertices[a->getDestino()]){
			retorno[a->getId()] = 1; 
		} else {
			retorno[a->getId()] = 0; 
		}
	}
	return retorno;
	
}

//vector<pair <int *, int*> > 
void prim_like(Grafo *g, vector<pair<int, int> > relacao2){
	/*cada vector<pair <int *, int*> > funciona, na verdade, como uma lista de arvores de tamanho t, onde t é seu indice em map. Portanto, uma lista de arvores da forma T^(t) 
	I^(t), do algorito original, nada mais é senao os indices do t-ésimo vector do map*/
	//int *grausChegada = new int[relacao->getQuantVertices()];
	
	at[0].push_back(make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]));
	for (int i=0; i<g->getQuantVertices(); i++) (at[0][0].first)[i] = 0;
	(at[0][0].first)[0] = 1;
	for (int i=0; i<g->getQuantArestas(); i++) (at[0][0].second)[i] = 0;

	for (int t=1; t<=g->getQuantVertices()-1; t++){
		vector<pair <int *, int*> > It = at[t-1];
		for (int i=0; i<It.size(); i++){
			
			int *arestas = omega(It[i].first, g);
			vector <Aresta *> max = maximal(g, arestas, relacao2);
			
			for (int a=0; a<max.size(); a++){
				pair <int *, int*> arvore = make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]);
				for (int p=0; p<g->getQuantVertices(); p++) (arvore.first)[p] = (It[i].first)[p];
				for (int p=0; p<g->getQuantArestas(); p++) (arvore.second)[p] = (It[i].second)[p];
				(arvore.first)[max[a]->getOrigem()] = 1;
				(arvore.first)[max[a]->getDestino()] = 1;
				(arvore.second)[max[a]->getId()] = 1;
				at[t].push_back(arvore);
			}   
		}
		// retira duplicatas
		for (int i=0; i<at[t].size(); i++){
			for (int j=i+1; j<at[t].size(); j++){
				if (isEgal(at[t][i].second, at[t][j].second, g->getQuantArestas())){
					at[t].erase(at[t].begin()+j);
					
				}
			}
		}
	}

	//return at[g->getQuantVertices()-1];
	
}

void printResultado(){
	vector<pair <int *, int*> > arvores =at[my_grafo.getQuantVertices()-1];
	for (int k = 0; k <arvores.size(); k++){ // cada arvore formada
	    	pair <int *, int*>  arestas= arvores[k]; 
	    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
	    	cout<<"Arvore "<<k+1<<endl;
	    	float cont1 = 0, cont2 = 0;
	    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
				if ((arestas.second)[a] == 1){
					cont1+= arestasPtr[a]->getPeso1();
					cont2+= arestasPtr[a]->getPeso2();
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

	//vector<pair <int *, int*> > arvores = 
	prim_like(&my_grafo, relacao2);
	
	times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
	cout<<"RESULTADO FINAL"<<endl;
	printResultado();

	return 0;
}