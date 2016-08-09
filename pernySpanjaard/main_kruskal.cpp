/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Perny and Spanjaard's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include "Grafo.h"
using namespace std;

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
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

/* recebe um vetor de inteiros, onde o valor do indice i refere-se o grau de chegada do vértice i da relacao
retorna um vetor de aresta do grafo normal (nao o da relacao)
*/
vector <Aresta *> maximal(Grafo *g, int *grausChegada){ // g é o grafo normal (nao o da relacao)
	vector <Aresta *> retorno;
	for (int i=0; i<g->getQuantArestas(); i++){
		if (grausChegada[i]==0){
			retorno.push_back((g->get_allArestas())[i]);
		}
	}
	return retorno;
}

/* Usada para subtrair um vertice v do grafo relacao, bem como seus graus de chegada e saida
*/
void subtrai(Grafo *relacao,int *grausChegada, int v){
	grausChegada[v] = -1;
	Vertice *vertice = relacao->getVertice(v);
	for (int i=0; i<vertice->getGrau(); i++){
		Aresta *a = vertice->getAresta(i);
		grausChegada[a->getDestino()]--;
	}
}

vector<pair <int *, int*> > krukal_like(Grafo *g, Grafo *relacao){
	map<int, vector<pair <int *, int*> > > at;
	/*cada vector<pair <int *, int*> > funciona, na verdade, como uma lista de arvores de tamanho t, onde t é seu indice em map. Portanto, uma lista de arvores da forma T^(t) 
	I^(t), do algorito original, nada mais é senao os indices do t-ésimo vector do map*/
	int *grausChegada = new int[relacao->getQuantVertices()];
	at[0].push_back(make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]));
	for (int i=0; i<g->getQuantVertices(); i++) (at[0][0].first)[i] = 0;
	for (int i=0; i<g->getQuantArestas(); i++) (at[0][0].second)[i] = 0;

	for (int t=1; t<=g->getQuantVertices()-1; t++){
		vector<pair <int *, int*> > It = at[t-1];
		for (int i=0; i<It.size(); i++){
			for (int p=0; p<relacao->getQuantVertices(); p++) grausChegada[p] = relacao->getVertice(p)->getGrau_chegada();
			for (int p=0; p<g->getQuantArestas(); p++){
				if ((It[i].second)[p] == 1) subtrai(relacao,grausChegada, p);
			}
			vector <Aresta *> max = maximal(g, grausChegada);
			for (int a=0; a<max.size(); a++){
				if ((It[i].first)[max[a]->getOrigem()] != (It[i].first)[max[a]->getDestino()] || ((It[i].first)[max[a]->getOrigem()] == 0 && (It[i].first)[max[a]->getDestino()]==0)){ // nao forma ciclo
					pair <int *, int*> arvore = make_pair(new int[g->getQuantVertices()], new int[g->getQuantArestas()]);
					for (int p=0; p<g->getQuantVertices(); p++) (arvore.first)[p] = (It[i].first)[p];
					for (int p=0; p<g->getQuantArestas(); p++) (arvore.second)[p] = (It[i].second)[p];
					(arvore.first)[max[a]->getOrigem()] = 1;
					(arvore.first)[max[a]->getDestino()] = 1;
					(arvore.second)[max[a]->getId()] = 1;
					at[t].push_back(arvore);
				}
			}   
		}
	}

	return at[g->getQuantVertices()-1];
	
}

int main(){
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
	cout<<m<<endl;
	id = 0;
	for (int i=0; i<m; i++){ // PADRAO : vértices numerados de 0 à n-1
		relacao.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		relacao.addArestaDirecionada(id++, origem, destino); // nao nos preocupamos com os pesos para o grafo relacao
		// origem R destino
	}

	vector<pair <int *, int*> > arvores = krukal_like(&my_grafo, &relacao);
	for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	pair <int *, int*>  arestas= arvores[k]; 
    	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	cout<<"Arvore "<<k+1<<endl;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if ((arestas.second)[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<endl;
    }


	// // RASCUNHO de TESTES
	// cout<<"saiu"<<endl;
	// int *grausChegada = new int[relacao.getQuantVertices()];
	
	// for (int i=0; i<relacao.getQuantVertices(); i++) grausChegada[i] = relacao.getVertice(i)->getGrau_chegada();
	
	// vector <Aresta *> max = maximal(&my_grafo, grausChegada);
	// for (int i=0; i<max.size(); i++){
	// 	cout<<max[i]->getOrigem() << " ";
 //    	cout<<max[i]->getDestino() << " ";
 //    	cout<<max[i]->getPeso1() << " ";
 //    	cout<<max[i]->getPeso2() << endl;
	// }
	
	// subtrai(&relacao, grausChegada, 1);
	
	// cout<<"\n\n\n"<<endl;
	// max = maximal(&my_grafo, grausChegada);
	// for (int i=0; i<max.size(); i++){
	// 	cout<<max[i]->getOrigem() << " ";
 //    	cout<<max[i]->getDestino() << " ";
 //    	cout<<max[i]->getPeso1() << " ";
 //    	cout<<max[i]->getPeso2() << endl;
	// }

   /* */
	return 0;
}