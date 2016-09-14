/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Alonso et al's (2005) algorithm (optimalcutset_K)
# to resolve the Biobjective Spanning Tree Problem (preference-based approach)

#=======================================================================
*/


#include <iostream>
#include <map> 
#include <string>
#include <utility>
#include <stack>
#include <list>
#include <queue>          // std::queue
#include "Grafo.h"
#include "Conjunto.h"
#include "Kruskal.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;


int idMST = 0;
Aresta ** arestasPtr;

///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, int* Pa, map<int, pair< pair<int*, Conjunto >, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes, queue<int> &fila,int k0){
	/*Pa = vetor de arestas = correspondente à partição P
	*/
	Grafo P1 = P, P2 = P;
	//cout<<List.getSize()<<endl;
	bool res = false;
	float custo;
	Aresta *a; 
	int *A2;
	//map <int, Aresta *> allArestas = P.get_allArestas();
	//for (int i=0; i<P.getQuantVertices()-1; i++){
	int m = P.getQuantArestas();
	for (int i=0; i<m; i++){
		a = arestasPtr[i];
		if (Pa[a->getId()]==1){
			//a = P.getArestas(i);	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				Conjunto conjunto(P1.getQuantVertices());
		
				A2 = new int[m];
				//for(int mmm = 0; mmm<m; mmm++) A2[mmm] = 0;
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				float x, y;
				res = kruskal(&P1, arestasPtr,A2, x, y,k0,conjunto);
				//custo =x*(yl-yll)+y*(xll-xl);
			
				if (res){
					vetorParticoes.push_back(P1);
					list<Grafo>::iterator itt = vetorParticoes.end();
					itt--;
					MSTs[idMST] = make_pair(make_pair(A2,conjunto),itt);//A2;
					fila.push(idMST++);
				 } else {
				 	delete[] A2;
				 }
				P1.setStatus(a->getId(), 1); 
				//P1 = P2;
			}
		}
	}
}

int AllSpaningTree(Grafo *g,list< pair<int*, Conjunto > > &resul, int k0){ 
	

		queue<int> fila;
		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
		map<int, pair< pair<int*, Conjunto>, list<Grafo>::iterator > > MSTs; // usada para lista de árvores
		idMST = 0;
		//cout<<"REDEFINIU"<<endl;
		int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
		float x, y;
		Conjunto conjunto(g->getQuantVertices());
		bool res = kruskal(g, arestasPtr, A,x, y,k0,conjunto);
		
		if (res){
			vetorParticoes.push_back(*g);
			list<Grafo>::iterator itt = vetorParticoes.end();
			itt--;
			MSTs[idMST] = make_pair(make_pair(A, conjunto),itt);//A2;
			fila.push(idMST++);
		}

		while (!fila.empty()){	

			//cout<<"size = "<<fila.size()<<endl;
			int id = fila.front();
			fila.pop();
			pair< pair<int*, Conjunto >, list<Grafo>::iterator > par = MSTs[id];
				
			pair<int*, Conjunto > it = par.first;//MSTs[id];
			Grafo Ps = *par.second;
			
			resul.push_back(it);
			//MSTs.erase(id);
			//vetorParticoes.erase(id);
			Partition(Ps,it.first, MSTs,vetorParticoes,fila,k0);
		}	
	//List.desaloca();
	return MSTs.size();
}


vector <Aresta *> maximal(int* T, Conjunto conjunto, Grafo *my_grafo,vector<pair<int, int> > relacao2){
	//int aux[my_grafo->getQuantVertices()];
	//for (int i=0; i<my_grafo->getQuantVertices(); i++) aux[i] =0; 

	map <int, Aresta *> arestas = my_grafo->get_allArestas();
	int *forComparaison = new int[arestas.size()]; // vetor dos id das arestas
	vector <Aresta *> ret;
	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		Aresta *e = arestas[i];
		if (!conjunto.compare(e->getDestino(), e->getOrigem()))
			forComparaison[e->getId()] = 1;
		else {
			forComparaison[e->getId()] = 0;
			if (T[e->getId()]==1){
				// if (aux[e->getOrigem()]==0) k0++;
				// if (aux[e->getDestino()]==0) k0++;
				// aux[e->getOrigem()]=1;
				// aux[e->getDestino()] = 1;
			 	my_grafo->setStatus(e->getId(), 1);// se a restas estah em T, entao ela é obrigatoria
			}
			else my_grafo->setStatus(e->getId(), 2);
		}

	}
	for (int i = 0; i<arestas.size(); i++){ //O(m) m arestas
		int id = arestas[i]->getId();
		if (forComparaison[id] == 1){
			bool dominada = false;
			for (int j=0; j<relacao2.size(); j++){ 
				if (id==relacao2[j].second){ // se existe alguém que domina v
					if (forComparaison[relacao2[j].first]==1) dominada=true;
				}
			}
			if (dominada==false){
				ret.push_back(arestas[i]);
				// if (aux[arestas[i]->getOrigem()]==0) k0++;
				// if (aux[arestas[i]->getDestino()]==0) k0++;
				// aux[arestas[i]->getOrigem()]=1;
				// aux[arestas[i]->getDestino()] = 1;
				my_grafo->setStatus(arestas[i]->getId(), 0);// se está na lista de maximais, é opcional 
			} else {
				my_grafo->setStatus(arestas[i]->getId(), 2);
			}

		}
	}
	delete[] forComparaison;
	return ret;
}

vector< int* > optimalcutset_K(Grafo *g,vector<pair<int, int> > relacao2){
	vector< int* > resul;
	stack<pair<int *, Conjunto> > pilhaT;
	int *Tinit = new int [g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) Tinit[i] =0; // tudo vazio
	Conjunto Tcinit(g->getQuantVertices());
	pilhaT.push(make_pair(Tinit, Tcinit));

	while (pilhaT.size()!=0){
		///cout<<"Size = "<<pilhaT.size()<<endl;
		pair<int *, Conjunto> s = pilhaT.top();
		pilhaT.pop();
		int *T = s.first;

		////

		////
		
		Conjunto c = s.second;
		
		//int k1 = 0;
		vector <Aresta *> E0 = maximal(T, c, g, relacao2);

		for (int i=0; i<E0.size(); i++){
			c.union1(E0[i]->getOrigem(), E0[i]->getDestino());
		}
		
		int aux[g->getQuantVertices()];
		int k0 = 0;
		for (int i=0; i<g->getQuantVertices(); i++) aux[i] =0; 
		for (int i=0; i<g->getQuantVertices(); i++){
			if (aux[c.find_set(i)]!=1){
				k0++;
				aux[c.find_set(i)] = 1; // um conjunto diferente
			}
		}
		//cout<<"k0 = "<<k0<<endl;
		//cout<<"size E0 = "<<E0.size()<<"  ( "<< E0[0]->getPeso1()<<", "<<E0[0]->getPeso2()<<" )"<<endl;
		//cout<<"k1 = "<<k1<<endl;
		// Conjunto copie(g->getQuantVertices());
		// copie = c;
		list< pair<int*, Conjunto > > resul2;

		AllSpaningTree(g,resul2,k0);
		//cout<<"size = "<<resul2.size()<<endl;
		//cout<<"K1 = "<<k1<<endl;
		for (list<pair<int*, Conjunto > >::iterator itnt = resul2.begin(); itnt!=resul2.end(); itnt++){
			//if (k1==g->getQuantVertices()){
			if (k0==1){
				resul.push_back(itnt->first);
			}else{
				
				pilhaT.push(make_pair(itnt->first, itnt->second));
			}
		}


		//
		// // T U E0
		// for (int i=0; i<E0.size(); i++){
		// 	copie.union1(E0[i]->getOrigem(), E0[i]->getDestino());
		// }
		
		// int aux[g->getQuantVertices()];
		// int k0 = 0;
		// for (int i=0; i<g->getQuantVertices(); i++) aux[i] =0; 
		// for (int i=0; i<g->getQuantVertices(); i++){
		// 	if (aux[copie.find_set(i)]!=1){
		// 		k0++;
		// 		aux[copie.find_set(i)] = 1; // um conjunto diferente
		// 	}

		// } 
		// cout<<"k0 = "<<k0<<endl;
		// 
	
		

	}
	return resul;
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
	//Grafo relacao(m); // como a relacao se dá no conjunto de arestas, entao a quantidade de vértices do Grafo relacao será a mesma quantidade de arestas de my_grafo  
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
	//int nA = id; // quantidade de arestas do grafo	
	//cout<<m<<endl;
	id = 0;
	// for (int i=0; i<m; i++){ // PADRAO : vértices numerados de 0 à n-1
	// 	relacao.addVertice(i);
	// }
	vector< pair<int, int> > relacao2; // primeira aresta domina a segunda
	while (cin>>origem){
		cin>>destino;
		//relacao.addArestaDirecionada(id++, origem, destino); // nao nos preocupamos com os pesos para o grafo relacao
		relacao2.push_back(make_pair(origem, destino));
		// origem R destino
	}

	arestasPtr = my_grafo.getAllArestasPtr();
	times(&tempsInit);
	vector<int* > arvores = optimalcutset_K(&my_grafo, relacao2);
	times(&tempsFinal1);
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	

	map <int, Aresta *> arestas = my_grafo.get_allArestas();
    	
	for (int k = 0; k < arvores.size(); k++){ // cada arvore formada
    	float cont1 = 0, cont2 = 0;
    	int * arvore= arvores[k]; 
    	cout<<"Arvore "<<k+1<<endl;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
			if (arvore[a] == 1){
				cont1+=arestas[a]->getPeso1();
				cont2+=arestas[a]->getPeso2();

    			cout<<arestas[a]->getOrigem() << " ";
    			cout<<arestas[a]->getDestino() << " ";
    			cout<<arestas[a]->getPeso1() << " ";
    			cout<<arestas[a]->getPeso2() << endl;
    		}
    	}

    	cout<<"("<<cont1<<", "<<cont2<<")"<<endl;
    	cout<<endl;
    }

	return 0;
}