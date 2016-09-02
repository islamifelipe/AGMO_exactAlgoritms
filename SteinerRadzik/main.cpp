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
#include <utility>      // std::pair
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
#include "margeSort.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

#define MAX1 20000 // TODO : Aumentar

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
int contAux = 0, contAux2=0;

bool isEgal(int *t1, int *t2, int size){
	for (int i=0; i<size; i++){
		if (t1[i]!=t2[i]) return false;
	}
	return true;
}

bool isEgalObjetive(float t1_peso1, float t1_peso2, float t2_peso1, float t2_peso2){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);
}
// bool t1_domina_t2(int *t1, int *t2){
// 	float t1_peso1=0, t1_peso2=0, t2_peso1=0, t2_peso2=0;
// 	for (int i=0; i<arestas.size(); i++){
// 		if (t1[i]==1){
// 			t1_peso1+=arestas[i]->getPeso1();
// 			t1_peso2+=arestas[i]->getPeso2();
// 		} 
// 		if (t2[i]==1){
// 			t2_peso1+=arestas[i]->getPeso1();
// 			t2_peso2+=arestas[i]->getPeso2();
// 		}
// 	}
// 	++contAux;
// 	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
// 		return true;
// 	} else return false;
// }

// void getXandY(int *t, float &X, float &Y ){
// 	++contAux2;
// 	X = 0; Y = 0;
// 	for (int i=0; i<arestas.size(); i++){
// 		if (t[i]==1){
// 			X+=arestas[i]->getPeso1();
// 			Y+=arestas[i]->getPeso2();
// 		} 
// 	}
// }

///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, float xl, float yl, float xll, float yll,int* Pa, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes){
	/*Pa = vetor de arestas = correspondente à partição P
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
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
		if (Pa[i]==1){
			a = P.getArestas(i);	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				//A2 = new Aresta*[P.getQuantVertices()-1];
				A2 = new int[m];
				//for(int mmm = 0; mmm<m; mmm++) A2[mmm] = 0;
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				float x, y;
				res = kruskal(&P1, arestasPtr,A2, x, y);
				custo +=x*(yl-yll)+y*(xll-xl);
			
				//map<int, pair<int*, list<Grafo>::iterator > > 
				if (res){
					vetorParticoes.push_back(P1);
					list<Grafo>::iterator itt = vetorParticoes.end();
					itt--;
					MSTs[idMST] = make_pair(make_pair(A2,make_pair( x, y)),itt);//A2;
					List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
					//vetorParticoes[idMST++] = P1;
				//	cout<<"size = "<<idMST<<endl;
				} else {
					delete[] A2;
				}
				P1.setStatus(a->getId(), 1); 
				//P1 = P2;
			}
		}
	}
}

int AllSpaningTree(Grafo *g,float xl, float yl, float xll, float yll, list< pair<int*, pair<float, float> > > &resul, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs, list <Grafo > &vetorParticoes){ 
	
			int id = List.getId();
			pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > par = MSTs[id];
				
			pair<int*, pair<float, float> > it = par.first;//MSTs[id];
			Grafo Ps = *par.second;
			
			List.extract();
			resul.push_back(it);
			//MSTs.erase(id);
			//vetorParticoes.erase(id);
			Partition(Ps,xl, yl, xll, yll, it.first, List,MSTs,vetorParticoes);
				

	
	//List.desaloca();
	return MSTs.size();
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)



void borderSearch(Grafo *g, list<pair<int*, pair<float, float> > > &resul, pair<int*, pair<float, float> > &sl, pair<int*, pair<float, float> > &sll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl.first;
	int *s2 = sll.first;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<float, float> > pilhaX;
	stack<pair<float, float> > pilhaY;
	stack<pair<int,list<pair<int*, pair<float, float> > >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	

	pilha.push(make_pair(s1, s2));
	pilhaX.push(make_pair(sl.second.first, sll.second.first)); //x's da primeira e sefunda solucao
	pilhaY.push(make_pair(sl.second.second, sll.second.second)); //y's da primeira e segunda solucao
	pilhaIt.push(make_pair(2, resul.begin()));


	while (pilha.size()!=0){
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list<pair<int*, pair<float, float> > >::iterator>  it = pilhaIt.top();
		pilhaIt.pop();

		pair<float, float> xs = pilhaX.top();
		pair<float, float> ys =  pilhaY.top();
		float xl = xs.first;
		float yl = ys.first;
		float xll = xs.second;
		float yll = ys.second;

		pilhaX.pop();
		pilhaY.pop();

		A2 = new int[g->getQuantArestas()];
		//for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
		float cont; // nao utilisazado nesse caso
		mergesort(xl, yl, xll, yll, arestasPtr, g->getQuantArestas(),3);
		float x, y;
		kruskal(g, arestasPtr,A2,x, y);
		if( !( (isEgalObjetive(x, y, xl, yl)) || (isEgalObjetive(x, y, xll, yll)) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, make_pair(A2, make_pair(x,y)));//A2); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, make_pair(A2, make_pair(x,y)));
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push(make_pair(A2, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilhaX.push(make_pair(x, xll));
			pilhaY.push(make_pair(y, yll));

			pilha.push(make_pair(s1, A2));  // L'
			pilhaIt.push(make_pair(1,it.second)); 
			pilhaX.push(make_pair(xl, x));
			pilhaY.push(make_pair(yl, y));
			
		}
	
	}
}


/*
#### Primeira fase ####
# A funcao phase1GM retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
list <pair<int*, pair<float, float> > > phase1GM(Grafo *g){
	list< pair<int*, pair<float, float> > > result;
	int *s1 = new int[g->getQuantArestas()];
	//for (int i=0; i<g->getQuantArestas(); i++) s1[i] = 0;
	float xr, yr; // nao utilisazado nesse caso
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),1);
	kruskal(g, arestasPtr,s1,xr, yr); // arvore para o primeiro objetivo
	 pair<int*, pair<float, float> > ps1 = make_pair(s1, make_pair(xr,yr));
	result.push_back(ps1);
	int* s2 = new int[g->getQuantArestas()];
	//for (int i=0; i<g->getQuantArestas(); i++) s2[i] = 0;
	mergesort(0, 0, 0, 0, arestasPtr, g->getQuantArestas(),2);
	float xr2, yr2;
	kruskal(g, arestasPtr, s2,xr2, yr2); // arvore para o segundo objetivo
	pair<int*, pair<float, float> > ps2 = make_pair(s2, make_pair(xr2,yr2));
//	list<int*>::iterator it = result.end();
	if (isEgalObjetive(xr, yr, xr2, yr2)==false){
		borderSearch(g, result, ps1, ps2);
		result.push_back(ps2);
	}
	return result;
 }
// /* 
// # retorna verdadeiro se o ponto kbest está dentro da regiao viável
// # se verdeiro, atualiza a regiao viável
// */	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y){
	bool retorno = false; // por default, o ponto nao está na regiao viavel 
	//This check is done by a simple linear search through the list of the consecutive corner points defining the viable region.
	//cout<<"\nkbest = ("<<x<<"'"<<y<<")"<<endl;
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 

		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			pair<float, float> ponto1 = make_pair(x, ponto_y);
			pair<float, float> ponto2 = make_pair(ponto_x, y);
			regiaoViavel.insert(it, ponto1);
			regiaoViavel.insert(it, ponto2);
			regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
			retorno = true;
			break; // se o ponto está regiao viavel, nao há mais rezao para percorrer-la 
		}
	}
	return retorno;
}

pair<float, float> getMaiorDistante(float a, float b, float c, list< pair<float, float> > regiaoViavel){
	pair<float, float>  pontoR = (*regiaoViavel.begin());
	float max = abs(a*pontoR.first + b*pontoR.second + c)/sqrt(a*a + b*b);
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it);
		float distancia = abs(a*ponto.first + b*ponto.second + c)/sqrt(a*a + b*b); // distância de ponto a r
 		if (distancia>max){
 			max = distancia;
 			pontoR = ponto;
 		}
	}
	return pontoR;
}
// /* 
// Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// Assim, teremos separadas as solucoes suportadas e nao suportadas
// */
list <pair<int*, pair<float, float> > >  phase2KB(Grafo *g, list< pair<int*, pair<float, float> > > extremas){
	list< pair<int*, pair<float, float> > > noSoportadas; // resultado a ser retornado
	list< pair<int*, pair<float, float> > >::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		pair<int*, pair<float, float> > pp = *(it);
		pair<int*, pair<float, float> > pq = *(++it);
		int *p = pp.first; 
		int *q = pq.first;
		float yp, yq, xp, xq;
		list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	
		xp = pp.second.first;
		yp = pp.second.second;
		xq = pq.second.first;
		yq = pq.second.second;
		//getXandY(p,xp, yp); // p
	 	//getXandY(q,xq, yq);	 //q
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		
		// determina a reta p-q (hipotenusa)
		float a, b; // pra determinar a equacao da reta p-q na forma ax+b = y
		a = (yp-yq)/(xp-xq); // coeficiente angular da reta p-q
		b = yq - a*xq; // coeficiente linear de p-q
		pair<float, float> maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
		//Agora determinamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
		//cout<<"Mais distante : "<<maisDistante.first<<", "<<maisDistante.second<<endl;
		float bM;
		bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y

		Heap List(10000); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
		map<int, pair<pair<int*, pair<float, float> >, list<Grafo>::iterator > > MSTs; // usada para lista de árvores
		idMST = 0;
		//cout<<"REDEFINIU"<<endl;
		int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
		//for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
		mergesort(xp,yp, xq,yq, arestasPtr, g->getQuantArestas(),3);
		float x, y;
		bool res = kruskal(g, arestasPtr, A,x, y);
		float custoMinimo = x*(yp-yq)+y*(xq-xp);
		
		if (res){
			vetorParticoes.push_back(*g);
			list<Grafo>::iterator itt = vetorParticoes.end();
			itt--;
			MSTs[idMST] = make_pair(make_pair(A, make_pair(x, y)),itt);//A2;
			List.insert(idMST++, custoMinimo); // o valor da variavel "custo" vem do kruskal
				
			
		}

		for (int k = 1; k<10000 && List.getSize()!=0; k++){
			list<pair<int*, pair<float, float> > > k_best_tree;
		
			AllSpaningTree(g,xp,yp, xq,yq, k_best_tree, List,MSTs, vetorParticoes);  // k-best
			pair<int*, pair<float, float> >  k_best = *(k_best_tree.begin());
			float x = k_best.second.first, y= k_best.second.second;

			if (isInViableRegion(g, regiaoViavel, x, y)){
				noSoportadas.push_back(k_best);
				maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
				//cout<<"Mais distante : "<<maisDistante.first<<", "<<maisDistante.second<<endl;
		
				//Agora atualizamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
				bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
			} else if ( y>=(a*x+bM)) { //s on or past maximum cost line 
				//cout<<"break"<<endl;
				break;
			}
		
		}
			
		
		contador++;
	}
	return noSoportadas;

}


int main(){
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	
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
	//my_grafo.gerarArestasPtr();
	
	arestas = my_grafo.get_allArestas();
	arestasPtr = my_grafo.getAllArestasPtr();

	 list <pair<int*, pair<float, float> > > arvores = phase1GM(&my_grafo);
	cout<<"Fim da primeira fase ... "<<endl;

	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);


	list <pair<int*, pair<float, float> > > noSuportadas = phase2KB(&my_grafo, arvores);
	

	cout<<"Fim da segunda fase ... "<<endl;

	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	cout<<user_time2<<endl;
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total ..."<<endl;
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;

	
    int i = 1, cont=0;
    //cout<<"saiu2"<<endl;
    //cout<<resul.size()<<endl;
   // list<int*>::iterator it=resul.begin();
   cout<<"Resultado \n SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=arvores.begin(); it!=arvores.end(); it++){
		
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if (((*it).first)[a] == 1){
				cout<<my_grafo.getArestas(a)->getOrigem() << " ";
    			cout<<my_grafo.getArestas(a)->getDestino() << " ";
    			cout<<my_grafo.getArestas(a)->getPeso1() << " ";
    			cout<<my_grafo.getArestas(a)->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}


	cout<<"Resultado \n NAO SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if (((*it).first)[a] == 1){
				cout<<my_grafo.getArestas(a)->getOrigem() << " ";
    			cout<<my_grafo.getArestas(a)->getDestino() << " ";
    			cout<<my_grafo.getArestas(a)->getPeso1() << " ";
    			cout<<my_grafo.getArestas(a)->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}

	// cout<<"Nao Suportada"<<endl;

	// for (list<int*>::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
	// 	cout<<"Arvore "<<i<<endl;
	// 	float cont1 = 0, cont2 = 0;
 //    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
	// 		if ((*it)[a] == 1){
	// 			cont1+=arestas[a]->getPeso1();
	// 			cont2+=arestas[a]->getPeso2();
				
 //    			cout<<arestas[a]->getOrigem() << " ";
 //    			cout<<arestas[a]->getDestino() << " ";
 //    			cout<<arestas[a]->getPeso1() << " ";
 //    			cout<<arestas[a]->getPeso2() << endl;
 //    		}
 //    	}
 //    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
 //    	i++;
	// }
	//cout<<"pega valor = "<<contAux2<<endl;
	//cout<<"Dominancia ou igualdade = "<<contAux<<endl;
	return 0;
}