 

#include "Heap.h"
#include <cmath>
using namespace std;

#define PRECISAO 0.00001

bool Heap::equalfloat(float a, float b){
	return abs(a-b)<=PRECISAO;
	
}

bool Heap::maiorQuefloat(float a, float b){ //retorna true se a>b
	return a-b>PRECISAO;
}

bool Heap::maiorIgualQuefloat(float a, float b){ //returna true se a>=b
	return maiorQuefloat(a, b)||equalfloat(a, b);	
}
 Heap::Heap(){ 
	//maximo = maximo1; // REDUNDANDTE
	//heap = new float[maximo];
	//auxHeap = new int[maximo];
	//auxID = new int[maximo];	
	heap.push_back(make_pair(0,0));	
	// Lembrar: A posição 0 do heap é ociosa
	// for (int i=0; i<maximo; i++){
	// 	auxID[i] = 0;
	// 	auxHeap[i] = -1;
	// }
	size = 0;
}

void  Heap::subir(int i){ 
	int pai = i/2;
	if (pai>0){ /*vetor com size+1 posições*/
		if (!maiorIgualQuefloat(heap[pai].second,heap[i].second)==true){
			pair <int, float> aux1 = heap[i];
			heap[i] = heap[pai];
			heap[pai] = aux1;
				
			// int aux = auxHeap[i];
			// auxHeap[i] = auxHeap[pai];
			// auxHeap[pai] = aux;
					
			subir(pai);
		}				
	}
	//if (auxHeap[i]>=0) auxID[auxHeap[i]] = i;
}
void  Heap::descer(int i){
		int filho = i*2;
		if (filho<=size){
			if (filho!=size){ 
				if (!maiorQuefloat(heap[filho+1].second,heap[filho].second)==false) filho++;
			}
			if (!maiorIgualQuefloat(heap[i].second,heap[filho].second)==true){
				pair <int, float>  aux1 = heap[i];
				heap[i] = heap[filho];
				heap[filho] = aux1;
				
				// int aux = auxHeap[i];
				// auxHeap[i] = auxHeap[filho];
				// auxHeap[filho] = aux;
			
				descer(filho);
			}
		}
		//if (auxHeap[i]>=0)auxID[auxHeap[i]] = i;
	}

bool  Heap::insert (int id, float chave){ /*insere no final e sobe o elemento*/
	if (size+1<heap.size()+2){//&& auxHeap[auxID[id]]!=id){
		pair <int, float> novo = make_pair(id, chave);
		heap.push_back(novo);
	//	auxHeap[size+1] = id;
		size++;
		subir(size);
		return true;
	} else return false;
}
	
/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
bool  Heap::extract (){ /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
	if (size>0){
		heap[1] = heap[size];
		heap.erase (heap.begin()+heap.size()-1);
		// int id = auxHeap[1];
		// auxHeap[1] = auxHeap[size];
		descer(1);
		size--;
		//auxID[id] = 0;
		return true;
	} else return false;
}
	
float  Heap::getChave(){
	return heap[1].second;
}

int  Heap::getId(){
	return heap[1].first;
}

int  Heap::getSize(){
	return size;
}


