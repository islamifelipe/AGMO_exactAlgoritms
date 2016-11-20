 

#include "Heap.h"
#include "kruskal.h"
#include <utility>      // std::pair
#include <map> 
#include <iostream>
using namespace std;

 Heap::Heap(int maximo1){ 
	maximo = maximo1;
	//auxHeap = new int[maximo];
	//auxID = new int[maximo];		
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
		if (maiorQuefloat(heap[pai].first,heap[i].first)==true){
			// float aux_float = heap[i].first;
			// int *aux_int = heap[i].second.first;
			// Grafo aux_grafo = heap[i].second.second; 	

			pair<float, int *>   aux1 = heap[i];
			heap[i] = heap[pai];
			heap[pai] = aux1;

			Grafo gaux = auxGrafo[i];
			auxGrafo[i] = auxGrafo[pai];
			auxGrafo[pai] = gaux;
			
					
			subir(pai);
		}				
	}
	//if (auxHeap[i]>=0) auxID[auxHeap[i]] = i;
}
void  Heap::descer(int i){
		int filho = i*2;
		if (filho<=size){
			if (filho!=size){ 
				if (maiorIgualQuefloat(heap[filho+1].first,heap[filho].first)==false) filho++;
			}
			if (maiorQuefloat(heap[i].first,heap[filho].first)==true){
				pair<float, int* >  aux1 = heap[i];
				heap[i] = heap[filho];
				heap[filho] = aux1;

				Grafo auxg = auxGrafo[i];
				auxGrafo[i] = auxGrafo[filho];
				auxGrafo[filho] = auxg;
				// int aux = auxHeap[i];
				// auxHeap[i] = auxHeap[filho];
				// auxHeap[filho] = aux;
			
				descer(filho);
			}
		}
		// //if (auxHeap[i]>=0)auxID[auxHeap[i]] = i;
	} 

bool  Heap::insert (float chave, int * MST, Grafo particao){ /*insere no final e sobe o elemento*/
	if (size+1<maximo) {//&& auxHeap[auxID[id]]!=id){
 		heap[size+1] = make_pair(chave, MST);//=chave;
 		auxGrafo[size+1] = particao;
		
		//auxHeap[size+1] = id;
		size++;
		subir(size);
		return true;
	} else return false;
}
	
/*ESTA FUNÇÃO NÃO RETORNA OS ELEMENTOS MIM. ELA SUMPÕE QUE OS MESMOS FORAM TOMANDOS IMEDIATAMENTE ANTES DA CHAMADA DA FUNÇÃO*/
bool  Heap::extract (){ /*pega o último elemento e coloca no lugar do primeiro, depois desce ele.*/
	if (size>0){
		heap[1] = heap[size];
		// int id = auxHeap[1];
		// auxHeap[1] = auxHeap[size];
		auxGrafo[1] = auxGrafo[size];
		descer(1);
		size--;
		//auxID[id] = 0;
		return true;
	} else return false;
}  
	
float  Heap::getChave(){
	return heap[1].first;
}

int *  Heap::getMST(){
	return (heap[1].second);
}

Grafo Heap::getParticao(){
	return auxGrafo[1];
	//return ((heap[1].second).second);
}

// int  Heap::getId(){
// 	return auxHeap[1];
// }

int  Heap::getSize(){ 
	return size;
}

void  Heap::desaloca(){
	// delete[] auxHeap;
	// delete[] heap;
	// delete[] auxID; 
}
