/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Lokman and Koksalan's (2013) algorithm 
# based on the Multiobjective Integer Program for p = 2
# This code implements the Branch and Bound algorithm to reolve each model Pnm
#=======================================================================
*/


#include <iostream>
#include <map> 
#include <string>
#include <climits>
#include <stack>   
#include <utility>      // std::pair
#include "Grafo.h"
#include <cmath>
#include "Heap.h"
#include "Conjunto.h"
#include <list>
using namespace std;



bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}

bool comp ( pair<int,float> i, pair<int,float> j) { return (maiorQuefloat(i.second, j.second)); }

bool atendeCriterios(int *solucao, vector< pair<int, float> > criterio){
	float sum = 0.0;
	for (int i=0; i<criterio.size()-1; i++){ // para cada coeficiente
		if (solucao[criterio[i].first] == 1) {
			sum += criterio[i].second; 
		}
	}

	if (!(sum >= criterio[criterio.size()-1].second)) return false; // esse >= é padrao para o caso particular da AGM-bi e para todos os critérios adcionados a cada iteracao (segundo Lokman and Koksalan)
	
	return true;
}


vector < pair<int,float> > getObjectiveFonction(Grafo &g, float epslon){ // considerando que m = 2
	vector< pair<int,float> > objetivo;
	map <int, Aresta *> arestas = g.get_allArestas();
	for (int j=0; j<g.getQuantArestas(); j++){
		pair<int,float> p = make_pair(arestas[j]->getId(),arestas[j]->getPeso2()+epslon*arestas[j]->getPeso1());
		p.second = p.second*(-1);
		objetivo.push_back(p);
	}
	return objetivo;

}

vector < pair<int,float> > getCoefCriterios(Grafo &g){
	vector< pair<int,float> > coefs;
	map <int, Aresta *> arestas = g.get_allArestas();
	for (int j=0; j<g.getQuantArestas(); j++){
		pair<int,float> p = make_pair(arestas[j]->getId(),  arestas[j]->getPeso1()*(-1));
		coefs.push_back(p);
	}
	return coefs;
}


// O vetor de objetivos (negativos) ja deve está ordenado em ordem decrescente (como a ordenacao é única, faze-la-emos no main)
bool krukalMax(Grafo &g, vector< pair<int,float> > objetivo, int *A, float &custo){
	custo = 0;
	Conjunto conjunto(g.getQuantVertices());
	map <int, Aresta *> listaAresta = g.get_allArestas();
	int cont = 0;
	for (int k=0; k<g.getQuantArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g.getStatus(objetivo[k].first)==1){ /*se for obrigatória*/
		 	A[objetivo[k].first] = 1;
			cont++; // contador que, ao final, deve ser igual à getQuantVertices()-1 arestas (uma arvore)
		 	conjunto.union1(listaAresta[objetivo[k].first]->getOrigem(), listaAresta[objetivo[k].first]->getDestino());
			custo+=objetivo[k].second; // Lembrar : os objetivos ja vêm com os coeficientes negativos
		} else {
			A[objetivo[k].first] = 0;
		}
	}
	int i=0;
	while (cont<g.getQuantVertices()-1 && i<objetivo.size()){ 
	/*A condição "i<g->getQuantArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getQuantArestas(), porém, o cont não será será igual a g->getN()-1 */
		if (g.getStatus(objetivo[i].first)==0 && !conjunto.compare(listaAresta[objetivo[i].first]->getOrigem(), listaAresta[objetivo[i].first]->getDestino())){ /*Se não formar ciclo*/
			cont++; // contador que, ao final, deve ser igual à n-1 arestas (uma arvore)
			A[objetivo[i].first] = 1;
			conjunto.union1(listaAresta[objetivo[i].first]->getOrigem(), listaAresta[objetivo[i].first]->getDestino());
			custo+=objetivo[i].second; // Lembrar : os objetivos ja vêm com os coeficientes negativos
		}
		i++;
	}
	conjunto.desaloca();
	if (cont==g.getQuantVertices()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/

}
void Partition(Grafo P, int *Pa, Heap &List, list <int *> &MSTs,list<Grafo> &vetorParticoes, vector< pair<int,float> > objetivo){
	/*Pa = vetor de arestas = correspondente à partição P
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P, P2 = P;
	//cout<<List.getSize()<<endl;
	bool res = false;
	float custo, x, y;
	Aresta *a; 
	int *A2;
	map <int, Aresta *> allArestas = P.get_allArestas();
	
	for (int i=0; i<allArestas.size(); i++){
		if (Pa[i]==1){
			a = allArestas[i];	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				A2 = new int[allArestas.size()];
				for(int mmm = 0; mmm<allArestas.size(); mmm++) A2[mmm] = 0;
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				
				custo=0;
				res = krukalMax(P1, objetivo, A2,custo);

				if (res){
					int quantMSTs = MSTs.size();
					MSTs.push_back(A2);
					List.insert(quantMSTs, custo); // o valor da variavel "custo" vem do kruskal
					vetorParticoes.push_back(P1);
				} else {
					delete[] A2;
				}
				//cout<<"Depois1  "<<List.getSize()<<"   "<<res<<endl;// estatístico
				
				P1 = P2;
			}
		}
	}
}
int * resolvePnm(vector< pair<int, float> > criterio, vector< pair<int,float> > objetivo, list<int*> &MSTs,Heap &List,list<Grafo> &vetorParticoes){
	
	do{ 
		if (List.getSize()>0){
			int id = List.getId();
			//ElementGrafo *init = vetorParticoes->getInit();
			//ElementArvore *initArvore = MSTs->getInit();
			list<int*>::iterator it = MSTs.begin();
			list<Grafo>::iterator itg = vetorParticoes.begin();
			int k=0;
			while (k<id && k<MSTs.size()){
				it++;
				itg++;
				k++;
			}
			// TODO : FAZER ESTIMATIVAS AQUI
			Grafo Ps = *itg;
			bool ok = atendeCriterios(*it, criterio);
			
			List.extract();
			//MSTs.remove(*it);
			//vetorParticoes.remove(Ps);
			Partition(Ps,*it, List,MSTs,vetorParticoes, objetivo);
			
			if (ok == true)
				return *it;
			}
			
	}while (List.getSize()>0);
	return NULL;

}

/* 
* Algoritmo 1 de Lokman and Koksalan 	
O vetor de coefeficientes do critério (de tamanho g.getQuantArestas()+1) já deve vir ordenado em ordem decrescente 
***** (exceto o ultimo) *****
*/
vector <int *> algorithm1(Grafo &g, vector< pair<int,float> >  criterio, vector< pair<int,float> > objetivo){
	vector <int *> resul;
	map <int, Aresta *> arestas = g.get_allArestas();
	list<int*> MSTs; // usada para lista de árvores
	float custoMinimo =0;
	Heap List; // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	
	int * primeira = new int [g.getQuantArestas()];
	bool res = krukalMax(g, objetivo, primeira, custoMinimo); // arvore que maximiza (negativamente) o obejtivo sem criterios Pm0
	
	
	if (res){
		resul.push_back(primeira);
		float sum = 1;
		for (int j=0; j<g.getQuantArestas(); j++) sum+=((arestas[j]->getPeso1()*primeira[j])*(-1));
		cout<<sum<<endl;
		criterio.push_back(make_pair(-1,sum)); // primeiro critério 	
		int contMSTs = MSTs.size();
		MSTs.push_back(primeira);
		List.insert(contMSTs, custoMinimo);
		vetorParticoes.push_back(g);
	}
	int * nova = NULL;;
	int contA = 0;
	do{
		nova = resolvePnm(criterio,objetivo,MSTs, List,vetorParticoes);
		if (nova!=NULL){
			contA++;
			cout<<"!=====ACHOU! " <<contA<<" =====!"<<endl;
			resul.push_back(nova);
			float sum = 1;
			for (int j=0; j<g.getQuantArestas(); j++) sum+=((arestas[j]->getPeso1()*nova[j])*(-1));
			cout<<"List size = "<<List.getSize()<<endl;
			if (criterio.size() == g.getQuantArestas()+1) {
				if (criterio[criterio.size()-1].second < sum ) criterio[g.getQuantArestas()].second = sum; // conserva sempre o maior ztj
			} else criterio.push_back(make_pair(-1,sum));
		}
	} while(nova!=NULL);
	return resul;

}
int main(){
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
	float epslon; // fornecer na entrada
	cin>>epslon;
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}
	int nA = id; // quantidade de arestas do grafo	

	int m = 2;// por default, o m falado por Lokman and Koksalan e m = p = 2

	
	vector < pair<int,float> > criterios = getCoefCriterios(my_grafo); // cada int é o id da aresta (coeficiente), float é o coeficiente correspondente,
    vector < pair<int,float> > objetivo = getObjectiveFonction(my_grafo, epslon); 
    /* por que um vector de pair? 
    * porque o int deve ser o id do coeficiente (igual ao id da aresta) e o float é 
    *  peso da aresta + epson (ou seja, o coeficiente propriamente dito)
     */


	std::sort (objetivo.begin(), objetivo.end(), comp); // O(nlogn)
	std::sort (criterios.begin(), criterios.end(), comp); // O(nlogn)
	

	// for (int i=0; i<objetivo.size(); i++) cout<<objetivo[i].second<<" ";
	// cout<<endl;
	// for (int i=0; i<criterios.size(); i++) cout<<criterios[i].second<<" ";
	// cout<<endl;
	
	/*Heap List(1000);
	List.insert(1,12);
	List.insert(2,30);
	List.insert(3,40);
	List.insert(4,30);
	List.insert(5,2);
	cout<< List.getId()<<", "<< List.getChave()<<endl;
	List.extract();
	cout<< List.getId()<<", "<< List.getChave()<<endl;
		List.extract();
	cout<< List.getId()<<", "<< List.getChave()<<endl;
		List.extract();
	cout<< List.getId()<<", "<< List.getChave()<<endl;
		List.extract();
	cout<< List.getId()<<", "<< List.getChave()<<endl;*/

	 vector<int *> sol = algorithm1(my_grafo, criterios, objetivo);
     cout<<"Finalizado!! Resultado : "<<endl;
	 map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    for (int k = 0; k < sol.size(); k++){ // cada arvore formada
    	int *arestas  = sol[k]; 
    	cout<<"Arvore "<<k+1<<endl;
    	float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			if (arestas[a] == 1){
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
	return 0;
}