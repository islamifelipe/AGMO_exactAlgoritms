#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <sys/time.h>
using namespace std;

#define MAX1 20000
//#define PRECISAO 0.0000000001

#define PRECISAO 0.00001

	

inline bool equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

inline bool maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

inline bool maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}


class Aresta{
	private:
		int origem, destino; /*se o grafo for não direcionado, não há diferença entre os vértices a origem e o destino*/
		int id; /*identificador da aresta*/
		float peso1, peso2; /*pesos da aresta*/
		//float pesoUnique; /*usado para guardar o valor gerado pelo métdo geométrico diconômico */
	public:
		Aresta(int id2, int origem1, int destino1, float peso1_1, float peso2_1){
			origem = origem1;
			destino = destino1;
			peso1 = peso1_1;
			peso2 = peso2_1;
			id= id2;
			//pesoUnique = 0;
		} 
		Aresta(){};
	
		/*void setPesoUnique(float novo){
			pesoUnique = novo;
		}
		float getPesoUnique(){
			return pesoUnique;
		}*/	
		int getId(){
			return id;
		}
		int getOrigem(){
			return origem;
		}
		int getDestino(){
			return destino;
		}
		float getPeso1(){
			return peso1;
		}
		float getPeso2(){
			return peso2;
		}
		void setId(int novo){
			id = novo;
		}
		void setOrigem(int n){
			origem = n;
		}
		void setDestino(int n){	
			destino = n;
		}

};


//IDEIA: as árvores são representadas por arranjos sequenciais de arestas (alocados dinamicamente). As árvores (i. e., tais vetores) são arranjadas em listas encadeadas, pois não sabemos quantas árvores haverá.
class ElementArvore{
	private:
		ElementArvore *next, *previous; //ponteiros para o endereço de memória que guarda  a próxima e a aterior aresta da lista;
		Aresta **arvore; //ponteiro para o endereço onde inicia o arranjo sequencial de ponteiros (Arestas) na memória. Essa árvore deve ser criada pelo usuário e passada ao construtor desta classe
		float x, y;
	public:
		ElementArvore(Aresta **tree, float x1, float y1){
			arvore = tree;
			next = NULL;
			previous = NULL;
			x=x1;
			y=y1;
		}
		Aresta **getArvore(){
			return arvore;
		}
		ElementArvore *getNext(){ // retorna o endereço de memória onde está localizada a próxima aresta 
			return next;
		}
		ElementArvore *getPrevious(){
			return previous;
		}
		void setNext(ElementArvore *next1){
			next = next1;
		}
		void setPrevious(ElementArvore *previous1){
			previous = previous1;
		}
		float getX(){
			return x;
		}	
		float getY(){
			return y;
		}
		
};

//A classe listaAresta é a lista propriamente dita. Ela guarda o início e o fim da lista
class ListaArvores{
	private:
		ElementArvore *init, *fim;		
	public:
		ListaArvores(){
			init = NULL;
			fim = NULL;
		}
		ElementArvore *getInit(){
			return init;
		}
		ElementArvore *getFim(){
			return fim;			
		}
		void setInit(ElementArvore *init1){
			init = init1;
		}
		void setFim(ElementArvore *fim1){
			fim = fim1;
		}	
		void insert2(ElementArvore *novoElemento){ //insere no final
			if (init==NULL){ // se a lista estiver vazia
				init = novoElemento;
				fim = novoElemento;
				novoElemento->setNext(novoElemento);
				novoElemento->setPrevious(novoElemento);
			} else {
				novoElemento->setNext(init);
				novoElemento->setPrevious(fim);
				fim->setNext(novoElemento);
				init->setPrevious(novoElemento);
				fim = novoElemento;
			}	
		}
		void insert(Aresta **nova, float x, float y){ // insere ao final
			ElementArvore *novoElemento = new ElementArvore(nova, x, y);
			insert2(novoElemento);	
		}
		void remove(ElementArvore *element){
			if (fim!=init){
				element->getPrevious()->setNext(element->getNext());
				element->getNext()->setPrevious(element->getPrevious());
				if (element==fim) fim = element->getPrevious();
				else if (element==init) init = element->getNext();
				//delete element;
			} else {
				init = NULL;
				fim = NULL;
				//delete element;
			}
		}		
};


class Vertice{
	private: 
		int id; /*identificador do vértice*/
		Aresta **adjacentes; /*Lista das arestas que incidem no vértice. Independe de o vértice em questão ser de origem ou destino. Vetor de ponteiros*/
		int contador; /*representará o grau do vértice*/
		
	public:
		Vertice (int id2, int n){ // n = número de vértices do grafo
			adjacentes = new Aresta*[n-1]; // geralmente, trabalhamos com grafos completos
			id = id2;
			contador=0;
		}
		Vertice(){};
		
		int getId(){
			return id;
		}
		void adicionaAresta(Aresta *a){
			adjacentes[contador++] = a;
		}
		
		Aresta* getAresta(int i){ /*retorna um ponteiro para a i-ésima aresta insidente no vértice*/
			return adjacentes[i];		
		}
		int getContador(){ //getGrau
			return contador;
		}
};

class Grafo{
	private:
		int n; /*número de vértice do grafo*/
		Vertice **listaDeAdjacencia; /*Lista de vértice. Cada vértice possui uma lista de arestas adjacentes. É um ponteiro que aponta para a primeira posição de memória de um arranjo sequancial alocado dinamicamente no construtor desta classe*/
		Aresta **listaArestas; /*lista de todas as arestas do Grafo. É um vetor de ponteiros*/
		/*Observação: é importante que "listaAresta" seja um vetor de ponteiros, pois, por exemplo, se alteramos alguma propriedade das arestas em listaArestas (i.e, nas referências das arestas), a alteração será refletida nas listas de insidência de aresta de cada vértice*/
		int contArestas;
		int particao[MAX1]; // vetor alocado dinamicamente no construtor. Size: quantidade de arestas 
		/*Este vetor (partição) é construído e interpretado da seguinte maneira:
			particao[i] = 0  ---> indica que a aresta de id=i é opcional (open)
			particao[i] = 1  ---> indica que a aresta de id=i é obrigatória 
			particao[i] = 2  ---> indica que a aresta de id=i é proibida
		*/
		
		float xl, yl, xll, yll;
	public:
		Grafo(int n1){
			n = n1;
			contArestas = 0;
			listaDeAdjacencia = new Vertice*[n]; // aloca n posições sequenciais de memória
			int quantArestas = (n*(n-1))/2;
			//particao = new int[quantArestas];
			listaArestas = new Aresta*[quantArestas];
			for (int i=0; i<quantArestas; i++){
				particao[i] = 0; /*inicialmente opcional*/
			}
		}
		Grafo(){};
		float getXl(){
			return xl;
		}
		
		float getYl(){
			return yl;
		}
		
		float getXll(){
			return xll;
		}
		
		float getYll(){
			return yll;
		}
		void setXl(float n){
			xl = n;
		}
		void setYl(float n){
			yl = n;
		}
		void setXll(float n){
			xll = n;
		}
		void setYll(float n){
			yll = n;
		}
		void addVertice(int id){
			Vertice *novo = new Vertice(id, n);
			listaDeAdjacencia[id] = novo; /*PADRÃO: A faixa de id's dos vértices é de 0 até n-1*/
		}

		Vertice *getVertice(int n){
			return listaDeAdjacencia[n]; 		
		}
		Aresta addAresta(int id, int origem, int destino, float peso1, float peso2){
			Aresta *nova = new Aresta(id, origem, destino, peso1, peso2);
			listaArestas[contArestas++] = nova; /*deve ser passado o ponteiro, isto é, a referência*/
			listaDeAdjacencia[origem]->adicionaAresta(nova);
			listaDeAdjacencia[destino]->adicionaAresta(nova);
			return *nova;
		}
		Aresta **getListaAresta(){ /*retorna a primeira posição do vetor listaArestas*/
			return listaArestas;
		}
		int getContArestas(){
			return contArestas;
		}
		int getN(){	
			return n;	
		}
		void setN(int nn){
			n = nn;
		}
		int getStatus(int i){
			return particao[i];
		} 
		void setStatus(int i, int valor){
			particao[i] = valor;
		}
		
};

class ElementGrafo{
	private:
		ElementGrafo *next, *previous; 
		Grafo grafo; 
	public:
		ElementGrafo(Grafo g){
			grafo = g;
			next = NULL;
			previous = NULL;
		}
		Grafo getGrafo(){
			return grafo;
		}
		ElementGrafo *getNext(){ // retorna o endereço de memória onde está localizada a próxima aresta 
			return next;
		}
		ElementGrafo *getPrevious(){
			return previous;
		}
		void setNext(ElementGrafo *next1){
			next = next1;
		}
		void setPrevious(ElementGrafo *previous1){
			previous = previous1;
		}
};

class ListaGrafos{
	private:
		ElementGrafo *init, *fim;		
	public:
		ListaGrafos(){
			init = NULL;
			fim = NULL;
		}
		ElementGrafo *getInit(){
			return init;
		}
		ElementGrafo *getFim(){
			return fim;			
		}
		void insert(Grafo nova){ // insere ao final
			ElementGrafo *novoElemento = new ElementGrafo(nova);
			if (init==NULL){ // se a lista estiver vazia
				init = novoElemento;
				fim = novoElemento;
				novoElemento->setNext(novoElemento);
				novoElemento->setPrevious(novoElemento);
			} else {
				novoElemento->setNext(init);
				novoElemento->setPrevious(fim);
				fim->setNext(novoElemento);
				init->setPrevious(novoElemento);
				fim = novoElemento;
			}
		}
		void remove(ElementGrafo *element){
			element->getPrevious()->setNext(element->getNext());
			element->getNext()->setPrevious(element->getPrevious());
			if (element==fim) fim = element->getPrevious();
			else if (element==init) init = element->getNext();
			delete element;
		}		
};


class Heap {
	private:
		int size; /*tamanho atual (parcial) da heap*/
		float *heap; //Guarda as chaves a serem ordenadas. Vetor de floats
		int *auxHeap; // Guarda os id das arestas
		int *auxID; //????
		int maximo;
	
	public: Heap(int maximo1){ 
		maximo = maximo1;
		heap = new float[maximo];
		auxHeap = new int[maximo];
		auxID = new int[maximo];		
		// Lembrar: A posição 0 do heap é ociosa
		for (int i=0; i<maximo; i++){
			auxID[i] = 0;
			auxHeap[i] = -1;
		}
		size = 0;
	}
	private:
		void subir(int i){ 
			int pai = i/2;
			if (pai>0){ /*vetor com size+1 posições*/
				if (maiorQuefloat(heap[pai],heap[i])){
					float aux1 = heap[i];
					heap[i] = heap[pai];
					heap[pai] = aux1;
				
					int aux = auxHeap[i];
					auxHeap[i] = auxHeap[pai];
					auxHeap[pai] = aux;
					
					subir(pai);
				}
					
			}
			if (auxHeap[i]>=0) auxID[auxHeap[i]] = i;
	}
	void descer(int i){
		int filho = i*2;
		if (filho<=size){
			if (filho!=size){
				if (!maiorIgualQuefloat(heap[filho+1],heap[filho])) filho++;
			}
			if (maiorQuefloat(heap[i],heap[filho])){
				float aux1 = heap[i];
				heap[i] = heap[filho];
				heap[filho] = aux1;
				
				int aux = auxHeap[i];
				auxHeap[i] = auxHeap[filho];
				auxHeap[filho] = aux;
			
				descer(filho);
			}
		}
		if (auxHeap[i]>=0)auxID[auxHeap[i]] = i;
	}
	public:
		bool insert (int id, float chave){ /*insere no final e sobe o elemento*/
			if (size+1<maximo && auxHeap[auxID[id]]!=id){
				heap[size+1]=chave;
				auxHeap[size+1] = id;
				size++;
				subir(size);
				return true;
			} else return false;
		}
	
	
	/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
	 	bool extract (){ /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
			if (size>0){
				heap[1] = heap[size];
				int id = auxHeap[1];
				auxHeap[1] = auxHeap[size];
				descer(1);
				size--;
				auxID[id] = 0;
				return true;
			} else return false;
		}
	
		float getChave(){
			return heap[1];
		}
		int getId(){
			return auxHeap[1];
		}
		int getSize(){
			return size;
		}
		void desaloca(){
			delete[] auxHeap;
			delete[] heap;
			delete[] auxID; 
		}
};


/*Heurística para a manipulação de conjuntos. Esta implementação é importante que se saber, por exemplo, se uma aresta que é adcionada gera um grafo cíclico*/
class Conjunto {
	private:
		int *pai;
		int *ordem;
	
	public:
		Conjunto(int n){
			pai = new int[n+1];
			ordem = new int[n+1];
			for (int i=0; i<=n; i++){
				pai[i] = i;
				ordem[i] = 0;
			}
		}
	
	private:
		void makeSet(int x){
			pai[x] = x;
			ordem[x] = 0;
		}
	
		/*Ao final da desta função, o nó x bem como todos os outros nós que constituem o caminho de localização (caminho percorrido de x até a raiz do conjunto/árvore)
		 * possuirão o mesmo pai, a saber, a raiz do conjunto/árvore*/
		int find_set(int x){
			if (x!=pai[x]){
				pai[x] = find_set(pai[x]);
			}
			return pai[x];
		}
	
		/*A união sempre pegará a raiz de "menor altura" e a colocará como subárovre filha da de "maior altura" */
		void link(int x, int y){
			if (ordem[x]>ordem[y]){  /*Note que caso aa ordens sejam diferentes, não há necessário de incremento da mesmas, pois a união 
			funcionaria como uma inserção de um nó numa árvore não-cheia, isto é, quando a inserção de um nó não aumenta a altura da árvore */
				pai[y] = x;
			}else{
				pai[x] = y;
				if (ordem[x]==ordem[y]){ 
					ordem[y] = ordem[y]+1;
				}
			}		
		} /* O vetor ordem é importante para controlar a altura da subárvore e assim poder construir um estrutura de menor altura possível. Isso resulta 
		em um número menor de comparações.*/
	
	
	public:
		void union1(int x, int y){
			link(find_set(x), find_set(y));
		}
	
		/*A raiz do conjunto o identifica; como a função find_set faz x ter como pai a raiz, e retorna este elemento, basta verificar se o valor
		 * retornando para a chamada com x e y são iguais.*/
		bool compare(int x, int y){
			return find_set(x)==find_set(y);
		
		}
		void desaloca(){
			delete[] pai;
			delete[] ordem;
		}
};



/*Implementação do MargeSort para utilizar no algortimo de Kruskal*/
void intercala(Grafo *g, int p, int q, int r, Aresta **v, int size, int direto){
	int i, j, k;
	Aresta **w = new Aresta *[size];
	float peso_i, peso_j; 
	i = p;
	j = q;
	k = 0;
	while (i < q && j < r) {
		if (direto==1){
			if (!maiorIgualQuefloat(v[i]->getPeso1(), v[j]->getPeso1())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso1(), v[j]->getPeso1())){
					if (!maiorQuefloat(v[i]->getPeso2(), v[j]->getPeso2())){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		} else if(direto==2) {
			if (!maiorIgualQuefloat(v[i]->getPeso2(), v[j]->getPeso2())) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(v[i]->getPeso2(), v[j]->getPeso2())){
					if (!maiorQuefloat(v[i]->getPeso1(), v[j]->getPeso1())){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		} else {
			peso_i=v[i]->getPeso1()*(g->getYl()-g->getYll())+v[i]->getPeso2()*(g->getXll()-g->getXl());
			peso_j=v[j]->getPeso1()*(g->getYl()-g->getYll())+v[j]->getPeso2()*(g->getXll()-g->getXl());
			if (!maiorIgualQuefloat(peso_i,peso_j)) {
				w[k] = v[i];
				i++;
			} else {
				if (equalfloat(peso_i, peso_j)){
					if (!maiorQuefloat(peso_i, peso_j)){
						w[k] = v[i];
						i++;
					} else {
						w[k] = v[j];
						j++;	
					}
				} else {
					w[k] = v[j];
					j++;
				}
			}
		}
		k++;
	}
	while (i < q) {
		w[k] = v[i];
		i++;
		k++;
	}
	while (j < r) {
		w[k] = v[j];
		j++;
		k++;
	}
	for (i = p; i < r; i++) v[i] = w[i-p];
	delete[] w;
}

void mergesort(Grafo *g, Aresta **v, int size, int direto){//v é um vetor de ponteiros do tipo Aresta (as arestas são ponteitos)
//implementação interativa
	int p, r, b=1;;
	while (b<size){
		p=0;
		while (p+b < size){
			r = p + 2*b;
			if (r>size) r = size;
			intercala(g, p, p+b, r, v, size, direto);
			p = p+2*b;
		}
		b = 2*b;
	}
}


bool kruskal (Grafo *g, Aresta  **A, float &x, float &y, float &custo, int direto){
	/*O parâmetro "direto" recebe:
		1 - se as arestas devem ser na ordem lexicográfica direta
		2 - se as arestas devem ser na ordem lexocográfica inversa  
		3 - se as arestas devem ser na ordem crescente de "pesos ponderado"*/
	Conjunto conjunto(g->getN());
	int cont=0, i=0;
	float peso;
	Aresta **listaAresta = g->getListaAresta();
	for (int k=0; k<g->getContArestas(); k++){ /*Adiciona as arestas obrigatórias*/
		if (g->getStatus(listaAresta[k]->getId())==1){ /*se for obrigatória*/
		 	
		 	A[cont++] = listaAresta[k];
		 	conjunto.union1(listaAresta[k]->getOrigem(), listaAresta[k]->getDestino());
			x+=listaAresta[k]->getPeso1();
			y+=listaAresta[k]->getPeso2();
			peso=listaAresta[k]->getPeso1()*(g->getYl()-g->getYll())+listaAresta[k]->getPeso2()*(g->getXll()-g->getXl());
			custo+=peso;
		}
	}
	mergesort(g, listaAresta, g->getContArestas(), direto);
	i=0;
	while (cont<g->getN()-1 && i<g->getContArestas()){ 
	/*A condição "i<g->getContArestas()" assegura que, se por acaso o grafo for desconexo, todas as arestas serão varridas, isto é, i=g->getContArestas(), porém, o cont não será será igual a g->getN()-1 */
		if (g->getStatus(listaAresta[i]->getId())==0 && !conjunto.compare(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino())){ /*Se não formar ciclo*/
			A[cont++] = listaAresta[i];
			conjunto.union1(listaAresta[i]->getOrigem(), listaAresta[i]->getDestino());
			x+=listaAresta[i]->getPeso1();
			y+=listaAresta[i]->getPeso2();
			peso=listaAresta[i]->getPeso1()*(g->getYl()-g->getYll())+listaAresta[i]->getPeso2()*(g->getXll()-g->getXl());
			custo+=peso;
		}
		i++;
	}
	conjunto.desaloca();
	
	if (cont==g->getN()-1) return true; /*grafo conexo*/
	else return false; /*grafo desconexo*/
	
}


///ALGORITMO DA SONRENSEN JANSSENS (2003)

void Partition(Grafo P, Aresta **Pa, Heap &List, ListaArvores *MSTs, int &quantMSTs, int &cont, ListaGrafos *vetorParticoes){
	//Pa = vetor de arestas = correspondente à partição P
	Grafo P1 = P, P2 = P;
	bool res = false;
	float custo, x, y;
	Aresta *a; 
	Aresta **A2;
	for (int i=0; i<P.getN()-1; i++){
		a = Pa[i];	
		if (P.getStatus(a->getId())==0){ /*Se a aresta for opcional*/
			A2 = new Aresta*[P.getN()-1];
			P1.setStatus(a->getId(), 2); /*proibida*/
			P2.setStatus(a->getId(), 1); /*obrigatória*/
			
			x=0; y=0;
			custo=0;
			res = kruskal(&P1, A2, x, y, custo, 3);
			cont++;
			if (res){
				
				MSTs->insert(A2, x, y);
				List.insert(quantMSTs, custo);
				vetorParticoes->insert(P1);
				quantMSTs++;
			} else {
				delete[] A2;
			}
			P1 = P2;
		}
	
	}
}

void AllSpaningTree(Grafo *g, ListaArvores *resul, int &contMSTs, int &cont, int k_best){ 
	Aresta **A = new Aresta*[g->getN()-1]; // usada para a primeira árvore 
	ListaArvores *MSTs = new ListaArvores(); // usada para lista de árvores
	contMSTs=0;
	float custoMinimo =0, x=0, y=0;
	Heap List(MAX1); // LEMBRAR: AQUI NÓS ESTAMOS MANIPULANDO CUSTOS DE ÁRVORES. NÃO SE PODE SABER AO CERTO QUANTAS ÁROVRES SERÃO GERADAS. AMARRA-SE MAX1 ERROR???????
	ListaGrafos *vetorParticoes = new ListaGrafos();//Parece dispensável, mas não é. Usa-se para guardar as partições por Id, e poder fornecer à função Partition. Note que List guarda somente os id e as chaves(custos das árvores)
	//A estrutura "List" do algortimo original é um heap chamada List
	bool res = kruskal(g, A, x, y, custoMinimo, 3);
	cont =1;
	if (res){
		MSTs->insert(A, x, y);
		List.insert(contMSTs, custoMinimo);
		vetorParticoes->insert(*g);
		contMSTs++;
	}
	do{ //List.getChave()<=custoMinimo
		if (List.getSize()>0){
			int id = List.getId();
			Grafo Ps;
			ElementGrafo *init = vetorParticoes->getInit();
			ElementArvore *initArvore = MSTs->getInit();
			for (int k=0; k<id; k++){ // pega o id-ésimo elemento de vetorPartições
				init = init->getNext();
				initArvore = initArvore->getNext();
			}
			Ps = init->getGrafo();
			k_best--;
			List.extract();
			if (k_best>0){
				Partition(Ps, initArvore->getArvore(), List,MSTs, contMSTs, cont,vetorParticoes);
				resul->insert(initArvore->getArvore(), initArvore->getX(), initArvore->getY());
			}
		}
	}while (List.getSize()>0);
	//cout<<"Quantidade de vezes que o Kruskal foi invocado: "<<cont<<endl;
	//cout<<"Quantidade de árvores criadas e armazenadas de fato: "<<contMSTs<<endl;
	//cout<<contMSTs<<endl;
	for (int i=0; i<contMSTs; i++){
		
			vetorParticoes->remove(vetorParticoes->getInit());
			ElementArvore *initArvore = MSTs->getInit();
			MSTs->remove(MSTs->getInit());
			delete initArvore;
	}
	
	List.desaloca();
	delete MSTs;
	delete vetorParticoes;
}


/// END ALGORITMO DA SONRENSEN JANSSENS (2003)


/// ALGORITMO DO STEINER AND RADZIK 2005

void borderSearch(ElementArvore *sl, ElementArvore *sll, Grafo *g, ListaArvores *resul, int &quantArvores){ 
// sl = s' ; sll = s''
	float x=0, y=0, custo=0;
	int cont=0;
	Aresta **listaAresta  = g->getListaAresta();
	ElementArvore *s1, *s2, *s_new;
	s1 = sl;
	s2 = sll;
	Aresta **A2;
	ListaArvores pilha;
	bool avanca = true;
	do{
		g->setYl(s1->getY());
		g->setYll(s2->getY());
		g->setXl(s1->getX());
		g->setXll(s2->getX());
		A2 = new Aresta*[g->getN()-1];
		custo =0;
		x=0;
		y=0;
		kruskal(g, A2, x, y, custo, 3);
		
		if(!((equalfloat(x, s1->getX()) && equalfloat(y, s1->getY()))||(equalfloat(x,s2->getX()) && equalfloat(y,s2->getY())))){
			s_new= new ElementArvore(A2, x, y);
			pilha.insert2(s2);
			s2 = s_new;
			avanca = true;
		} else {
			if (pilha.getInit()==NULL){ //se pilha está fazia
				
				avanca = false;
			} else {
			
				avanca = true;
				s1 = s2;
				resul->insert2(s2);
				s2 = pilha.getFim();
				pilha.remove(s2);
				quantArvores++;
			}
			delete[] A2;
		}	
	} while (avanca);
}

void phase1GM(Grafo *g, ListaArvores *resul, int &quantArvores){
	Aresta **A2;
	float x=0, y=0, custo=0;
	ElementArvore *s1, *s2;
	A2 = new Aresta*[g->getN()-1];
	kruskal(g, A2, x, y, custo, 1);
	s1= new ElementArvore(A2, x, y);
	resul->insert2(s1);
	quantArvores++;
	custo=0; x=0; y=0;
	A2 = new Aresta *[g->getN()-1];
	kruskal(g, A2, x, y, custo, 2);
	s2 = new ElementArvore(A2, x, y);
	if (!(equalfloat(s2->getX(),s1->getX()) && equalfloat(s2->getY(),s1->getY()))){
		borderSearch(s1, s2, g, resul, quantArvores);
		resul->insert2(s2);
		quantArvores++;
	}
	
}


/*!
 * A função retorna apenas o conjunto de soluções não suportadas, bem como sua cardinalidade.
 * Assim, podemos ter a separação entre soluções suportadas e não suportadas.
 * Obs.: A função phase1GM(...) já retorna a lista de aresta suportadas em ordenação lexicográfica (de acordo
 * com os objetivos)
*/


//LEMBRAR: Para separar em suportadas e não suportadas, colocar uma variável bool em ElementArvore


void phase2KB(Grafo *g, ListaArvores *resul, int &quantArvores ){
	ElementArvore *p = resul->getInit(); /*Ponteiro que percorre a lista de soluções suportadas*/
	ElementArvore *q = p->getNext();
	ElementArvore *aux, *anti, *varre;
	ListaArvores *k_best_tree;
	int k; /*k-ésima melhor árvore*/
	int cont=0, contMST=0;  /*Futuramente necessários para dados estatísticos*/
	float a, b; /*Necessário para construir a equação da reta de custo máximo, da forma ax + b = y*/
	float br, cr;
	float m; /*coeficiente angular da reta r*/
	float distancia; /*representa a distância de um ponto e a reta r (custo máximo)*/
	bool otima, pare;
	/*!
  	 * p e q definem um trigângulo. Todas as ávores não-suportadas encontradas são inseridas entre p e q na 	 * lista 'resul'. Para a sub-lista SL = 'p .... q', encontramos a região viável a partir da definição
  	 * dos triângulos formados entre dois elementos consecutivos u e v de SL. Tais triângulos pelos pontos 
  	 * u=(ux, uy), v=(vx, vy) e (vx, uy); 
	 */
	//while (p->getNext()!=resul->getInit()){
	while (q!=resul->getInit()){
		//q = p->getNext();
		
		g->setYl(p->getY());
		g->setYll(q->getY());
		g->setXl(p->getX());
		g->setXll(q->getX());	
		
		k=1;
		/* Inicialmente a região viável é o próprio triângulo entre p e q
		 * A reta de custo máximo, inicialmente, passará pelos pontos (qx, py)
		 */
		
		
		m = (p->getY()-q->getY())/(p->getX()-q->getX()); // coeficiente angular da reta de custo máximo
		b = p->getY() - q->getX()*m; // a reta de custo máximo é paralela à r (entre p e q)
		a = m; // ax + b = y (reta de custo máximo);
		
		br = -1;
		cr = q->getY() - m*q->getX(); /*Coefieciente lienar da reta r que une p e q*/
		distancia = abs(a*q->getX() + br*p->getY() + cr)/sqrt(a*a + br*br); // distância de (qx, py) a r
		pare = false; // false = não para o laço
		
		/*! OBS.: O valor passado em AllSpaningTree (útimo parâmetro) é estimado, de modo a diminuir a complexidade do algortimo. LEMBRAR: fazer muitos testes, estimá-lo a partir de k e do número de vértices de G*/
		k_best_tree = new ListaArvores();
		Grafo gg = *g;	
		
		AllSpaningTree(g, k_best_tree,contMST, cont, 100); 
		*g = gg;
		varre = k_best_tree->getInit();	
		do{ 
			if (pare == false){
				/*! Testes de dominância*/
				/*! Verifica se a solução corrent pertence à região viável*/
				aux = p;
				anti = p;
				otima = false; // false = árvore não é ótima
				float dis, auxb;
				float maxDistance =0;
				while (aux!=q){ //varre todos os triângulos
					if (!maiorIgualQuefloat(varre->getX(), aux->getNext()->getX()) && !maiorIgualQuefloat(varre->getY(), aux->getY())){
						otima=true;
						anti = aux;
					}
					dis = abs(a*aux->getNext()->getX() + br*aux->getY() + cr)/sqrt(a*a + br*br);
					if (dis>maxDistance){	
						maxDistance=dis;
						auxb = aux->getY() - a*aux->getNext()->getX();
					}
					aux = aux->getNext();
				
				}
				if (otima){ //se a solução estiver na região viável
					
					aux = varre->getPrevious();
					k_best_tree->remove(varre);
					varre->setNext(anti->getNext());
					anti->getNext()->setPrevious(varre);
					anti->setNext(varre);
					varre->setPrevious(anti);
					quantArvores++;
					/*! Nova linha de custo máximo*/
					dis = abs(a*anti->getNext()->getX() + br*anti->getY() + cr)/sqrt(a*a + br*br); // entre anti e a nova árove
					if (dis>maxDistance){	
						maxDistance=dis;
						auxb = aux->getY() - a*aux->getNext()->getX();
					} 
					dis = abs(a*varre->getNext()->getX() + br*varre->getY() + cr)/sqrt(a*a + br*br); // entre a nova árvore e a próxima
					if (dis>maxDistance){	
						maxDistance=dis;
						auxb = aux->getY() - a*aux->getNext()->getX();
					}
					
					b =  auxb;
					varre = aux;
					
					
				} else {
					//verificar se o solução corrent está acima ou sobre a reta de custo máximo
					float y = a*varre->getX() + b;
					if (maiorQuefloat(y, varre->getY())==false) pare = true; /*manda parar*/
					aux = varre->getPrevious();
					k_best_tree->remove(varre);
					delete[] varre->getArvore();
					delete varre;
					
					varre = aux; 
					 	
				}
				cout<<k<<endl;		
			} else {
				aux = varre->getPrevious();
				k_best_tree->remove(varre);
				delete[] varre->getArvore();
				delete varre;
				varre = aux;
			} 
			k++;
			varre = varre->getNext();
		} while (varre!=NULL && k<100);
		delete k_best_tree;
		p = q;
		q = p->getNext();
		
	
		
	}
}


/// END ALGORITMO DO STEINER AND RADZIK 2005


int main(){
	/*medição do tempo*/	
	struct timeval  inicio, fim;
    	long double mili, seg, micro;
	
	/* Coleta a data de inicio do processo */
	gettimeofday(&inicio, NULL);
	/*end medição do tempo*/
	
	ifstream file("Grafic_30_Integer.txt");
	ofstream output("New_Resul_30_Integer.txt");
	output.precision(10);
	int n, origem, destino, cont=0, quantArvores=0, quantNotSuportadas=0;
	float peso1, peso2; 
	file>>n;
	Grafo *g = new Grafo(n);
	int i=0;
	for (i=0; i<n; i++){	
		g->addVertice(i);
	}
	ListaArvores *resul = new ListaArvores();
	i=0;
	while (file){
		file>>origem;
		file>>destino;
		file>>peso1;
		file>>peso2;
		if (origem!=-1){
			Aresta  nova = g->addAresta(i,origem-1, destino-1, peso1, peso2);
			i++;
		}
	}
	output<<"=============================================================="<<endl;
	output<<"Implementação do algortimo de Steiner and Radzik (2003)\n"<<endl;
	output<<"Entrada: Grafo G(N,E), onde N é o conjunto de nós e E é o conjunto de arestas\n"<<endl;
	output<<"Saída: Conjunto solução e conjunto objetivo referentes ao problema da AGMB\n"<<endl;
	output<<"=============================================================="<<endl;
	output<<"|N| = "<<n<<"\t";
	output<<"|E| = "<<i<<endl;
	output<<"\nLista de Arestas sem ordenação\n"<<endl;
	Aresta **listaAresta = g->getListaAresta();
	for (int i=0; i<g->getContArestas(); i++){
		output<<"ArcList["<<i<<"] arc ["<<listaAresta[i]->getOrigem()<<", "<<listaAresta[i]->getDestino()<<"] cost ("<<listaAresta[i]->getPeso1()<<", "<<listaAresta[i]->getPeso2()<<")"<<endl;
				
	}

	phase1GM(g, resul, quantArvores);
	/*output<<"Quantidade de árvores Suportadas: "<<quantArvores<<endl;*/
	/*ElementArvore *init = resul->getInit();
	do{
		if (init!=NULL){
			Aresta **A = init->getArvore();
			//for (int i=0; i<n-1; i++){
				//output<<"("<<A[i]->getOrigem()<<", "<<A[i]->getDestino()<<")"<<"\t";
			//}
			rascunho<<"Espaço objetivo: ("<<init->getX()<<", "<<init->getY()<<")"<<endl;
			//output<<endl;
			init = init->getNext();
		}
	} while (init!=resul->getInit());
	*/
	phase2KB(g, resul, quantArvores);
	
	
	output<<"\nQuantidade de árvores: "<<quantArvores<<endl;
	output<<"\nEspaço objetivo resultante:\n "<<endl;
	ElementArvore *init = resul->getInit();
	cont=0;
	do{
		if (init!=NULL){
			output<<"MSTree "<<cont<<" Custo = ("<<init->getX()<<", "<<init->getY()<<")"<<endl;
			init = init->getNext();
			cont++;
		}
	} while (init!=resul->getInit());
	
	output<<"\nEspaço solução resultante:\n "<<endl;
	init = resul->getInit();
	cont=0;
	float x, y;
	do{
		if (init!=NULL){
			x =0;
			y=0;
			Aresta **A = init->getArvore();
			output<<"MSTree "<<cont<<" = [";
			for (int j=0; j<g->getN()-1; j++){
				output<<"("<<A[j]->getOrigem()+1<<", "<<A[j]->getDestino()+1<<") ";
				x+=A[j]->getPeso1();
				y+=A[j]->getPeso2();
			}
			output<<"] --> ("<<x<<", "<<y<<")"<<endl;
			init = init->getNext();
			cont++;
		}
	} while (init!=resul->getInit());
	
	
	
	gettimeofday(&fim, NULL);
	seg = (fim.tv_sec - inicio.tv_sec)*1000;
	micro = (fim.tv_usec - inicio.tv_usec)/1000;
	if (seg>100){ //em milisegundos
		if (seg>1000000){
			mili = seg+micro;
			output<<"\nTempo consumido (em milisegundos) :"<<mili<<endl;
		} else {
			seg = seg/1000;
			micro = micro/1000;
			mili = seg+micro;
			output<<"\nTempo consumido (em segundos) :"<<mili<<endl;
		}
	} else { //em microsegundos
		seg = seg*1000;
		micro = micro*1000;
		mili = seg+micro;
		output<<"\nTempo consumido (em microsegundos) :"<<mili<<endl;
	}
	/*cout<<"Grafo: "<<sizeof(Grafo)<<endl;
	cout<<"ElementArvore: "<<sizeof(ElementArvore)<<endl;
	cout<<"Vertice: "<<sizeof(Vertice)<<endl;
	cout<<"Aresta: "<<sizeof(Aresta)<<endl;
	cout<<"Heap: "<<sizeof(Heap)<<endl;
	cout<<"Conjunto: "<<sizeof(Conjunto)<<endl;*/	
	//ListaAresta *matriz = new ListaAresta[100000];
	return 0;
}
