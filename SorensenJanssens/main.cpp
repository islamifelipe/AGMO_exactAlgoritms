/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2017
#-----------------------------------------------------------------------
# This code implements SONRENSEN JANSSENS' (2003) algorithm 
# to calculate the k-best tree
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <vector>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
#include "margeSort.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

#define MAX2 2000 // TODO : Aumentar

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;


bool isEgalObjetive(float t1_peso1, float t1_peso2, float t2_peso1, float t2_peso2){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);
}


///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, int* Pa, Heap &List, map<int, pair< pair<int*, float>, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes){
	/*Pa = vetor de arestas = correspondente à partição P (n-1 arestas)
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P, P2 = P;
	bool res = false;
	float custo;
	int *A2;
	int m = P.getQuantArestas();
	int n =P.getQuantVertices();
	for (int i=0; i<n-1 && List.getSize()<MAX2; i++){
		int iddnovo = Pa[i];
		if (P.getStatus(iddnovo)==0){ /*Se a aresta for opcional*/
			A2 = new int[n-1];
			//A2 = new int[m];
			P1.setStatus(iddnovo, 2); /*proibida*/
			P2.setStatus(iddnovo, 1); /*obrigatória*/
			custo=0;
			res = kruskal(&P1, arestasPtr,A2, custo);
			//custo =x*(yl-yll)+y*(xll-xl);
			if (res){
				vetorParticoes.push_back(P1);
				list<Grafo>::iterator itt = vetorParticoes.end();
				itt--;
				MSTs[idMST] = make_pair(make_pair(A2,custo),itt);//A2;
				List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
			} else {
				delete[] A2;
			}
			P1.setStatus(iddnovo, 1); 
		}
	}
}

vector< pair<int*, float> > AllSpaningTree(Grafo *g, int k){ 
	vector< pair<int*, float> > result;
	Heap List(MAX2); 
	list<Grafo> vetorParticoes; //Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	map<int, pair< pair<int*, float>, list<Grafo>::iterator > > MSTs; // usada para lista de árvores do retorno
	idMST = 0;

	int *A = new int[g->getQuantVertices()-1]; // usada para a primeira árvore 
	mergesort(arestasPtr, g->getQuantArestas());
	float custo = 0;
	bool res = kruskal(g, arestasPtr, A,custo);
		
	if (res){
		vetorParticoes.push_back(*g);
		list<Grafo>::iterator itt = vetorParticoes.end();
		itt--;
		MSTs[idMST] = make_pair(make_pair(A, custo),itt);//A2;
		List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
	}


	while (List.getSize()!=0){
			int id = List.getId();
			pair< pair<int*, float >, list<Grafo>::iterator > par = MSTs[id];
				
			pair<int*, float > it = par.first;//MSTs[id];
			result.push_back(it);
			Grafo Ps = *par.second;
			
			List.extract();
			
			Partition(Ps,it.first, List,MSTs,vetorParticoes);
	}		
	return result;
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)


int main(){
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	
	int n;
	float peso1;
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
		my_grafo.addAresta(id, origem, destino, peso1);
		id++;
	}

	int nA = id; // quantidade de arestas do grafo	
	//my_grafo.gerarArestasPtr();
	
	arestas = my_grafo.get_allArestas();
	arestasPtr = my_grafo.getAllArestasPtr();
	vector< pair<int*, float> > result = AllSpaningTree(&my_grafo, 0);
	cout<<result.size()<<endl;
	for (int i=0; i<result.size(); i++){
		pair<int*, float> par_arvore = result[i];
		cout<<"Arvore "<<i<<"  ---  Peso = "<<par_arvore.second<<endl;
		for (int a = 0; a < n-1; a++){
			cout<<arestas[par_arvore.first[a]]->getOrigem()<<", "<<arestas[par_arvore.first[a]]->getDestino()<<", "<<arestas[par_arvore.first[a]]->getPeso()<<endl;
		}
		cout<<endl;
	}

	

 // 	times(&tempsInit);

	//  list <pair<int*, pair<float, float> > > arvores = phase1GM(&my_grafo);
	// cout<<"Fim da primeira fase ... "<<endl;

	// times(&tempsFinal1);   /* current time */ // clock final
	// clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	// cout<<user_time1<<endl;
	// cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
 //   	times(&tempsInit);


	// list <pair<int*, pair<float, float> > > noSuportadas = phase2KB(&my_grafo, arvores);
	

	// cout<<"Fim da segunda fase ... "<<endl;

	// times(&tempsFinal2);   /* current time */ // clock final
	// clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	// cout<<user_time2<<endl;
	// cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	// cout<<"Total ..."<<endl;
	// cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;

	
 //    int i = 1, cont=0;
 //    //cout<<"saiu2"<<endl;
 //    //cout<<resul.size()<<endl;
 //   // list<int*>::iterator it=resul.begin();
 //   cout<<"Resultado \n SUPORTADAS"<<endl;
 //    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		
	// 	cout<<"Arvore "<<i<<endl;
 //    	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
	// 		int iddd = ((*it).first)[a];
			
	// 			cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    		
 //    	}
 //    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
 //    	i++;
	// }


	// cout<<"Resultado \n NAO SUPORTADAS"<<endl;
 //    for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		
	// 	cout<<"Arvore "<<i<<endl;
 //    	for (int a = 0; a<n-1; a++){ // cada aresta da arvore
	// 		int iddd = ((*it).first)[a];
	// 			cout<<my_grafo.getArestas(iddd)->getOrigem() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getDestino() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getPeso1() << " ";
 //    			cout<<my_grafo.getArestas(iddd)->getPeso2() << endl;
    		
 //    	}
 //    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
 //    	i++;
	// }

	return 0;
}