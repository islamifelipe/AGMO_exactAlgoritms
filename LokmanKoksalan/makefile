
all: Aresta.o Vertice.o Grafo.o Conjunto.o Heap.o algoritm1 lokmanKoksalanWithBB

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c Conjunto.cpp

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp
	
lokmanKoksalan.o : lokmanKoksalan.cpp
	g++ -g -c  lokmanKoksalan.cpp

lokmanKoksalanWithBB.o : lokmanKoksalanWithBB.cpp
	g++ -g -c lokmanKoksalanWithBB.cpp
		
algoritm1 : lokmanKoksalan.o  Grafo.o Vertice.o Aresta.o  Heap.o Conjunto.o 
	g++ -g  Aresta.o  Vertice.o Grafo.o Heap.o Conjunto.o  lokmanKoksalan.o -lm -o algoritm1

lokmanKoksalanWithBB : lokmanKoksalanWithBB.o  Grafo.o Vertice.o Aresta.o  Conjunto.o
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o lokmanKoksalanWithBB.o -lm -o lokmanKoksalanWithBB
