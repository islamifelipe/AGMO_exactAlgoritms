/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Perny and Spanjaard's (2003) algorithm (kruskal_like)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include "Grafo.h"
#include <sys/times.h>
#include <unistd.h>
#include "Conjunto.h"
using namespace std;

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

/* recebe um vetor de inteiros, onde o valor do indice i é 1 se a aresta de id=i deve ser contabilizada
retorna um vetor de aresta do grafo normal 
*/
vector <Aresta *> maximal(Grafo *g, int *arestas, Conjunto *conjIt, vector<pair<int, int> > relacao2){ 
	vector <Aresta *> retorno;
	for (int i=0; i<g->getQuantArestas(); i++){
		Aresta *aresta = (g->get_allArestas())[i];
		if (arestas[aresta->getId()]==0){
			if (conjIt->compare(aresta->getOrigem(), aresta->getDestino())==false){
				bool esta = false;
				for (int j=0; j<relacao2.size(); j++){
					Aresta *firstt = (g->get_allArestas())[relacao2[j].first];
					if (relacao2[j].second == aresta->getId() && arestas[relacao2[j].first] == 0 && conjIt->compare(firstt->getOrigem(), firstt->getDestino())==false) {
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
	}
	
	return retorno;
}


vector< int * > krukal_like(Grafo *g, vector< pair<int, int> > relacao2){
	map<int, vector< int * > > at;
	/*cada vector<pair <int *, int*> > funciona, na verdade, como uma lista de arvores de tamanho t, onde t é seu indice em map. Portanto, uma lista de arvores da forma T^(t) 
	I^(t), do algorito original, nada mais é senao os indices do t-ésimo vector do map*/
	//int *grausChegada = new int[relacao->getQuantVertices()];
	at[0].push_back(new int[g->getQuantArestas()]);
	for (int i=0; i<g->getQuantArestas(); i++) at[0][0][i] = 0;
	map<int, vector<Conjunto * > > conjuntos;
	Conjunto *conjunto = new Conjunto(g->getQuantVertices());
	conjuntos[0].push_back(conjunto);
	for (int t=1; t<=g->getQuantVertices()-1; t++){
		vector<int * > It = at[t-1];
		vector<Conjunto* > veco = conjuntos[t-1];
		for (int i=0; i<It.size(); i++){
			Conjunto *conjIt = veco[i];
			vector <Aresta *> max = maximal(g, It[i], conjIt, relacao2);
			for (int a=0; a<max.size(); a++){
				if (conjIt->compare(max[a]->getOrigem(), max[a]->getDestino())==false){
					int * arvore = new int[g->getQuantArestas()];
					for (int p=0; p<g->getQuantArestas(); p++) arvore[p] = It[i][p];
					arvore[max[a]->getId()] = 1;
					Conjunto * auxxx = new Conjunto(g->getQuantVertices());
					auxxx->copia(conjIt);
					auxxx->union1(max[a]->getOrigem(), max[a]->getDestino());
					conjuntos[t].push_back(auxxx);
					at[t].push_back(arvore);
				}
				//}
			}   
		}
		// retira duplicatas
		for (int i=0; i<at[t].size(); i++){
			for (int j=i+1; j<at[t].size(); j++){
				if (isEgal(at[t][i], at[t][j], g->getQuantArestas())){
					at[t].erase(at[t].begin()+j);
					conjuntos[t].erase(conjuntos[t].begin()+j);
				}
			}
		}
	}

	return at[g->getQuantVertices()-1];
	
}

int main(){

	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	

	int n, m;
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	cin>>m;
	Grafo my_grafo(n);
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

	vector< int * > arvores = krukal_like(&my_grafo, relacao2);
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	

	for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	int* arestas= arvores[k]; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<k+1<<endl;
    	float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if (arestas[a] == 1){
				cont1 += arestasPtr[a]->getPeso1();
    			cont2 += arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
    	cout<<endl;
    }

	return 0;
}