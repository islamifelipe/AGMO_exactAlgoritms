/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Ramos et al's (1998) algorithm 
# to resolve the Biobjective Spanning Tree Problem
# The user time is returned;
#=======================================================================
*/

#include <iostream>
#include <map> 
#include <list>
#include <string>
#include <cmath>
#include <stack>   
#include <utility>      // std::pair
#include <sys/times.h>
#include <unistd.h>
#include "Grafo.h"
#include "Conjunto.h"
#include "kruskal.h"
#include "Heap.h"
using namespace std;


struct tms tempsInit, tempsFinal; // para medir o tempo
list<pair<int*, pair<float, float> > > noSuportadas;
list<pair<int*, pair<float, float> > > arvoresSuportadas;
list<pair<int*, pair<float, float> > > arvoresSuportadasGLOBAL;
map <int, Aresta *> arestasPtr;
Grafo my_grafo;
int nA ;

vector< pair<float, float> > min_f_g_vector;

bool isEgalObjetive(float xl, float yl,float xll, float yll){
	float t1_peso1=xl, t1_peso2=yl, t2_peso1=xll, t2_peso2=yll;
	
	return equalfloat(t1_peso1,t2_peso1) && equalfloat(t2_peso2,t1_peso2);

}
bool t1_domina_t2(float xl, float yl,float xll, float yll){
	float t1_peso1=xl, t1_peso2=yl, t2_peso1=xll, t2_peso2=yll;
	if (t1_peso1 <= t2_peso1 && t1_peso2 <= t2_peso2 && (t1_peso1 < t2_peso1 || t1_peso2 < t2_peso2)){
		return true;
	} else return false;
}



int idMST = 0;


#define MAX2 50
///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, float xl, float yl, float xll, float yll,int* Pa, Heap &List, map<int, pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > > &MSTs,list <Grafo > &vetorParticoes, int direto){
	/*Pa = vetor de arestas = correspondente à partição P (n-1 arestas)
	cont = contar quantas vezes o Kruskal foi invocado (apenas para fins estatísticos)
	*/
	Grafo P1 = P;//, P2 = P;
	//cout<<List.getSize()<<endl;
	bool res = false;
	float custo;
	int *A2;
	int m = P.getQuantArestas();
	int n =P.getQuantVertices();
	//for (int i=0; i<m && List.getSize()<MAX2; i++){
	for (int i=0; i<m && List.getSize()<MAX2; i++){
		//int iddnovo = Pa[i];
		if (Pa[i]==1){
			int iddnovo = i;
			if (P.getStatus(iddnovo)==0){ /*Se a aresta for opcional*/
				A2 =  new int[m];//new int[n-1];
				P1.setStatus(iddnovo, 2); /*proibida*/
				//P2.setStatus(iddnovo, 1); /*obrigatória*/
				float x, y;
				res = kruskal(&P1, A2, xl, yl, xll, yll,custo,x,y, direto);//kruskal(&P1,A2, x, y);
				// double custo2 =x*(yl-yll)+y*(xll-xl);
				// cout<<"ODKFODK"<<endl;

				if (res){
					vetorParticoes.push_back(P1);
					list<Grafo>::iterator itt = vetorParticoes.end();
					itt--;
					MSTs[idMST] = make_pair(make_pair(A2,make_pair( x, y)),itt);//A2;
					List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
					
				} else {
					delete[] A2;
				}
				P1.setStatus(iddnovo, 1); 
			}
		}
	}
}

void AllSpaningTree(Grafo *g,float xl, float yl, float xll, float yll, vector< pair<int*, pair<float, float> > > &resul, int direto){ 
		// vector< pair<int*, pair<float, float> > > resul;
		Heap List(MAX2); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE UM VALOR
		list<Grafo> vetorParticoes; //Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
		map<int, pair<pair<int*, pair<float, float> >, list<Grafo>::iterator > > MSTs; // usada para lista de árvores
		idMST = 0;
		int *A = new int[g->getQuantArestas()]; //new int[g->getQuantVertices()-1]; // usada para a primeira árvore 
		//for(int mmm = 0; mmm<g->getQuantArestas(); mmm++) A[mmm] = 0;
		// mergesort(xp,yp, xq,yq, arestasPtr, g->getQuantArestas(),3);
		float x, y, custo;
		bool res = kruskal(g, A, xl, yl, xll, yll,custo,x,y, direto);//kruskal(&P1,A2, x, y);
		float primeiro = custo;
		float xprimeiro = x, yprimeiro = y;
		// cout<<"custo = "<<primeiro<<endl;
		if (res){
			vetorParticoes.push_back(*g);
			list<Grafo>::iterator itt = vetorParticoes.end();
			itt--;
			MSTs[idMST] = make_pair(make_pair(A, make_pair(x, y)),itt);//A2;
			List.insert(idMST++, custo); // o valor da variavel "custo" vem do kruskal
		}

		while (List.getSize()!=0){ // && custo==primeiro && xprimeiro == x && yprimeiro == y
 			int id = List.getId();
			pair< pair<int*, pair<float, float> >, list<Grafo>::iterator > par = MSTs[id];
				
			pair<int*, pair<float, float> > it = par.first;//MSTs[id];
			Grafo Ps = *par.second;
			float coosottt = it.second.first*(yl-yll) + it.second.second*(xll-xl);
			if (coosottt != primeiro || xprimeiro != it.second.first || yprimeiro != it.second.second) break; // TOMA SOMENTE OS PRIMEIROS IGUAIS
			List.extract();
			resul.push_back(it);
			//MSTs.erase(id);
			//vetorParticoes.erase(id);
			float x = it.second.first;
			float y = it.second.second;
			
			Partition(Ps,xl, yl, xll, yll, it.first, List,MSTs,vetorParticoes, direto);
		}	
			
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)


void UniobjectiveSearch(Grafo *g, list<pair<int*, pair<float, float> > > &resul, int * sl, pair<float, float> psl, int *sll, pair<float, float> psll){ 
	/* it = interator da lista
	* Os novos elementos (arvores) devem ser inseridos entre it-1 e it
	* sl = s'
	* sll = s''
	**/
	int *s1 = sl;
	int *s2 = sll;
	int * A2;
	stack<pair<int*,int*> >  pilha;
	stack<pair<int,list<pair<int*, pair<float, float> > >::iterator> >  pilhaIt; // 1 : antes ; 2 : depois
	stack<pair<float, float> > pilhaF; // f de uma solucao, f da outra
	stack<pair<float, float> > pilhaG; // idem
	float xl, yl, xll, yll;

	pilha.push(make_pair(s1, s2));
	pilhaIt.push(make_pair(2, resul.begin()));
	pilhaF.push(make_pair(psl.first, psll.first));
	pilhaG.push(make_pair(psl.second, psll.second));


	while (pilha.size()!=0){
		//cout<<"size = "<<pilha.size()<<endl;
		pair<int*,int*> sols = pilha.top();
		pilha.pop();
		s1 = sols.first;
		s2 = sols.second;
		pair<int,list< pair<int*, pair<float, float> > >::iterator> it = pilhaIt.top();
		pilhaIt.pop();

		pair<float, float> f = pilhaF.top();
		xl = f.first;
		xll = f.second;
		pair<float, float> gg = pilhaG.top();
		yl = gg.first;
		yll = gg.second;

		pilhaF.pop();
		pilhaG.pop();


		//getXandY(s1, g->get_allArestas(), xl, yl);
		//getXandY(s2, g->get_allArestas(), xll, yll);
		// A2 = new int[g->getQuantArestas()];
		// for (int i=0; i<g->getQuantArestas(); i++) A2[i] = 0;
		float cont; // nao utilisazado nesse caso
		float x, y;
		vector< pair<int*, pair<float, float> > > resulttttt;
		 AllSpaningTree(g,xl, yl,  xll, yll, resulttttt, 3);
		x = resulttttt[0].second.first;
		y = resulttttt[0].second.second;
		// kruskal(g, A2, xl, yl, xll, yll,cont,x,y, 3);
		// cout<<"size() = "<<resulttttt.size()<<endl;
		if( !( (isEgalObjetive(x,y,xl,yl)) || (isEgalObjetive(x,y, xll,yll)) ) ){
			if (it.first == 1){ // antes
				resul.insert(it.second, make_pair(resulttttt[0].first, make_pair(x,y))); 
				it.second--;// it agora aponta para o item  A2
			} else if (it.first == 2) { // depois
				it.second++;
				resul.insert(it.second, make_pair(resulttttt[0].first, make_pair(x,y)));
				it.second--;// it agora aponta para o item  A2
			}
			for (int i=0; i<resulttttt.size(); i++){
				arvoresSuportadasGLOBAL.push_back(resulttttt[i]);
			}

			pilha.push(make_pair(resulttttt[0].first, s2)); // L''
			pilhaIt.push(make_pair(2,it.second)); 
			pilha.push(make_pair(s1, resulttttt[0].first));  // L'
			pilhaIt.push(make_pair(1,it.second)); 

			pilhaF.push(make_pair(x, xll));
			pilhaG.push(make_pair(y, yll));

			pilhaF.push(make_pair(xl, x));
			pilhaG.push(make_pair(yl, y));
			
		} //else delete[] A2; // NOVO
	}
}


/*
#### Primeira fase ####
# A funcao E􏳇cffientBiobjectiveSTinEB retorna a lista de arvores (elementos do espaço de decisao)
# Cada arvore é representada simplismente como um vetor de 0s ou 1s.
# Se o i-ésimo elemento do vetor é 1, entao a aresta de id=i está presente na arvore. Sera 0 caso contrario 
# Portanto, retorna uma lista de inteiros
*/
void efficientBiobjectiveSTinEB(Grafo *g){
	//list<pair<int*, pair<float, float> >  > arvoresSuportadas;
	// int *s1 = new int[g->getQuantArestas()];
	// for (int i=0; i<g->getQuantArestas(); i++) s1[i] = 0;
	// float cont; // nao utilisazado nesse caso
	// float xl, yl;
	vector< pair<int*, pair<float, float> > > resul;
	AllSpaningTree(g,0, 1, 0, 0, resul, 1);// primeiro objetivo
	// kruskal(g, s1, 0, 0, 0, 0,cont, xl, yl,  1); // arvore para o primeiro objetivo
	arvoresSuportadas.push_back(resul[0]);
	for (int i=0; i<resul.size(); i++){
		arvoresSuportadasGLOBAL.push_back(resul[i]);
	}
	// int* s2 = new int[g->getQuantArestas()];
	// for (int i=0; i<g->getQuantArestas(); i++) s2[i] = 0;
	// float xll, yll;
	// kruskal(g, s2, 0, 0, 0, 0,cont, xll, yll, 2); // arvore para o segundo objetivo
	vector< pair<int*, pair<float, float> > > resul2;
	AllSpaningTree(g,0,0, 1, 0, resul2, 2); //segundo objetivo
	
	list<pair<int*, pair<float, float> > >::iterator it = arvoresSuportadas.end();
	if (isEgalObjetive(resul[0].second.first,resul[0].second.second,resul2[0].second.first,resul2[0].second.second)==false){
		UniobjectiveSearch(g, arvoresSuportadas, resul[0].first, resul[0].second,resul2[0].first, resul2[0].second);
		

		for (int i=0; i<resul2.size(); i++){
			arvoresSuportadasGLOBAL.push_back(resul2[i]);
		}
		arvoresSuportadas.push_back(resul2[0]);
	}
}
/* 
# retorna verdadeiro se o ponto (x,y) está dentro da regiao viável
# se verdeiro, atualiza a regiao viável
*/	
bool isInViableRegion(Grafo *g, list< pair<float, float> > &regiaoViavel, float x, float y, list< pair<float, float> >::iterator &it){
	
	for ( it = regiaoViavel.begin(); it!=regiaoViavel.end(); it++){
		pair<float, float> ponto = (*it); // um ponto extremo que delimita a regiao viável
		float ponto_x = ponto.first, ponto_y = ponto.second;
		/* o x deve ser menor que o ponto_x   E   o y deve ser menor que o ponto_y  
		caso isso nao seja verdadeiro para nenhum ponto, entao o (x,y) nao está na regiao viavel*/ 
		//cout<<"corner = ("<<ponto_x<<"'"<<ponto_y<<")"<<endl;
		//cout<<"kbest = ("<<x<<"'"<<y<<")"<<endl;
		if (x<ponto_x && y<ponto_y){//caso o ponto esteja na regiao viavel, atualizamos-a imediatamente
			return true;
		}
	}
	
	return false;
}

pair<float, float> min_f_g_construcao(vector<Aresta*> adjacentes){ // retorna f barra e g barra {f, g}

	float f = adjacentes[0]->getPeso1(), g = adjacentes[0]->getPeso2();
	for (int i=1; i<adjacentes.size(); i++){
		if (adjacentes[i]->getPeso1()<f) f = adjacentes[i]->getPeso1();
		if (adjacentes[i]->getPeso2()<g) g = adjacentes[i]->getPeso2();
		
	}
	pair<float, float> result = make_pair(f, g); //{x,y} <--> {f, g} 
	return result;
}


void retiraDominadas(list<pair<int*, pair<float, float> > > &noSuportadas, pair<int*, pair<float, float> > T){
	
	for (std::list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it != noSuportadas.end(); ++it){
		if (t1_domina_t2(T.second.first, T.second.second, (*it).second.first, (*it).second.second)){
			int *qux =  (*it).first;
			it = noSuportadas.erase(it);
			delete[] qux;  
			it--;
		} else if (t1_domina_t2((*it).second.first, (*it).second.second, T.second.first, T.second.second)){
			noSuportadas.remove(T);

			delete[] T.first;
			return;
		}
	}
}

bool isInViableRegion2(float x, float y){
	for (std::list<pair<int*, pair<float, float> > >::iterator ittt=noSuportadas.begin(); ittt != noSuportadas.end(); ++ittt){
		if (isEgalObjetive((*ittt).second.first, (*ittt).second.second, x, y)==true) return true;
	}
	return false;
}
void EBST_BrB(int *T, float fBound, float gBound, int step, Grafo *g, list< pair<int*, pair<float, float> > > &noSuportadas, Conjunto conjunto, list< pair<float, float> > &regiaoViavel){
	
	stack<Conjunto> pilhaCojunto; // cojuntos
	stack<pair<float, float> > pilhaLimites; // fBound, gBound
	stack<pair<int *, int> > pilha; // solucao parcial , step
	stack<pair<float, float> >pilhaFG; // f, g da solucao do topo da pilha
	
	pilhaCojunto.push(conjunto);
	pilhaLimites.push(make_pair(fBound, gBound));
	pilha.push(make_pair(T, step));
	pilhaFG.push(make_pair(0,0));

	float tf, tg;
	pair<int *, int> p1;
	pair<float, float> fg;
	pair<float, float> p2;
	list< pair<float, float> >::iterator it;
	pair<float, float> ponto, ponto1,ponto2,fg_step1; 
	pair<int*, pair<float, float> > novo;
	float novo_tf,novo_tg,ponto_x,ponto_y;
	int *T_aux ;
	Aresta *e;
	vector<Aresta*> Astep;
	while (pilha.size()!=0){
		p1 = pilha.top();
		T = p1.first;
		step = p1.second;
		pilha.pop();

		fg = pilhaFG.top();
		tf = fg.first;
		tg = fg.second; 
		pilhaFG.pop();

		p2 = pilhaLimites.top();
		fBound = p2.first;
		gBound = p2.second;
		pilhaLimites.pop();

		conjunto = pilhaCojunto.top();
		pilhaCojunto.pop();

		 Astep = g->getVertice(step)->getAdjacentes();

		//cout<<fBound<<", "<<gBound<<endl;
		//cout<<"regiaoViavel = "<<regiaoViavel.size()<<endl;
		//cout<<"pilha = "<<pilha.size()<<endl;
		for (int a=0; a<Astep.size(); a++){
			e = Astep[a];
			//cout<<T[e->getId()]<<"   "<< conjunto.compare(e->getOrigem(), e->getDestino())<<endl;
			if (!conjunto.compare(e->getOrigem(), e->getDestino())){ // se nao formar ciclo

				//getXandY(T, g->get_allArestas(), tf, tg);
				//list< pair<float, float> >::iterator it;
				//cout<<isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)<<endl;

				if (isInViableRegion(g, regiaoViavel, tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound, it)){ // se estiver na regiao viavel
					
					T_aux = new int[g->getQuantArestas()];
					for (int i=0; i<g->getQuantArestas(); i++) T_aux[i] = T[i]; // copia
					Conjunto copie(g->getQuantVertices());
					copie.copia(conjunto);
					T_aux[e->getId()] = 1;
					novo_tf = tf+e->getPeso1();
					novo_tg = tg+e->getPeso2();
					copie.union1(e->getOrigem(), e->getDestino());
					if (step+1 == g->getQuantVertices()-1){
						novo = make_pair(T_aux,make_pair(novo_tf,novo_tg));
						noSuportadas.push_back(novo); // armazena T
						retiraDominadas(noSuportadas, novo); //remove any solutions dominated by T
						
						// atualiza a regiao viavel
						ponto = (*it); // um ponto extremo que delimita a regiao viável
						 ponto_x = ponto.first, ponto_y = ponto.second;
						ponto1 = make_pair(tf+e->getPeso1(), ponto_y);
						ponto2 = make_pair(ponto_x, tg+e->getPeso2());
						regiaoViavel.insert(it, ponto1);
						regiaoViavel.insert(it, ponto2);
						regiaoViavel.remove(ponto); // remove-se o ponto (corner), e adiciona-se os novos dois pontos formados
							
					} else {
						fg_step1 = min_f_g_vector[step+1];//min_f_g(g->getVertice(step+1)->getAdjacentes());
						pilhaCojunto.push(copie);
						pilhaLimites.push(make_pair(fBound-fg_step1.first,gBound-fg_step1.second));
						pilha.push(make_pair(T_aux, step+1));
						pilhaFG.push(make_pair(novo_tf, novo_tg));
					}
				} else if (isInViableRegion2(tf+e->getPeso1()+fBound, tg+e->getPeso2()+gBound)){
					T_aux = new int[g->getQuantArestas()];
					for (int i=0; i<g->getQuantArestas(); i++) T_aux[i] = T[i]; // copia
					Conjunto copie(g->getQuantVertices());
					copie.copia(conjunto);
					T_aux[e->getId()] = 1;
					novo_tf = tf+e->getPeso1();
					novo_tg = tg+e->getPeso2();
					copie.union1(e->getOrigem(), e->getDestino());
					if (step+1 == g->getQuantVertices()-1){
						novo = make_pair(T_aux,make_pair(novo_tf,novo_tg));
						noSuportadas.push_back(novo); // armazena T
					}
				}
			}
		}
		delete[] T;
		conjunto.desaloca();
	}
}

void efficientBiobjectiveSTinENB(Grafo *g, list<pair<int*, pair<float, float> > > extremas){
	float fBound =0, gBound=0;
	for (int i=1; i<g->getQuantVertices()-1; i++){ 
	// o algoritmo diz que o somatorio deve ser de 2 até n-1. Como nossa numeracao começa do 0 até n-1, nos vamos do 1 até n-2
		pair<float, float> fg = min_f_g_vector[i]; //min_f_g(g->getVertice(i)->getAdjacentes());
		fBound+= fg.first;
		gBound+= fg.second;
	}
	int *T = new int[g->getQuantArestas()];
	for (int i=0; i<g->getQuantArestas(); i++) T[i] = 0;
	//list<pair<int*, pair<float, float> > > noSuportadas;
	Conjunto conjunto(g->getQuantVertices());
	// Calcular a regiao viavel inicial (delimitada somente pelos pontos suportados)
	list< pair<float, float> > regiaoViavel;
	list<pair<int*, pair<float, float> > >::iterator it = extremas.begin();
	int contador = 0;
	int size = extremas.size();
	while (contador<size-1){
		float yp, yq, xp, xq;
		int *p = (*it).first; 
		xp = (*it).second.first;
		yp = (*it).second.second;
		int *q = (*(++it)).first;
		xq = (*it).second.first;
		yq = (*it).second.second;
		//getXandY(p, g->get_allArestas(),xp, yp); // p
	 	//getXandY(q, g->get_allArestas(), xq, yq);	 //q
	 	//cout<<"("<<xq<<","<< yp<<")"<<endl;
		regiaoViavel.push_back( make_pair(xq, yp));// inicialmente, a regiao viável é composta por um unico ponto (o âgulo reto do triângulo cuja hipotenusa é a reta entre p-q -- ver algoritmo origial)
		contador++;
	}

	EBST_BrB(T, fBound,gBound,0,g, noSuportadas,conjunto, regiaoViavel);
	
	//return noSuportadas;

}



void printResultado(){
	int i = 1;
   cout<<"SUPORTADAS"<<endl;
    for (list<pair<int*, pair<float, float> > >::iterator it=arvoresSuportadasGLOBAL.begin(); it!=arvoresSuportadasGLOBAL.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
		
			if ((*it).first[a] == 1){
				cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}

	cout<<"Nao Suportada"<<endl;

	for (list<pair<int*, pair<float, float> > >::iterator it=noSuportadas.begin(); it!=noSuportadas.end(); it++){
		cout<<"Arvore "<<i<<endl;
    	for (int a = 0; a<my_grafo.getQuantArestas(); a++){ // cada aresta da arvore
		
			if ((*it).first[a] == 1){
    			cout<<arestasPtr[a]->getOrigem() << " ";
    			cout<<arestasPtr[a]->getDestino() << " ";
    			cout<<arestasPtr[a]->getPeso1() << " ";
    			cout<<arestasPtr[a]->getPeso2() << endl;
    		}
    	}
    	cout<<"("<<(*it).second.first<<", "<<(*it).second.second<<")\n"<<endl;
    	i++;
	}
}


void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec==3600){ //3600
			cout<<"RESULTADO AO FIM DE 1H"<<endl;
			printResultado();
			sleep(3400); // 3400 é importante pra nao ficar verificando todo o tempo
		} else if (sec==7200){//7200
			cout<<"RESULTADO AO FIM DE 2H"<<endl;
			printResultado();
			sleep(3400); // é importante pra nao ficar verificando todo o tempo
		} else if (sec==10800){// 10800 se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			
			cout<<"RESULTADO AO FIM DE 3H"<<endl;
			cout<<"TEMPO LIMITE ATINGIDO..."<<endl;

			printResultado();
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			//exit(-1);
			exit(EXIT_FAILURE);
		}
	}
}

int main(){

	int n;
	float peso1, peso2;
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
		my_grafo.addAresta(id, origem, destino, peso1, peso2);
		id++;
	}

	nA = id; // quantidade de arestas do grafo	
	my_grafo.gerarArestasPtr();
	
	for (int i=0; i<my_grafo.getQuantVertices(); i++){
		min_f_g_vector.push_back(min_f_g_construcao(my_grafo.getVertice(i)->getAdjacentes()));
	}
	arestasPtr = my_grafo.get_allArestas();
	times(&tempsInit);  // pega o tempo do clock inical
	
	// para medir o tempo em caso limite
	pthread_t thread_time; 
	pthread_attr_t attr;
	int nnnnnnnn=0;
	if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
        cout<<"Error to create the thread"<<endl;
        //exit(-1);
        exit(EXIT_FAILURE);
    }
    //

	efficientBiobjectiveSTinEB(&my_grafo);
	
	efficientBiobjectiveSTinENB(&my_grafo, arvoresSuportadas);
    
    times(&tempsFinal);   /* current time */ // clock final
	clock_t user_time2 = (tempsFinal.tms_utime - tempsInit.tms_utime);
	cout<<"Tempo em segundos: ";
	cout<<(float) (user_time2) / (float) sysconf(_SC_CLK_TCK)<<endl;

	cout<<"RESULTADO FINAL"<<endl;
    printResultado();
	return 0;
}
