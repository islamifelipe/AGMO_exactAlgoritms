/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Perny and Spanjaard's (2003) algorithm (kruskal_like)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)
# It returns just only solution Pareto efficient
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <climits>
#include <string>
#include <utility>
#include "Grafo.h"
#include "Conjunto.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;



// // calcula os fronts
// void front11(Grafo *g,vector <Aresta *> &front){
// 	map <int, Aresta *> arestas = g->get_allArestas();
// 	for (int i=0; i<g->getQuantArestas(); i++){
// 		if (arestas[i]->isUsed()==false){
// 			bool isDominated = false;
// 			for (int j=i+1; j<g->getQuantArestas(); j++){
// 				if (arestas[j]->isUsed()==false){
// 					if (arestas[j]->getPeso1()<=arestas[i]->getPeso1() && arestas[j]->getPeso2()<=arestas[i]->getPeso2() && (arestas[j]->getPeso1()<arestas[i]->getPeso1() || arestas[j]->getPeso2()<arestas[i]->getPeso2())){
// 						isDominated=true;
// 						break;
// 					}
// 				}
// 			}
// 			if (isDominated==false){
// 				front.push_back(arestas[i]);
// 			}
// 		}
// 	}


// }

// pair <int *, int*> krukal_like(Grafo *g){
// 	vector <Aresta *> front;
// 	front11(g, front);
// 	pair <int *, int*> retorno = make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]);
// 	for (int i=0; i<g->getQuantVertices(); i++) (retorno.first)[i] = 0;
//  	for (int i=0; i<g->getQuantArestas(); i++) (retorno.second)[i] = 0;
// 	int contArestas = 0; // deve atingir g->getQuantVertices()-1;
// 	Conjunto conjunto(g->getQuantVertices());
// 	int comeco = 0;
// 	while (contArestas<g->getQuantVertices()-1){
// 		for (int i=comeco; i<front.size() && contArestas<g->getQuantVertices()-1; i++){ // os novos elemntos do front sao colocados na cabeça do vetor, de modo que quando o i atinge uma aresta usada, ele para
// 			if (front[i]->isUsed()==false){
// 				if (conjunto.compare(front[i]->getOrigem(), front[i]->getDestino()==false)){
// 					(retorno.first)[front[i]->getOrigem()] = 1;
//  					(retorno.first)[front[i]->getDestino()] = 1;
//  					(retorno.second)[front[i]->getId()] = 1;
//  					conjunto.union1(front[i]->getOrigem(),front[i]->getDestino());
// 					contArestas++;
// 				}
// 				front[i]->toUse(true);
// 			} 
// 		}
// 		comeco = front.size();
// 		front11(g, front);
// 	}
// 	return retorno;

// }

Aresta * maximal(Grafo *g,Conjunto conjunto ){ // g é o grafo normal (nao o da relacao)
	map <int, Aresta *> arestas = g->get_allArestas();
	Aresta * retorno;
	float min1 = INT_MAX, min2 = INT_MAX;
	for (int i=0; i<g->getQuantArestas(); i++){
		Aresta *a = arestas[i];
		if (conjunto.compare(a->getOrigem(), a->getDestino()) == false){
			if (a->getPeso1()<min1 || (a->getPeso1()==min1 && a->getPeso2()<min2)){
				retorno = a;
				min1 = a->getPeso1();
				min2 = a->getPeso2();
			}		
		}
	}
	return retorno;
}


pair <int *, int*> krukal_like(Grafo *g){
	map<int, vector<pair <int *, int*> > > at;
	/*cada vector<pair <int *, int*> > funciona, na verdade, como uma lista de arvores de tamanho t, onde t é seu indice em map. Portanto, uma lista de arvores da forma T^(t) 
	I^(t), do algorito original, nada mais é senao os indices do t-ésimo vector do map*/
	at[0].push_back(make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]));
	for (int i=0; i<g->getQuantVertices(); i++) (at[0][0].first)[i] = 0;
	for (int i=0; i<g->getQuantArestas(); i++) (at[0][0].second)[i] = 0;

	pair <int *, int*> corrente = at[0][0];
	Conjunto cojunto(g->getQuantVertices());
	for (int t=1; t<=g->getQuantVertices()-1; t++){
		//vector<pair <int *, int*> > It = at[t-1];
		//for (int i=0; i<It.size(); i++){
			//for (int p=0; p<relacao->getQuantVertices(); p++) grausChegada[p] = relacao->getVertice(p)->getGrau_chegada();
			// for (int p=0; p<g->getQuantArestas(); p++){
			// 	if ((It[i].second)[p] == 1) subtrai(relacao,grausChegada, p);
			// 	else if ((It[i].first)[g->get_allArestas()[p]->getOrigem()] ==1 &&  (It[i].first)[g->get_allArestas()[p]->getDestino()] ==1) subtrai(relacao,grausChegada, p);// retira-se tambem as que foram ciclo
					
			// }

			Aresta * max = maximal(g, cojunto);
			//for (int a=0; a<max.size(); a++){
				//if ((It[i].first)[max[a]->getOrigem()] != (It[i].first)[max[a]->getDestino()] || ((It[i].first)[max[a]->getOrigem()] == 0 && (It[i].first)[max[a]->getDestino()]==0)){ // nao forma ciclo
					//pair <int *, int*> arvore = make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]);
					(corrente.first)[max->getOrigem()] = 1;
					(corrente.first)[max->getDestino()] = 1;
					(corrente.second)[max->getId()] = 1;
					cojunto.union1(max->getOrigem(),max->getDestino());
					//at[t].push_back(arvore);
				//}
			//}   
		///}
		// retira duplicatas
		// for (int i=0; i<at[t].size(); i++){
		// 	for (int j=i+1; j<at[t].size(); j++){
		// 		if (isEgal(at[t][i].second, at[t][j].second, g->getQuantArestas())){
		// 			at[t].erase(at[t].begin()+j);
					
		// 		}
		// 	}
		// }
	}

	return corrente;
	
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
	while (cin>>origem){
		cin>>destino;
		relacao.addArestaDirecionada(id++, origem, destino); // nao nos preocupamos com os pesos para o grafo relacao
		// origem R destino
	}

	times(&tempsInit);

	pair <int *, int*>  arestas = krukal_like(&my_grafo);
	
	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
	//for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	//pair <int *, int*>  arestas= arvores[k]; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<endl;
    	float cont1 =0, cont2=0;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if ((arestas.second)[a] == 1){
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
   // }

	return 0;
}