/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Lokman and Koksalan's (2013) algorithm 
# based on the Multiobjective Integer Program for p = 2
# This code implements the Branch and Bound algorithm to reolve each model Pnm
#=======================================================================
*/

/*
# O modelo terá |E| variaveis de decisao : 1 se a aresta corespondente está na árvore, 0 caso contrário
# as restricoes sao : 
	Somatorio Ai = |V| - 1
	para cada vértice v, o Somatorio de Ai >= grau de v, onde Ai sao as arestas que incidem em v
*/

#include <iostream>
#include <map> 
#include <string>
#include <climits>
#include <stack>   
#include <utility>      // std::pair
#include "Grafo.h"
#include <cmath>
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

// usada para a ordenacao dos coeficientes da funcao objetivo
bool comp ( pair<int,float> i, pair<int,float> j) { return (maiorQuefloat(i.second, j.second)); }

bool atendeCriterios(int size, int *solucao, map<int, float*> criterios){
	for (int i=0; i<criterios.size(); i++){ // para cada critério
		float sum = 0.0;
		for (int j =0; j<size-1; j++){ // para cada variavel (aresta) 
			sum += solucao[j]*criterios[i][j]; 
		}
		cout<<sum<<endl;
		if (!(sum >= criterios[i][size-1])) return false; // esse >= é padrao para o caso particular da AGM-bi e para todos os critérios adcionados a cada iteracao (segundo Lokman and Koksalan)
	}
	return true;
}

//retorna a solucao que *** maximiza *** o modelo pnm 
int *branchAndBound(Grafo &g, map<int, float*> criterios, vector< pair<int,float> > objetivo){
	/* IMPORTANTE : 
	Lokman and Koksalan trantam do problema de maximizacao. 
	Como a AGM-Bi é de minimizacao, esta funcao BB supõe que os coeficientes da funcao objetivo foram todos multiplicado por -1
	*/
	/* LB = limite infeior = melhor solucao encontrada até o momento para a maximizacao. Começar com um valor inicial
	   UB = Construído a cada nó da arvore de busca. Pega a soma das arestas incluidas mais as opcionais de maiores valores. Esse valor é o maior possivel em que podemos chegar. Ou seja, UB. Caso UB seja menor ou igual à LB (ou seja, o melhor que podemos fazer é ainda menor que o que ja temos, podamos a arvore)
	*/
	float LB = INT_MIN;
	float UB = 0;
	stack< pair<int, int* > > pilha; 
	// para simular a recursao : o int é a quantidade de variavais em 1
	// o vetor int* (de comprimeito |E|) é composto por 1's ou 0's para indicar a escolha ou nao da aresta de id = i  

	stack< pair<int, float > > nivelUB; // int = nivel da arvore de busca; float : valor de UB
	
	int *melhorSolucao = new int[g.getQuantArestas()];
	int *sol = new int[g.getQuantArestas()];
	for (int j=0; j<g.getQuantArestas(); j++) {
		melhorSolucao[j] = 0;
		sol[j] = 0;
	}
	pilha.push(make_pair(0, sol));
	nivelUB.push(make_pair(0,UB));
	
	while (pilha.size()!=0){
		pair<int, float > nivelUBp = nivelUB.top();
		nivelUB.pop();
		int nivel = nivelUBp.first;
		float UBaux = nivelUBp.second;
		pair<int, int* > s = pilha.top();
		pilha.pop();
		int sizeSolucao = s.first;
		int* solucao = s.second;
		
		/*estimando UB : pega os maiores coeficientes de cuja variavais sao diferente de 1*/
		int cont=sizeSolucao;
		float UBestimado = UBaux;
		for (int j=0; j<g.getQuantArestas() && cont<g.getQuantVertices()-1; j++) { // para cada aresta
			if (solucao[objetivo[j].first] != 1){ // se a aresta nao estiver inserida
				UBestimado+=objetivo[j].second;
				cont++;
			}
		}

		if (UBestimado>LB){ // senao, poda...
			cout<<"Aqui1"<<endl;
			if (sizeSolucao == g.getQuantVertices()-1 && atendeCriterios(g.getQuantArestas()+1, solucao, criterios)){ // solucao otima por momento
				cout<<"Aqui2"<<endl;
				LB = UBestimado;
				for (int j=0; j<g.getQuantArestas(); j++) melhorSolucao[j] = solucao[j];
			} else{
				cout<<nivel<<endl;
				if ( nivel<g.getQuantArestas()) { // veficiamos se os criterios sao atendiddos
					cout<<"Aqui4"<<endl;
					/* a cada iteracao, verificamos a aresta que tem id = nivel*/
					int *avec = new int[g.getQuantArestas()];
					int *sans = new int[g.getQuantArestas()];
					float coef = 0.0;
					for (int j=0; j<g.getQuantArestas(); j++){
						if (objetivo[j].first == nivel)  coef = objetivo[j].second;
						avec[j] = solucao[j];
						sans[j] = solucao[j];
					}
					avec[nivel] = 1;
					sans[nivel] = 0;
					nivelUB.push(make_pair(nivel+1, UBaux)); // sans
					pilha.push(make_pair(sizeSolucao, sans)); //sans

					nivelUB.push(make_pair(nivel+1, UBaux+coef)); // avec
					pilha.push(make_pair(sizeSolucao+1, avec)); // avec

				}
				 // se nao atende aos critérios, o presente nó de busca nao é expandido
			}
		}
		delete[] solucao;
	}

	return melhorSolucao;
}

map<int, float*> initialization(Grafo &g){
	map<int, float*> criterios; // embora no inicio os coeficientes sejam inteiros (0 ou 1), depois outros critérios serao adicionandos com coneficientes reais
	for (int i=0; i<g.getQuantVertices(); i++){
		float *vi = new float[g.getQuantArestas()+1]; // a1 ... am >= 1 onde m = |E|
		Vertice *v = g.getVertice(i);
		for (int j=0; j<g.getQuantArestas()+1; j++) vi[j] = 0.0;
		for (int j=0; j<v->getGrau(); j++){
			vi[v->getAresta(j)->getId()] = 1.0;
		}
		vi[g.getQuantArestas()] = 1;
		criterios[i] = vi;
	}
	return criterios;
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

	
	map<int, float*> criterios; // cada int é o id do cretério (inequacao), float é o um array (critério) correspondente, que, no nosso caso, terá |E|+1 elementos que guardam o coeficientes
    // o critério de a1 + ... + anA = |V| - 1 nao está contido no map. Ele vai ser passado ao BB como um parâmetro à parte
    vector < pair<int,float> > objetivo = getObjectiveFonction(my_grafo, epslon); 
    // por que um vector de pair? porque o int deve ser o id do coeficiente (igual ao id da aresta) e o float é peso da aresta + epson (ou seja, o coeficiente propriamente dito)

	criterios = initialization(my_grafo);

	std::sort (objetivo.begin(), objetivo.end(), comp); // O(nlogn)
	// for (int j=0; j<my_grafo.getQuantArestas(); j++){
	// 	cout<<"("<< objetivo[j].first<<", "<< objetivo[j].second<<") ";
	// }
	// cout<<endl;

	int *sol = branchAndBound(my_grafo, criterios, objetivo);


	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    	
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			if (sol[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}   
    	}
    	cout<<endl;
    


   //  for (int k = 1; k <= m[n]; k++){ // cada arvore formada
   //  	int *arestas  = A[n][k]; 
   //  	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
   //  	cout<<"Arvore "<<k<<endl;
   //  	for (int a = 0; a<nA; a++){ // cada aresta da arvore
			// if (arestas[a] == 1){
   //  			cout<<arestasPtr[a]->getOrigem() << " ";
   //  			cout<<arestasPtr[a]->getDestino() << " ";
   //  			cout<<arestasPtr[a]->getPeso1() << " ";
   //  			cout<<arestasPtr[a]->getPeso2() << endl;
   //  		}   
   //  	}
   //  	cout<<endl;
   //  }
	return 0;
}