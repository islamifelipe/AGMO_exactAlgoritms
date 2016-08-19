#ifndef _HEAP_H_
#define _HEAP_H_

/*Utilizado pelo algoritmo do SONRENSEN JANSSENS*/
#include <utility>      // std::pair
#include <vector>
class Heap {
	private:
		int size; /*tamanho atual (parcial) da heap*/
		 std::vector < std::pair <int, float> > heap; //Guarda os ids e as chaves a serem ordenadas
		//int *auxHeap; // Guarda os id das arestas
		//int *auxID; 
		
	
	public: Heap();
	private:
		void subir(int i);
		void descer(int i);

		bool equalfloat(float a, float b);

		bool maiorQuefloat(float a, float b);

		bool maiorIgualQuefloat(float a, float b);
	public:
		bool insert (int id, float chave);
	
	
	/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
	 	bool extract (); /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
			
	
		float getChave();
		int getId();
		int getSize();
};
#endif