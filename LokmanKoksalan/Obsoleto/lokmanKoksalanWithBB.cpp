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
#include "Conjunto.h"
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

bool atendeCriterios(int size, int *solucao, map<int, float*> criterios){
	for (int i=0; i<criterios.size(); i++){ // para cada critério
		float sum = 0.0;
		for (int j =0; j<size-1; j++){ // para cada variavel (aresta) 
			sum += solucao[j]*criterios[i][j]; 
		}
		if (!(sum >= criterios[i][size-1])) return false; // esse >= é padrao para o caso particular da AGM-bi e para todos os critérios adcionados a cada iteracao (segundo Lokman and Koksalan)
	}
	return true;
}

/*
retorna a solucao que *** maximiza *** o modelo pnm
como nosso problema é bi-objetivo, com m=2, 
entao para j=1 teremos sempre os memos coneficientes e o mesmo sinal de inequacao (>=) nos critérios
Logo, é suficiente concervar o maior valor do lado direto da inequacao, mantendo sempre somente um critério 
esta funcao pressupoe que o lado direto da inequacao (criterio[|E|]) é o maior já registrado 
*/
pair <bool, int *>branchAndBound(Grafo &g, vector< pair<int,float> >  criterio, vector< pair<int,float> > objetivo, float LB, float cUB){
	/* IMPORTANTE : 
	Lokman and Koksalan trantam do problema de maximizacao. 
	Como a AGM-Bi é de minimizacao, esta funcao BB supõe que os coeficientes da funcao objetivo foram todos multiplicado por -1
	*/
	/*	LB = limite infeior do objetivo = melhor solucao encontrada até o momento para a maximizacao. É o valor do obejtivo paro o Krukal que maximiza o criterio 
		UB = limite superior do objetivo = Construído a cada nó da arvore de busca. Pega a soma das arestas incluidas mais as opcionais de maiores valores. Esse valor é o maior possivel em que podemos chegar. Ou seja, UB. Caso UB seja menor ou igual à LB (ou seja, o melhor que podemos fazer é ainda menor que o que ja temos, podamos a arvore)
	   	cLB = limite infeior do critério = criterio[|E|]
	   	cUB = limite superior do critério = construido a cada nó da arvore de busca. Pega a soma das arestas incluidas mais as opcionais de maiores coeficientes nos critérios. Esse valor é o maior possivel em que podemos chegar. Ou seja, cUB. Caso cUB seja menor ou igual à cLB (ou seja, o melhor que podemos fazer é ainda menor que o que ja temos, podamos a arvore)	
	*/
	float UB = 0;
	cout<<"LB = "<<LB<<endl;
	stack< pair<int, int* > > pilha; 
	// para simular a recursao : o int é a quantidade de variavais em 1
	// o vetor int* (de comprimeito |E|) é composto por 1's ou 0's para indicar a escolha ou nao da aresta de id = i  

	stack< pair<int, float > > nivelUB; // int = nivel da arvore de busca; float : valor de UB
	
	stack< Conjunto > ciclo; // cada (sub)arvore terá um registro dos seus vertices para que possamos verificar os ciclos  
	Conjunto conjunto(g.getQuantVertices());
	int *melhorSolucao = new int[g.getQuantArestas()];
	int *sol = new int[g.getQuantArestas()];
	for (int j=0; j<g.getQuantArestas(); j++) {
		melhorSolucao[j] = 0;
		sol[j] = 0;
	}
	pilha.push(make_pair(0, sol));
	nivelUB.push(make_pair(0,UB));
	ciclo.push(conjunto);
	bool ok = false;
	int contttttt = 1;
	while (pilha.size()!=0){
		//cout<<contttttt++<<endl;
		pair<int, float > nivelUBp = nivelUB.top();
		nivelUB.pop();
		int nivel = nivelUBp.first;
		float UBaux = nivelUBp.second;
		pair<int, int* > s = pilha.top();
		pilha.pop();
		int sizeSolucao = s.first;
		int* solucao = s.second;
		conjunto = ciclo.top();
		ciclo.pop();
		

		float maxCriterio = 0.0;
		for (int j=0; j<g.getQuantArestas(); j++) { // para cada aresta
			if (solucao[criterio[j].first] == 1){ // se a aresta estiver inserida
				maxCriterio+=criterio[j].second;
			}
		}

		/*estimando UB : pega os maiores coeficientes de cuja variavais sao diferente de 1*/
		int cont=sizeSolucao;
		int cont2=sizeSolucao;
		float UBestimado = UBaux;
		for (int j=0; j<g.getQuantArestas(); j++) { // para cada aresta
			if (cont<g.getQuantVertices()-1 && solucao[objetivo[j].first] != 1){ // se a aresta nao estiver inserida
				UBestimado+=objetivo[j].second;
				cont++;
			}
			if (cont2<g.getQuantVertices()-1 && solucao[criterio[j].first] != 1) {
				maxCriterio+=criterio[j].second;
				cont2++;
			}
		}
		

		if (UBestimado>LB && maxCriterio>=criterio[g.getQuantArestas()].second){ // senao, poda..
			if (sizeSolucao == g.getQuantVertices()-1){ // solucao otima por momento
				LB = UBestimado;
				for (int j=0; j<g.getQuantArestas(); j++) melhorSolucao[j] = solucao[j];
				ok = true;
			} else{
				if (((g.getQuantArestas()-nivel)>=g.getQuantVertices()-sizeSolucao-1)){
				//if ( nivel<g.getQuantArestas()) { // veficiamos se os criterios sao atendiddos
					
					/* a cada iteracao, verificamos a aresta que tem id = objetivo[nivel].first*/
					int *avec = new int[g.getQuantArestas()];
					int *sans = new int[g.getQuantArestas()];
					float coef = objetivo[nivel].second;
					for (int j=0; j<g.getQuantArestas(); j++){
						//if (objetivo[j].first == nivel)  coef = objetivo[j].second;
						
						avec[j] = solucao[j];
						sans[j] = solucao[j];
					}


					sans[objetivo[nivel].first] = 0;
					nivelUB.push(make_pair(nivel+1, UBaux)); // sans
					pilha.push(make_pair(sizeSolucao, sans)); //sans
					Conjunto c_avec(g.getQuantVertices());
					c_avec = conjunto;
					ciclo.push(conjunto);

					if (!c_avec.compare(g.get_allArestas()[objetivo[nivel].first]->getOrigem(),g.get_allArestas()[objetivo[nivel].first]->getDestino())){
						
						avec[objetivo[nivel].first] = 1;
						c_avec.union1(g.get_allArestas()[objetivo[nivel].first]->getOrigem(),g.get_allArestas()[objetivo[nivel].first]->getDestino());
						ciclo.push(c_avec);
						nivelUB.push(make_pair(nivel+1, UBaux+coef)); // avec
						pilha.push(make_pair(sizeSolucao+1, avec)); // avec

					}

				}
				 // se nao atende aos critérios, o presente nó de busca nao é expandido
			}
		}
		//conjunto.desaloca();
		delete[] solucao;
	}
	return make_pair(ok, melhorSolucao);
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


/* 
* Algoritmo 1 de Lokman and Koksalan 	
O vetor de coefeficientes do critério (de tamanho g.getQuantArestas()+1) já deve vir ordenado em ordem decrescente 
***** (exceto o ultimo) *****
*/
vector <int *> algorithm1(Grafo &g, vector< pair<int,float> >  criterio, vector< pair<int,float> > objetivo){
	vector <int *> resul;

	map <int, Aresta *> arestas = g.get_allArestas();
	pair<bool, int *> sol;
	int * ultima = new int [g.getQuantArestas()];
	int * primeira = new int [g.getQuantArestas()];
	Conjunto conjunto(g.getQuantVertices());
	int cont=0;
	float cUB = 0.0;
	float LB = 0.0;
	for (int i=0; i<g.getQuantArestas(); i++){ // percorre os primeirps g.getQuantArestas() elementos, deixando somente o ultimo de lado
		if (cont<g.getQuantVertices()-1){
			if (!conjunto.compare(arestas[criterio[i].first]->getOrigem(),arestas[criterio[i].first]->getDestino())){
				conjunto.union1(arestas[criterio[i].first]->getOrigem(),arestas[criterio[i].first]->getDestino());
				ultima[criterio[i].first] = 1;
				cont++;
				cUB += criterio[i].second;
			} else ultima[criterio[i].first]=0;
		} else ultima[criterio[i].first]=0;
	}
	for (int i=0; i<g.getQuantArestas(); i++){ 
		if (ultima[objetivo[i].first]==1) LB += objetivo[i].second;
	}
	// Pm0 : primeira solucao : objetivos ordenados em ordem decrescente
	cont = 0;
	float sum = 1;
	Conjunto conjunto2(g.getQuantVertices());
	for (int i=0; i<g.getQuantArestas(); i++){ // percorre os primeirps g.getQuantArestas() elementos, deixando somente o ultimo de lado
		if (cont<g.getQuantVertices()-1){
			if (!conjunto2.compare(arestas[objetivo[i].first]->getOrigem(),arestas[objetivo[i].first]->getDestino())){
				conjunto2.union1(arestas[objetivo[i].first]->getOrigem(),arestas[objetivo[i].first]->getDestino());
				primeira[objetivo[i].first] = 1;
				cont++;
				sum+=((arestas[objetivo[i].first]->getPeso1())*(-1));
			} else primeira[objetivo[i].first]=0;
		} else primeira[objetivo[i].first]=0;
	}
	criterio.push_back(make_pair(-1,sum)); // primeiro critério 
	cout<<"sum = "<<sum<<endl;
	resul.push_back(primeira);//primeira solucao

	do{
		cout<<"branchAndBound"<<endl;
		sol = branchAndBound(g, criterio, objetivo, LB, cUB);
		
		if (sol.first == true){
			resul.push_back(sol.second);

			sum = 1;
			for (int j=0; j<g.getQuantArestas(); j++) sum+=((arestas[j]->getPeso1()*sol.second[j])*(-1));
			cout<<"sum = "<<sum<<endl;
			if (criterio.size() == g.getQuantArestas()+1) {
				if (criterio[g.getQuantArestas()].second < sum ) criterio[g.getQuantArestas()].second = sum; // conserva sempre o maior ztj
			} else criterio.push_back(make_pair(-1,sum));
			// for (int i=0; i<criterios.size(); i++){ // para cada critério
			// 	cout<<i<<" = "<<criterios[i]<<endl;
			// 	for (int j=0; j<g.getQuantArestas(); j++){ // para cada variavel (aresta) 
			// 		cout<<criterios[i][j]<<" ";
			// 	}
			// 	cout<<criterios[i][g.getQuantArestas()]<<endl;
			// }
			// cout<<endl;

		}
	}while(sol.first == true);
	resul.push_back(ultima);
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