#ifndef _HEAP_H_
#define _HEAP_H_

#include <utility>      // std::pair
#include "Grafo.h"
#include <vector>
/*Utilizado pelo algoritmo do SONRENSEN JANSSENS*/
class Heap {
	private:
		int size; /*tamanho atual (parcial) da heap*/
		map< int, pair<float, int* > > heap; //Guarda as chaves a serem ordenadas, e o respectivo MST e particao
		map<int, Grafo> auxGrafo;
		//int *auxHeap; // Guarda os id das arestas
		//int *auxID; 
		int maximo;
	
	public: Heap(int maximo1);
	private:
		void subir(int i);
		void descer(int i);
	public:
		bool insert (float chave, int * MST, Grafo particao);
	
	
	/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
	 	bool extract (); /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
			
	
		float getChave();
		//int getId();
		int getSize();
		void desaloca();

		int *  getMST();

		Grafo getParticao();

};
#endif