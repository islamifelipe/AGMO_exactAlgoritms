/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Steiner and Radzik's (2003) algorithm 
# to resolve the 3-objective Spanning Tree Problem
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

#define MAX2 2000 // TODO : Aumentar

int idMST = 0;
map <int, Aresta *> arestas;
Aresta ** arestasPtr;
int contAux = 0, contAux2=0;
int nesimaArvore=1;
Grafo my_grafo;

typedef struct t{
	float x;
	float y;
	float z;
}Pesos;

typedef struct tt{
	int * sl;
	int * sll;
	int * slll;
}Trees;


bool isEgalObjetive(float t1_peso1, float t1_peso2, float t1_peso3, float t2_peso1, float t2_peso2,  float t2_peso3){
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2) && equalfloat(t2_peso3,t1_peso3) ;
}

//diferente dos outros algoritmos, esse egal aqui testa no espaço objetivo
bool isEgal(int *t1, int *t2){
	float t1_cont1=0, t1_cont2=0, t1_cont3=0,t2_cont1=0, t2_cont2=0, t2_cont3=0;
	for (int i=0; i<my_grafo.getQuantVertices()-1; i++){

		t1_cont1 += my_grafo.getArestas(t1[i])->getPeso1();
		t1_cont2 += my_grafo.getArestas(t1[i])->getPeso2();
		t1_cont3 += my_grafo.getArestas(t1[i])->getPeso3();

		t2_cont1 += my_grafo.getArestas(t2[i])->getPeso1();
		t2_cont2 += my_grafo.getArestas(t2[i])->getPeso2();
		t2_cont3 += my_grafo.getArestas(t2[i])->getPeso3();

	}
	return ((t1_cont1==t2_cont1) && (t1_cont2==t2_cont2) && (t1_cont3==t2_cont3));
}

bool order(pair<int*, pair<float, float> > &p1, pair<int*, pair<float, float> > &p2){

	int *t1 = p1.first;
	int *t2 = p2.first;

	float t1_cont1=0, t1_cont2=0, t1_cont3=0,t2_cont1=0, t2_cont2=0, t2_cont3=0;
	for (int i=0; i<my_grafo.getQuantVertices()-1; i++){

		t1_cont1 += my_grafo.getArestas(t1[i])->getPeso1();
		t1_cont2 += my_grafo.getArestas(t1[i])->getPeso2();
		t1_cont3 += my_grafo.getArestas(t1[i])->getPeso3();

		t2_cont1 += my_grafo.getArestas(t2[i])->getPeso1();
		t2_cont2 += my_grafo.getArestas(t2[i])->getPeso2();
		t2_cont3 += my_grafo.getArestas(t2[i])->getPeso3();

	}

	return (t1_cont1<t2_cont1) || (t1_cont1==t2_cont1 && t1_cont2<t2_cont2) || (t1_cont1==t2_cont1 && t1_cont2==t2_cont2 && t1_cont3<t2_cont3);



}

void retiraDuplciatas(list <pair<int*, pair<float, float> > > &list1, list <pair<int*, pair<float, float> > > &list2){

	for (list <pair<int*, pair<float, float> > >::iterator it1 =list1.begin();it1!=list1.end();it1++){
		
		for (list <pair<int*, pair<float, float> > >::iterator it2=list2.begin();it2!=list2.end();it2++){
			if (isEgal(it1->first, it2->first)){
				list2.erase(it2);
				it2--;
			}
		}
	}

}

void retiraDuplciatas2(list <pair<int*, pair<float, float> > > &list1){

	for (list <pair<int*, pair<float, float> > >::iterator it1 =list1.begin();it1!=list1.end();it1++){
		list <pair<int*, pair<float, float> > >::iterator it2=++it1;
		it1--;
		for (;it2!=list1.end();it2++){
			if (isEgal(it1->first, it2->first)){
				list1.erase(it2);
				it2--;
			}
		}
	}

}

///ALGORITMO DA SONRENSEN JANSSENS (2003)

// void Partition(Grafo P, float xl, float yl, float xll, float yll,int* Pa, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes, int pares){
// 	/*Pa = vetor de arestas = correspondente à partição P (n-1 arestas)
// 	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
// 	*/
// 	contAux++;
// 	Grafo P1 = P, P2 = P;
// 	//cout<<List.getSize()<<endl;
// 	bool res = false;
// 	float custo;
// 	int *A2;
// 	int m = P.getQuantArestas();
// 	int n =P.getQuantVertices();
// 	//for (int i=0; i<m && List.getSize()<MAX2; i++){
// 	for (int i=0; i<n-1 && List.getSize()<MAX2; i++){
// 		int iddnovo = Pa[i];
// 		if (P.getStatus(iddnovo)==0){ /*Se a aresta for opcional*/
// 			A2 = new int[n-1];
// 			//A2 = new int[m];
// 			P1.setStatus(iddnovo, 2); /*proibida*/
// 			P2.setStatus(iddnovo, 1); /*obrigatória*/
// 			float x, y, x_aux, y_aux, z_aux;
// 			res = kruskal(&P1, arestasPtr,A2, x_aux, y_aux, z_aux);
// 			if (pares==12) {
// 				x = x_aux;
// 				y = y_aux;
// 			} else if (pares == 13){
// 				x = x_aux;
// 				y = z_aux;
// 			} else{ // 23
// 				x = y_aux;
// 				y = z_aux;
// 			}
// 			custo =x*(yl-yll)+y*(xll-xl);
// 			if (res){
// 				vetorParticoes.push_back(P1);
// 				list<Grafo>::iterator itt = vetorParticoes.end();
// 				itt--;
// 				MSTs[idMST] = make_pair(make_pair(A2,make_pair( x, y)),itt);//A2;
// 				List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
				
// 			} else {
// 				delete[] A2;
// 			}
// 			P1.setStatus(iddnovo, 1); 
// 		}
// 	}
// }

// int AllSpaningTree(Grafo *g,float xl, float yl, float xll, float yll, list< pair<int*, pair<float, float> > > &resul, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs, list <Grafo > &vetorParticoes, float a, float bM, int k, int pares){ 
	
// 			int id = List.getId();
// 			pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > par = MSTs[id];
				
// 			pair<int*, pair<float, float> > it = par.first;//MSTs[id];
// 			Grafo Ps = *par.second;
			
// 			List.extract();
// 			resul.push_back(it);
// 			//MSTs.erase(id);
// 			//vetorParticoes.erase(id);
// 			float x = it.second.first;
// 			float y = it.second.second;
			
// 			if (maiorIgualQuefloat(y,(a*x+bM))==false){ //k<363 &&  // Caso o ponto minimo passa da linha maxima, nao precisa calcular novamente
				
// 				Partition(Ps,xl, yl, xll, yll, it.first, List,MSTs,vetorParticoes, pares);
			
// 			}
// 	return MSTs.size();
// }


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)



void borderSearch(Grafo *g, list<pair<int*, Pesos > > &resul, pair<int*, Pesos > &sl, pair<int*, Pesos > &sll,pair<int*, Pesos > &slll){ 
	/* 
	**/
	int *s1 =   sl.first;
	int *s2 =  sll.first;
	int *s3 = slll.first;
	int * A2;
	stack<Trees >  pilha;
	stack<Pesos > pilhaX;
	stack<Pesos > pilhaY;
	stack<Pesos > pilhaZ;
	stack<pair<int,list<pair<int*, Pesos> >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	

	pilha.push((Trees){s1, s2, s3});
	pilhaX.push( (Pesos) {sl.second.x, sll.second.x, slll.second.x}); //x's da primeira, segunda e Terceira solucao
	pilhaY.push( (Pesos) {sl.second.y, sll.second.y, slll.second.y}); //y's da primeira, segunda e Terceira solucao
	pilhaZ.push( (Pesos) {sl.second.z, sll.second.z, slll.second.z}); //Z's da primeira, segunda e Terceira solucao
	pilhaIt.push(make_pair(2, resul.begin())); 


	int connnt=0;

	while (pilha.size()!=0 && connnt<14){//connnt++;
		Trees sols = pilha.top();
		pilha.pop();
		s1 = sols.sl;
		s2 = sols.sll;
		s3 = sols.slll;
		pair<int,list<pair<int*, Pesos> >::iterator>  it = pilhaIt.top();
		pilhaIt.pop();
		Pesos xs = pilhaX.top();
		Pesos ys =  pilhaY.top();
		Pesos zs =  pilhaZ.top();

		float xl = xs.x;
		float yl = ys.x;
		float zl = zs.x;

		float xll = xs.y;
		float yll = ys.y;
		float zll = zs.y;

		float xlll = xs.z;
		float ylll = ys.z;
		float zlll = zs.z;

		pilhaX.pop();
		pilhaY.pop();
		pilhaZ.pop();

		A2 = new int[g->getQuantVertices()-1];
		mergesort(xl, yl, zl, xll, yll, zll, xlll, ylll, zlll, arestasPtr, g->getQuantArestas(),4);
		float x, y,z;

		kruskal(g, arestasPtr,A2, x, y, z);
	
		if( !( (isEgalObjetive(x, y, z, xl, yl, zl)) || (isEgalObjetive(x, y,z, xll, yll, zll)) || (isEgalObjetive(x, y,z, xlll, ylll, zlll)) ) ){

			if (it.first == 1){ // antes
				resul.insert(it.second, make_pair(A2,(Pesos){x, y, z}));//A2); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, make_pair(A2,(Pesos){x, y, z}));
				it.second--;// it agora aponta para o item  A2
			}

			pilha.push((Trees){s2, s3, A2}); 
			pilhaIt.push(make_pair(1,it.second)); 
			pilhaX.push((Pesos){xll, xlll, x});
			pilhaY.push((Pesos){yll, ylll, y});
			pilhaZ.push((Pesos){zll, zlll, z});

			pilha.push((Trees){s1, s3, A2});  
			pilhaIt.push(make_pair(1,it.second)); 
			pilhaX.push((Pesos){xl, xlll, x});
			pilhaY.push((Pesos){yl, ylll, y});
			pilhaZ.push((Pesos){zl, zlll, z});

			pilha.push((Trees){s1, s2, A2});  
			pilhaIt.push(make_pair(1,it.second)); 
			pilhaX.push((Pesos){xl, xll, x});
			pilhaY.push((Pesos){yl, yll, y});
			pilhaZ.push((Pesos){zl, zll, z});

			
		} else cout<<"fora!!!"<<endl;
	
	}
}


/*
#### Primeira fase ####
#*/
void phase1GM(Grafo *g, list<pair<int*, Pesos > > &result){
	int *s1 = new int[g->getQuantVertices()-1];
	float xr, yr,zr; 

	mergesort(0, 0, 0,0, 0, 0,0, 0, 0, arestasPtr, g->getQuantArestas(),1); // primeiro objetivo
	kruskal(g, arestasPtr,s1, xr, yr,zr); // arvore para o primeiro objetivo
	pair<int*, Pesos > ps1 = make_pair(s1, (Pesos){xr, yr,zr});
	result.push_back(ps1);
	///cout<<xr<<" "<<yr<<" "<<zr<<endl;
	int* s2 = new int[g->getQuantVertices()-1];
	mergesort(0, 0, 0,0, 0, 0,0, 0, 0, arestasPtr, g->getQuantArestas(), 2); // segundo objetivo
	float xr2, yr2, zr2;
	kruskal(g, arestasPtr, s2,xr2, yr2, zr2); // arvore para o segundo objetivo
	pair<int*, Pesos > ps2 = make_pair(s2, (Pesos){xr2, yr2, zr2});
	//cout<<xr2<<" "<<yr2<<" "<<zr2<<endl;
	int* s3 = new int[g->getQuantVertices()-1];
	mergesort(0, 0, 0,0, 0, 0,0, 0, 0, arestasPtr, g->getQuantArestas(), 3); // segundo objetivo
	float xr3, yr3, zr3;
	kruskal(g, arestasPtr, s3,xr3, yr3, zr3); // arvore para o segundo objetivo
	pair<int*,Pesos > ps3 = make_pair(s3, (Pesos){xr3, yr3, zr3});
	//cout<<xr3<<" "<<yr3<<" "<<zr3<<endl;

	if (isEgalObjetive(xr, yr,zr, xr2, yr2, zr2)==false && isEgalObjetive(xr, yr,zr, xr3, yr3, zr3)==false && isEgalObjetive(xr2, yr2,zr2, xr3, yr3, zr3)==false){
		borderSearch(g, result, ps1, ps2, ps3);
		result.push_back(ps2);
		result.push_back(ps3);
	}

	//return result;
 }

// // /* 
// // # retorna verdadeiro se o ponto kbest está dentro da regiao viável
// // # se verdeiro, atualiza a regiao viável
// // */	
// bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y){
// 	bool retorno = false; // por default, o ponto nao está na regiao viavel 
// 	//This check is done by a simple linear search through the list of the consecutive corner points defining the viable region.
// 	//cout<<"\nkbest = ("<<x<<"'"<<y<<")"<<endl;
// 	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
// 		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
// 		float ponto_x = ponto.first, ponto_y = ponto.second;
// 		/* o k_best_x deve ser menor que o ponto_x   E   o k_best_y deve ser menor que o ponto_y  
// 		caso isso nao seja verdadeiro para nenhum ponto, entao o kbest nao está na regiao viavel*/ 

// 		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		
// 		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
// 			pair<float, float> ponto1 = make_pair(x, ponto_y);
// 			pair<float, float> ponto2 = make_pair(ponto_x, y);
// 			regiaoViavel.insert(it, ponto1);
// 			regiaoViavel.insert(it, ponto2);
// 			regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
// 			retorno = true;
// 			break; // se o ponto está regiao viavel, nao há mais rezao para percorrer-la 
// 		}
// 	}
// 	return retorno;
// }

// pair<float, float> getMaiorDistante(float a, float b, float c, list< pair<float, float> > regiaoViavel){
// 	pair<float, float>  pontoR = (*regiaoViavel.begin());
// 	float max = ((float)abs(a*pontoR.first + b*pontoR.second + c))/((float) sqrt(a*a + b*b));
// 	for (list< pair<float, float> >::iterator it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
// 		pair<float, float> ponto = (*it);
// 		float distancia = abs(a*ponto.first + b*ponto.second + c)/sqrt(a*a + b*b); // distância de ponto a r
//  		if (maiorQuefloat(distancia,max)){
//  			max = distancia;
//  			pontoR = ponto;
//  		}
// 	}
// 	return pontoR;
// }
// // /* 
// // Recebe o grafo e a lista de solucoes soportads (as extremas do eixo convexo) 
// // Retorna a lista de solucoes nao-suportadas (aquelas que estao dentro de um triângulo formado por duas soluceos extremas)
// // Assim, teremos separadas as solucoes suportadas e nao suportadas
// // */
// void phase2KB(Grafo *g, list< pair<int*, pair<float, float> > > extremas, int pares, list< pair<int*, pair<float, float> > > &noSoportadas){

// 	list< pair<int*, pair<float, float> > >::iterator it = extremas.begin(); 
// 	int contador = 0;
// 	int size = extremas.size();
// 	while (contador<size-1){
// 		pair<int*, pair<float, float> > pp = *(it);
// 		pair<int*, pair<float, float> > pq = *(++it);
// 		int *p = pp.first; 
// 		int *q = pq.first;
// 		float yp, yq, xp, xq;
// 		list< pair<float, float> > regiaoViavel; // lista de pontos que delimitam a regiao viável.
	
// 		xp = pp.second.first;
// 		yp = pp.second.second;
// 		xq = pq.second.first;
// 		yq = pq.second.second;
// 		//getXandY(p,xp, yp); // p
// 	 	//getXandY(q,xq, yq);	 //q
// 		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		
// 		// determina a reta p-q (hipotenusa)
// 		float a, b; // pra determinar a equacao da reta p-q na forma ax+b = y
// 		a = ((float)(yp-yq))/((float) (xp-xq)); // coeficiente angular da reta p-q
// 		b = yq - a*xq; // coeficiente linear de p-q
// 		pair<float, float> maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
// 		//Agora determinamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
// 		//cout<<"Mais distante : "<<maisDistante.first<<", "<<maisDistante.second<<endl;
// 		float bM = maisDistante.second - a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y

// 		Heap List(MAX2); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE UM VALOR
// 		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
// 		map<int, pair<pair<int*, pair<float, float> >, list<Grafo>::iterator > > MSTs; // usada para lista de árvores
// 		idMST = 0;
// 		//cout<<"REDEFINIU"<<endl;
// 		int *A = new int[g->getQuantVertices()-1]; // usada para a primeira árvore 
// 		//for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
// 		mergesort(xp,yp, xq,yq, arestasPtr, g->getQuantArestas(),4, pares);
// 		float x, y, x_aux, y_aux, z_aux;
// 		bool res = kruskal(g, arestasPtr, A,x_aux, y_aux, z_aux);
// 			if (pares==12) {
// 				x = x_aux;
// 				y = y_aux;
// 			} else if (pares == 13){
// 				x = x_aux;
// 				y = z_aux;
// 			} else{ // 23
// 				x = y_aux;
// 				y = z_aux;
// 			}
// 		float custoMinimo = x*(yp-yq)+y*(xq-xp);
		
// 		if (res){
// 			vetorParticoes.push_back(*g);
// 			list<Grafo>::iterator itt = vetorParticoes.end();
// 			itt--;
// 			MSTs[idMST] = make_pair(make_pair(A, make_pair(x, y)),itt);//A2;
// 			List.insert(idMST++, custoMinimo); // o valor da variavel "custo" vem do kruskal
// 		}

// 		for (int k = 1; k<MAX2 && List.getSize()!=0; k++){
// 			list<pair<int*, pair<float, float> > > k_best_tree;
		
// 			AllSpaningTree(g,xp,yp, xq,yq, k_best_tree, List,MSTs, vetorParticoes,a, bM, k, pares);  // k-best
// 			pair<int*, pair<float, float> >  k_best = *(k_best_tree.begin());
// 			float x = k_best.second.first;
// 			float y= k_best.second.second;

// 			if (isInViableRegion(g, regiaoViavel, x, y)){
// 				noSoportadas.push_back(k_best);
// 				maisDistante = getMaiorDistante(a, -1, b, regiaoViavel);
// 				//Agora atualizamos a reta de custo maximo, ou seja, a reta paralela à p-q que passa pelo ponto mais distante
// 				bM = (float)maisDistante.second - (float)a*maisDistante.first; // coeficiente angular da reta de custo maximo ax+bM = y
// 				//cout<<"k add = "<<k<<endl;
// 			} else if ( maiorIgualQuefloat(y,(a*x+bM))) { //s on or past maximum cost line 
// 				//cout<<"K break = "<<k<<endl;
// 				break;
// 			}
// 		}
// 		//cout<<"List size final = "<<List.getSize()<<endl;	
// 		//cout<<"RV = "<<regiaoViavel.size()<<endl;
				
// 		contador++;
// 	}
// 	//return noSoportadas;

// }
//nesimaArvore
void printArvore (list <pair<int*, Pesos > > &list1, Grafo *my_grafo){
	for (list<pair<int*, Pesos > >::iterator it=list1.begin(); it!=list1.end(); it++){
		
		cout<<"Arvore "<<nesimaArvore<<endl;
		float p1=0, p2=0, p3=0;
    	for (int a = 0; a<my_grafo->getQuantVertices()-1; a++){ // cada aresta da arvore
			int iddd = ((*it).first)[a];
			
				cout<<my_grafo->getArestas(iddd)->getOrigem() << " ";
    			cout<<my_grafo->getArestas(iddd)->getDestino() << " ";
    			cout<<my_grafo->getArestas(iddd)->getPeso1() << " ";
    			cout<<my_grafo->getArestas(iddd)->getPeso2() << " ";
    			cout<<my_grafo->getArestas(iddd)->getPeso3() << endl;
    			p1 += my_grafo->getArestas(iddd)->getPeso1();
    			p2 += my_grafo->getArestas(iddd)->getPeso2();
    			p3 += my_grafo->getArestas(iddd)->getPeso3();
    		
    	}
    	cout<<"("<<p1<<","<<p2<<","<<p3<<")\n"<<endl;
    	nesimaArvore++;
	}
}

int main(){
	struct tms tempsInit, tempsFinal1,tempsFinal2 ; // para medir o tempo
	
	int n;
	float peso1, peso2, peso3;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	my_grafo.setN(n);
	// contruir o grafo
	for (int i=0; i<n; i++){ // PADRAO : vértices numerados de 0 à n-1
		my_grafo.addVertice(i);
	}
	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		cin>>peso3;
		my_grafo.addAresta(id, origem, destino, peso1, peso2, peso3);
		id++;
	}

	int nA = id; // quantidade de arestas do grafo	
	//my_grafo.gerarArestasPtr();
	
	arestas = my_grafo.get_allArestas();
	arestasPtr = my_grafo.getAllArestasPtr();

	list <pair<int*, Pesos > > arvoresSuportadas;
	list< pair<int*, pair<float, float> > > noSoportadas;
	//cout<<"COMBINACAO 1 E 2 OBJETIVOS"<<endl;
 	times(&tempsInit);

	phase1GM(&my_grafo,arvoresSuportadas);
	cout<<"Fim da primeira fase ... "<<endl;

	times(&tempsFinal1);   /* current time */ // clock final
	clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
	cout<<user_time1<<endl;
	cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	
	// arvoresSuportadas.sort(order);
	// retiraDuplciatas2(arvoresSuportadas);
 //   	times(&tempsInit);


	// phase2KB(&my_grafo, arvoresSuportadas, 12,noSoportadas);
	// phase2KB(&my_grafo, arvoresSuportadas, 13,noSoportadas);
	// phase2KB(&my_grafo, arvoresSuportadas, 23,noSoportadas);
	

	// cout<<"Fim da segunda fase ... "<<endl;

	//  times(&tempsFinal2);    current time  // clock final
	//  clock_t user_time2 = (tempsFinal2.tms_utime - tempsInit.tms_utime);
	//  cout<<user_time2<<endl;
	//  cout<<(float) (user_time2+user_time1) / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
	// cout<<"Total ..."<<endl;
	// cout<<(float) (user_time1+user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;

    int i = 1, cont=0;
   cout<<"Resultado \n SUPORTADAS"<<endl;
   // printArvore(arvores, &my_grafo);
   // printArvore(arvores13, &my_grafo);
   printArvore(arvoresSuportadas, &my_grafo);

	// cout<<"Resultado \n NAO SUPORTADAS"<<endl;

	 //  printArvore(noSoportadas, &my_grafo);
	// printArvore(noSuportadas, &my_grafo);
 //   printArvore(noSuportadas13, &my_grafo);
 //   printArvore(noSuportadas23, &my_grafo);


	cout<<"Partition = "<<contAux<<endl;
	return 0;
}