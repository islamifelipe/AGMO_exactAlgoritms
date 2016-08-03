/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Steiner and Radzik's (2003) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# This code utilise the SONRENSEN JANSSENS's (2003) algorithm
# to calculate the k-best tree
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
using namespace std;

#define MAX1 20000 // TODO : Aumentar

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

bool isEgalObjetive(int *t1, int *t2, map <int, Aresta *> arestas){
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
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);

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

void getXandY(int *t, map <int, Aresta *> arestas, float &X, float &Y ){
	X = 0; Y = 0;
	for (int i=0; i<arestas.size(); i++){
		if (t[i]==1){
			X+=arestas[i]->getPeso1();
			Y+=arestas[i]->getPeso2();
		} 
	}
}

///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, float xl, float yl, float xll, float yll, int *Pa, Heap &List, list <int *> MSTs, int &cont, list<Grafo> vetorParticoes){
	/*Pa = vetor de arestas = correspondente à partição P
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P, P2 = P;
	bool res = false;
	float custo, x, y;
	Aresta *a; 
	int *A2;
	map <int, Aresta *> allArestas = P.get_allArestas();
	for (int i=0; i<P.getQuantVertices()-1; i++){
		a = allArestas[Pa[i]];	
		if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
			//A2 = new Aresta*[P.getQuantVertices()-1];
			A2 = new int[allArestas.size()];
			custo=0;
			kruskal(&P1, A2, xl, yl, xll, yll,custo, 3);
			cont++; // estatístico
			P1.setStatus(a->getId(), 2); /*proibida*/
			P2.setStatus(a->getId(), 1); /*obrigatória*/
			
			if (res){
				int quantMSTs = MSTs.size();
				MSTs.push_back(A2);
				List.insert(quantMSTs, custo); // o valor da variavel "custo" vem do kruskal
				vetorParticoes.push_back(P1);
			} else {
				delete[] A2;
			}
			P1 = P2;
		}
	
	}
}

// void AllSpaningTree(Grafo *g, ListaArvores *resul, int &contMSTs, int &cont, int k_best){ 
// 	Aresta **A = new Aresta*[g->getN()-1]; // usada para a primeira árvore 
// 	ListaArvores *MSTs = new ListaArvores(); // usada para lista de árvores
// 	contMSTs=0;
// 	float custoMinimo =0, x=0, y=0;
// 	Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
// 	ListaGrafos *vetorParticoes = new ListaGrafos();//Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
// 	//A estrutura "List" do algortimo original é um heap chamada List
// 	bool res = kruskal(g, A, x, y, custoMinimo, 3);
// 	cont =1;
// 	if (res){
// 		MSTs->insert(A, x, y);
// 		List.insert(contMSTs, custoMinimo);
// 		vetorParticoes->insert(*g);
// 		contMSTs++;
// 	}
// 	do{ //List.getChave()<=custoMinimo
// 		if (List.getSize()>0){
// 			int id = List.getId();
// 			Grafo Ps;
// 			ElementGrafo *init = vetorParticoes->getInit();
// 			ElementArvore *initArvore = MSTs->getInit();
// 			for (int k=0; k<id; k++){ // pega o id-ésimo elemento de vetorPartições
// 				init = init->getNext();
// 				initArvore = initArvore->getNext();
// 			}
// 			Ps = init->getGrafo();
// 			k_best--;
// 			List.extract();
// 			if (k_best>0){
// 				Partition(Ps, initArvore->getArvore(), List,MSTs, contMSTs, cont,vetorParticoes);
// 				resul->insert(initArvore->getArvore(), initArvore->getX(), initArvore->getY());
// 			}
// 		}
// 	}while (List.getSize()>0);
// 	//cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
// 	//cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
// 	//cout<<contMSTs<<endl;
// 	for (int i=0; i<contMSTs; i++){
		
// 			vetorParticoes->remove(vetorParticoes->getInit());
// 			ElementArvore *initArvore = MSTs->getInit();
// 			MSTs->remove(MSTs->getInit());
// 			delete initArvore;
// 	}
	
// 	List.desaloca();
// 	delete MSTs;
// 	delete vetorParticoes;
// }


// /// END ALGORITMO DA SONRENSEN JANSSENS (2003)


void borderSearch(Grafo *g, list<int*> &resul, int * sl, int *sll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<int* >  pilha;
	bool avanca = true;
	float xl, yl, xll, yll;
	do{
		getXandY(s1, g->get_allArestas(), xl, yl);
		getXandY(s2, g->get_allArestas(), xll, yll);
		A2 = new int[g->getQuantArestas()];
		float cont; // nao utilisazado nesse caso
		kruskal(g, A2, xl, yl, xll, yll,cont, 3);
		
		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
			pilha.push(s2);
			s2 = A2;
			avanca = true;
		} else {
			if (pilha.size()==0){ //se pilha está fazia
				avanca = false;
			} else {
				avanca = true;
				s1 = s2;
				resul.push_back(s2);
				s2 = pilha.top();
				pilha.pop();
			}
			//delete[] A2;
		}	
	} while (avanca);
}


/*
#### Primeira fase ####
# A funcao phase1GM retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list<int*> phase1GM(Grafo *g){
	list<int*> result;
	int *s1 = new int[g->getQuantArestas()];
	float cont; // nao utilisazado nesse caso
	kruskal(g, s1, 0, 0, 0, 0,cont, 1); // arvore para o primeiro objetivo
	result.push_back(s1);
	int* s2 = new int[g->getQuantArestas()];
	kruskal(g, s2, 0, 0, 0, 0,cont, 2); // arvore para o segundo objetivo
	list<int*>::iterator it = result.end();
	if (isEgalObjetive(s1, s2,g->get_allArestas())==false){
		borderSearch(g, result, s1, s2);
		result.push_back(s2);
	}
	return result;
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
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}

	int nA = id; // quantidade de arestas do grafo	
	my_grafo.gerarArestasPtr();
	

	list<int*> arvores = phase1GM(&my_grafo);
	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    int i = 1;
    for (list<int*>::iterator it=arvores.begin(); it!=arvores.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<endl;
    	i++;
	}
	return 0;
}