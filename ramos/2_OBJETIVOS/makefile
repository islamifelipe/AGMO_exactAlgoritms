
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o  Heap.o  ramos AllMST

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp

Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp

main.o : main.cpp
	g++ -g -c  main.cpp
		
ramos : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o  Heap.o   
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o  Heap.o  main.o -lm -o ramos

AllMST.o : AllMST.cpp
	g++ -pthread -g -c AllMST.cpp

AllMST : AllMST.o Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o   
	g++ -pthread -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o AllMST.o -lm -o AllMST
