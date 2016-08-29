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
#include <sys/times.h>
#include <unistd.h>
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

void Partition(Grafo P, float xl, float yl, float xll, float yll, int *Pa, Heap &List, list <int *> &MSTs, int &cont, list<Grafo> &vetorParticoes){
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
	//for (int i=0; i<P.getQuantVertices()-1; i++){
	
	for (int i=0; i<allArestas.size(); i++){
		if (Pa[i]==1){
			a = allArestas[i];	
			if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
				//A2 = new Aresta*[P.getQuantVertices()-1];
				A2 = new int[allArestas.size()];
				for(int mmm = 0; mmm<allArestas.size(); mmm++) A2[mmm] = 0;
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				
				custo=0;//cout<<"Antes1"<<endl;
				res = kruskal(&P1, A2, xl, yl, xll, yll,custo, 3);

				cont++; 
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

int AllSpaningTree(Grafo *g,float xl, float yl, float xll, float yll, list<int*> &resul, int &cont, Heap &List, list<int*> &MSTs, list<Grafo> &vetorParticoes){ 
	//int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
	//for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
				
	//list<int*> MSTs; // usada para lista de árvores
	//float custoMinimo =0;
	//Heap List(40000); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	//list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	//A estrutura "List" do algortimo original é um heap chamada List
	// bool res = kruskal(g, A, xl, yl, xll, yll, custoMinimo, 3);
	cont =1;
	// if (res){
	// 	//cout<<"custoMinimo : "<<custoMinimo<<endl;
	// 	int contMSTs = MSTs.size();
	// 	MSTs.push_back(A);
	// 	List.insert(contMSTs, custoMinimo);
	// 	vetorParticoes.push_back(*g);
	// }
	//do{ //List.getChave()<=custoMinimo
		//if (List.getSize()>0){
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
			
			Grafo Ps = *itg;
			//cout<<"Size : "<<List.getSize()<<endl;
			
			List.extract();
			//cout<<"Size : "<<List.getSize()<<endl;
			// if (id==0) {
			// 	float x, y;
			// 	getXandY(*it, g->get_allArestas(),x, y); 
			// 	//cout<<"xl = "<<xl<<" yl = "<<yl<<" xll = "<<xll<<" yll = "<<yll<<endl;
			// 	//cout<<"FFJFFFFFFFF ("<<x<<", "<<y<<") = "<<x*(yl-yll)+y*(xll-xl)<<endl;;
		
			// }
			//if (k_best>0){
				resul.push_back(*it);
				Partition(Ps,xl, yl, xll, yll, *it, List,MSTs, cont,vetorParticoes);
				
			//}
			//k_best--;
			
		//}
	//}while (List.getSize()>0);
	//cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
	//cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
	//cout<<contMSTs<<endl;
	/*for (int i=0; i<vetorParticoes.size(); i++){ TODO
		
			vetorParticoes->remove(vetorParticoes->getInit());
			ElementArvore *initArvore = MSTs->getInit();
			MSTs->remove(MSTs->getInit());
			delete initArvore;
	}*/
	
	//List.desaloca();
	return MSTs.size();
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)



void borderSearch(Grafo *g, list<int*> &resul, int * sl, int *sll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<int,list<int*>::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	float xl, yl, xll, yll;

	pilha.push(make_pair(s1, s2));
	pilhaIt.push(make_pair(2, resul.begin()));


	while (pilha.size()!=0){
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list<int*>::iterator> it = pilhaIt.top();
		pilhaIt.pop();

		getXandY(s1, g->get_allArestas(), xl, yl);
		getXandY(s2, g->get_allArestas(), xll, yll);
		A2 = new int[g->getQuantArestas()];
		for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
		float cont; // nao utilisazado nesse caso
		kruskal(g, A2, xl, yl, xll, yll,cont, 3);
		//cout<<pilha.size()<<endl;
		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, A2); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, A2);
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push(make_pair(A2, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilha.push(make_pair(s1, A2));  // L'
			pilhaIt.push(make_pair(1,it.second)); 
			
		}
	
	}
}



// void borderSearch(Grafo *g, list<int*> &resul, int * sl, int *sll){ 
// 	/* it = interator da lista
// 	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
// 	* sl = s'
// 	* sll = s''
// 	**/
// 	int *s1 = sl;
// 	int *s2 = sll;
// 	int * A2;
// 	stack<int* >  pilha;
// 	bool avanca = true;
// 	float xl, yl, xll, yll;
// 	do{
// 		getXandY(s1, g->get_allArestas(), xl, yl);
// 		getXandY(s2, g->get_allArestas(), xll, yll);
// 		A2 = new int[g->getQuantArestas()];
// 		for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
// 		float cont; // nao utilisazado nesse caso
// 		kruskal(g, A2, xl, yl, xll, yll,cont, 3);
// 		//cout<<pilha.size()<<endl;
// 		if( !( (isEgalObjetive(A2, s1, g->get_allArestas())) || (isEgalObjetive(A2, s2, g->get_allArestas())) ) ){
// 			pilha.push(s2);
// 			s2 = A2;
// 			avanca = true;
// 		} else {
// 			if (pilha.size()==0){ //se pilha está fazia
// 				avanca = false;
// 			} else {
// 				avanca = true;
// 				s1 = s2;
// 				resul.push_back(s2);
// 				s2 = pilha.top();
// 				pilha.pop();
// 				//if (pilha.size()==0) avanca = false;
// 			}
// 			//delete[] A2;
// 		}	
// 	} while (avanca);
// }


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
	for (int i=0; i<g->getQuantArestas(); i++) s1[i] = 0;
	float cont; // nao utilisazado nesse caso
	kruskal(g, s1, 0, 0, 0, 0,cont, 1); // arvore para o primeiro objetivo
	result.push_back(s1);
	int* s2 = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) s2[i] = 0;
	kruskal(g, s2, 0, 0, 0, 0,cont, 2); // arvore para o segundo objetivo
	list<int*>::iterator it = result.end();
	if (isEgalObjetive(s1, s2,g->get_allArestas())==false){
		borderSearch(g, result, s1, s2);
		result.push_back(s2);
	}
	return result;
}
/* 
# retorna verdadeiro se o ponto kbest está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
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
/* 
Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
Assim, teremos separadas as solucoes suportadas e nao suportadas
*/
list<int*> phase2KB(Grafo *g, list<int*> extremas){
	list<int*> noSoportadas; // resultado a ser retornado
	list<int*>::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		int *p = *(it); 
		int *q = *(++it);
		float yp, yq, xp, xq;
		list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	
		getXandY(p, g->get_allArestas(),xp, yp); // p
	 	getXandY(q, g->get_allArestas(), xq, yq);	 //q
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

		int contMST=0;  /*Futuramente necessários para dados estatísticos*/
		
		Heap List(40000); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
		list<int*> MSTs; // usada para lista de árvores
		int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
		for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
		
		float custoMinimo = 0;
		//int cont =1;
		/**init **/
		bool res = kruskal(g, A, xp,yp, xq,yq, custoMinimo, 3);
		
		
		if (res){
			//cout<<"custoMinimo : "<<custoMinimo<<endl;
			int contMSTs = MSTs.size();
			MSTs.push_back(A);
			List.insert(contMSTs, custoMinimo);
			vetorParticoes.push_back(*g);
		}

		for (int k = 1; k<30000 && List.getSize()!=0; k++){
			list<int*> k_best_tree;
		
			AllSpaningTree(g,xp,yp, xq,yq, k_best_tree, contMST, List,MSTs, vetorParticoes);  // k-best
			int* k_best = *(k_best_tree.begin());
			float x, y;
			getXandY(k_best, g->get_allArestas(),x, y); 

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
			
		// for (list<int*>::iterator kb_it = k_best_tree.begin(); kb_it!=k_best_tree.end();  kb_it++){
		// 	int* k_best = *kb_it;
		// 	float x, y;
		// 	getXandY(k_best, g->get_allArestas(),x, y); 
		// 	//cout<<"("<<x<<", "<<y<<") = "<<x*(yp-yq)+y*(xq-xp)<<endl;;
		// }
		// for (list<int*>::iterator kb_it = k_best_tree.begin(); kb_it!=k_best_tree.end();  kb_it++){
		// 	int* k_best = *kb_it;
		// 	float x, y;
		// 	getXandY(k_best, g->get_allArestas(),x, y); 
		// 	if (isInViableRegion(g, regiaoViavel, x, y)){
		// 		noSoportadas.push_back(k_best);
		// 		maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
		// 		//cout<<"Mais distante : "<<maisDistante.first<<", "<<maisDistante.second<<endl;
		
		// 		//Agora atualizamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
		// 		bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
		// 	} else if ( y>=(a*x+bM)) { //s on or past maximum cost line 
		// 		//cout<<"break"<<endl;
		// 		break;
		// 	}
		// }
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
	my_grafo.gerarArestasPtr();
	

	list<int*> arvores = phase1GM(&my_grafo);
	cout<<"Fim da primeira fase ... "<<endl;

	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	times(&tempsInit);


	list<int*> noSuportadas = phase2KB(&my_grafo, arvores);
	

	cout<<"Fim da segunda fase ... "<<endl;

	times(&tempsFinal2);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	cout<<user_time2<<endl;
	cout<<(float) user_time2 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	cout<<"Total ..."<<endl;
	cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;


	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    
    
    int i = 1, cont=0;
    //cout<<"saiu2"<<endl;
    //cout<<resul.size()<<endl;
   // list<int*>::iterator it=resul.begin();
   cout<<"Resultado \n SUPORTADAS"<<endl;
    for (list<int*>::iterator it=arvores.begin(); it!=arvores.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
    	i++;
	}

	cout<<"Nao Suportada"<<endl;

	for (list<int*>::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		cout<<"Arvore "<<i<<endl;
		float cont1 = 0, cont2 = 0;
    	for (int a = 0; a<nA; a++){ // cada aresta da arvore
		
			if ((*it)[a] == 1){
				cont1+=arestasPtr[a]->getPeso1();
				cont2+=arestasPtr[a]->getPeso2();
				
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<cont1<<", "<<cont2<<")\n"<<endl;
    	i++;
	}
	return 0;
}