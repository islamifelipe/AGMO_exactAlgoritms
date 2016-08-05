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
	cout<<List.getSize()<<endl;
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
				P1.setStatus(a->getId(), 2); /*proibida*/
				P2.setStatus(a->getId(), 1); /*obrigatória*/
				
				//custo=0;cout<<"Antes1"<<endl;
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

int AllSpaningTree(Grafo *g,float xl, float yl, float xll, float yll, list<int*> &resul, int &cont, int k_best){ 
	int *A = new int[g->getQuantArestas()]; // usada para a primeira árvore 
	list<int*> MSTs; // usada para lista de árvores
	float custoMinimo =0, x=0, y=0;
	Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	//A estrutura "List" do algortimo original é um heap chamada List
	bool res = kruskal(g, A, xl, yl, xll, yll, custoMinimo, 3);
	cont =1;
	if (res){
		int contMSTs = MSTs.size();
		MSTs.push_back(A);
		List.insert(contMSTs, custoMinimo);
		vetorParticoes.push_back(*g);
	}
	do{ //List.getChave()<=custoMinimo
		if (List.getSize()>0){
			int id = List.getId();
			//ElementGrafo *init = vetorParticoes->getInit();
			//ElementArvore *initArvore = MSTs->getInit();
			list<int*>::iterator it;
			list<Grafo>::iterator itg = vetorParticoes.begin();
			int k=0;
			for (it=MSTs.begin(); k<id; ++it){
				k++;
				itg++;
			}
			Grafo Ps = *itg;
			//cout<<"Size : "<<List.getSize()<<endl;
			
			List.extract();
			//cout<<"Size : "<<List.getSize()<<endl;
			
			if (k_best>0){
				Partition(Ps,xl, yl, xll, yll, *it, List,MSTs, cont,vetorParticoes);
				resul.push_back(*it);
			}
			k_best--;
			
		}
	}while (List.getSize()>0);
	//cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
	//cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
	//cout<<contMSTs<<endl;
	/*for (int i=0; i<vetorParticoes.size(); i++){ TODO
		
			vetorParticoes->remove(vetorParticoes->getInit());
			ElementArvore *initArvore = MSTs->getInit();
			MSTs->remove(MSTs->getInit());
			delete initArvore;
	}*/
	
	List.desaloca();
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
/* 
# retorna verdadeiro se o ponto kbest está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, int* k_best ){
	bool retorno = false; // por default, o ponto nao está na regiao viavel 
	float x, y;
	getXandY(k_best, g->get_allArestas(),x, y); 
	//This check is done by a simple linear search through the list of the consecutive corner points defining the viable region.
	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 
		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		//cout<<"kbest = ("<<x<<"'"<<y<<")"<<endl;
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

/* 
Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
Assim, teremos separadas as solucoes suportadas e nao suportadas
*/
list<int*> phase2KB(Grafo *g, list<int*> extremas){
	list<int*> noSoportadas; // resultado a ser retornado
	list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	list<int*>::iterator it = extremas.begin(); 
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		int *p = *(it); 
		int *q = *(++it);
		float yp, yq, xp, xq;
		getXandY(p, g->get_allArestas(),xp, yp); // p
	 	getXandY(q, g->get_allArestas(), xq, yq);	 //q
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		list<int*> k_best_tree;
		int contMST=0;  /*Futuramente necessários para dados estatísticos*/

		AllSpaningTree(g,xp,yp, xq,yq, k_best_tree, contMST, 100);  // k-best
		
		for (list<int*>::iterator kb_it = k_best_tree.begin(); kb_it!=k_best_tree.end();  kb_it++){
			int* k_best = *kb_it;
			if (isInViableRegion(g, regiaoViavel, k_best)){
		
				noSoportadas.push_back(k_best);
			} else {
				//cout<<"saiu222222"<<endl;
				//break;
			}
		}
		contador++;
	}
	return noSoportadas;

}

// //LEMBRAR: Para separar em suportadas e não suportadas, colocar uma variável bool em ElementArvore


// void phase2KB(Grafo *g, list<int*> &resul){
// 	list<int*>::iterator it = resul.begin(); // deve apontar sempre para p
// 	int *p = *(it); /*Ponteiro que percorre a lista de soluções suportadas*/
// 	int *q = *(++it);
// 	it--;

// 	list<int*>::iterator aux, anti, varre;
// 	list<int*> k_best_tree;
// 	int k; /*k-ésima melhor árvore*/
// 	int cont=0, contMST=0;  /*Futuramente necessários para dados estatísticos*/
// 	float a, b; /*Necessário para construir a equação da reta de custo máximo, da forma ax + b = y*/
// 	float br, cr;
// 	float m; /*coeficiente angular da reta r*/
// 	float distancia; /*representa a distância de um ponto e a reta r (custo máximo)*/
// 	bool otima, pare;
// 	/*!
//   	 * p e q definem um trigângulo. Todas as ávores não-suportadas encontradas são inseridas entre p e q na 	 * lista 'resul'. Para a sub-lista SL = 'p .... q', encontramos a região viável a partir da definição
//   	 * dos triângulos formados entre dois elementos consecutivos u e v de SL. Tais triângulos pelos pontos 
//   	 * u=(ux, uy), v=(vx, vy) e (vx, uy); 
// 	 */
// 	//while (p->getNext()!=resul->getInit()){
//   	 float yl, yll, xl, xll;
// 	while (q!=(*resul.begin())){
// 		//q = p->getNext();
		
// 		getXandY(p, g->get_allArestas(), xl, yl);
// 		getXandY(q, g->get_allArestas(), xll, yll);	
		
// 		k=1;
// 		/* Inicialmente a região viável é o próprio triângulo entre p e q
// 		 * A reta de custo máximo, inicialmente, passará pelos pontos (qx, py)
// 		 */
// 		m = (yl-yll)/(xl-xll); // coeficiente angular da reta de custo máximo
// 		b = yl - xll*m; // a reta de custo máximo é paralela à r (entre p e q)
// 		a = m; // ax + b = y (reta de custo máximo);
		
// 		br = -1;
// 		cr = yll - m*xll; /*Coefieciente lienar da reta r que une p e q*/
// 		distancia = abs(a*xll + br*yl + cr)/sqrt(a*a + br*br); // distância de (qx, py) a r
// 		pare = false; // false = não para o laço
		
// 		/*! OBS.: O valor passado em AllSpaningTree (útimo parâmetro) é estimado, de modo a diminuir a complexidade do algortimo. LEMBRAR: fazer muitos testes, estimá-lo a partir de k e do número de vértices de G*/
// 		//k_best_tree = new ListaArvores();
// 		Grafo gg = *g;	
		
// 		//AllSpaningTree(g, k_best_tree,contMST, cont, 100); 
// 		cout<<"testeeee"<<endl;
// 		AllSpaningTree(g,xl,yl, xll,yll, k_best_tree, contMST, 100);
	
// 		*g = gg;
// 		varre = k_best_tree.begin();	
// 		do{ 
// 			if (pare == false){
// 				/*! Testes de dominância*/
// 				/*! Verifica se a solução corrent pertence à região viável*/
// 				aux = it;
// 				anti = it;
// 				otima = false; // false = árvore não é ótima
// 				float dis, auxb;
// 				float maxDistance =0;
// 				while ((*aux)!=q){ //varre todos os triângulos
// 					float vx, vy, auxx1, auxy1, auxx, auxy;
// 					getXandY(*varre, g->get_allArestas(), vx, vy);
// 					getXandY(*(++aux), g->get_allArestas(), auxx1, auxy1);
// 					aux--;
// 					getXandY(*(aux), g->get_allArestas(), auxx, auxy);
// 					//if (!maiorIgualQuefloat(varre->getX(), aux->getNext()->getX()) && !maiorIgualQuefloat(varre->getY(), aux->getY())){
						
// 					if (!maiorIgualQuefloat(vx, auxx1) && !maiorIgualQuefloat(vy, auxy)){
// 						otima=true;
// 						anti = aux;
// 					}
// 					//dis = abs(a*aux->getNext()->getX() + br*aux->getY() + cr)/sqrt(a*a + br*br);
// 					dis = abs(a*auxx1 + br*auxy + cr)/sqrt(a*a + br*br);
// 					if (dis>maxDistance){	
// 						maxDistance=dis;
// 						auxb = auxy - a*auxx1;
// 						//auxb = aux->getY() - a*aux->getNext()->getX();
// 					}
// 					aux++;
// 				}
// 				if (otima){ //se a solução estiver na região viável
					
// 					aux = --varre;
// 					varre++;
// 					k_best_tree.remove(*varre);

// 					/*varre->setNext(anti->getNext());
// 					anti->getNext()->setPrevious(varre);
// 					anti->setNext(varre);
// 					varre->setPrevious(anti);
// 					quantArvores++;*/
				
// 					if (resul.end()==anti) resul.push_back(*varre);
// 					else {

// 						resul.insert(++anti, *varre);
// 						anti--;
// 					}
					
// 					/*! Nova linha de custo máximo*/
// 					float antix1, antiy1, antix, antiy,vx1, vy1, auxx1, auxy1, auxx, auxy,vx, vy;
// 					getXandY(*(++anti), g->get_allArestas(), antix1, antiy1);
// 					anti--;
// 					getXandY(*anti, g->get_allArestas(), antix, antiy);
// 					getXandY(*(++varre), g->get_allArestas(), vx1, vy1);
// 					varre--;
// 					getXandY(*(++aux), g->get_allArestas(), auxx1, auxy1);
// 					aux--;
// 					getXandY(*(aux), g->get_allArestas(), auxx, auxy);
// 					getXandY(*varre, g->get_allArestas(), vx, vy);
					
// 					dis = abs(a*antix1 + br*antiy + cr)/sqrt(a*a + br*br); // entre anti e a nova árove
// 					if (dis>maxDistance){	
// 						maxDistance=dis;
// 						auxb = auxy - a*auxx1;
// 					} 
// 					dis = abs(a*vx1 + br*vy + cr)/sqrt(a*a + br*br); // entre a nova árvore e a próxima
// 					if (dis>maxDistance){	
// 						maxDistance=dis;
// 						auxb = auxy - a*auxx1;
// 					}
					
// 					b =  auxb;
// 					varre = aux;
					
					
// 				} else {
// 					float vx, vy;
// 					getXandY(*varre, g->get_allArestas(), vx, vy);
					
// 					//verificar se o solução corrent está acima ou sobre a reta de custo máximo
// 					float y = a*vx + b;
// 					if (maiorQuefloat(y, vy)==false) pare = true; /*manda parar*/
					
// 					aux = --varre;
// 					varre++;
// 					k_best_tree.remove(*varre);
// 					//delete[] varre->getArvore();
// 					//delete varre;
					
// 					varre = aux; 
					 	
// 				}
// 				cout<<k<<endl;		
// 			} else {
// 				aux = --varre;
// 				varre++;
// 				k_best_tree.remove(*varre);
// 				//delete[] varre->getArvore();
// 				//delete varre;
// 				varre = aux;
// 			} 
// 			k++;
// 			varre++;
// 		//} while (varre!=NULL && k<100);
// 		} while (resul.end()!=varre && k<100);
// 		//delete k_best_tree;
// 		p = q;
// 		q = p++;
		
	
		
// 	}
// }




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
	cout<<"saiu1"<<endl;
	list<int*> noSuportadas= phase2KB(&my_grafo, arvores);
	
	map <int, Aresta *> arestasPtr = my_grafo.get_allArestas();
    
    
    int i = 1, cont=0;
    cout<<"saiu2"<<endl;
    //cout<<resul.size()<<endl;
   // list<int*>::iterator it=resul.begin();
   cout<<"SUPORTADAS"<<endl;
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

	cout<<"Nao Suportada"<<endl;

	for (list<int*>::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
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