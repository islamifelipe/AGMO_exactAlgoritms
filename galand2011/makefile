
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o Heap.o  ranking

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
ranking.o : ranking.cpp
	g++ -g -c  ranking.cpp

lambda.o : lambda.cpp
	g++ -g -c  lambda.cpp

Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp

Heap.o : Heap.cpp Heap.h
	g++ -g -c Heap.cpp

		
ranking : ranking.o  Grafo.o Vertice.o Aresta.o   lambda.o ranking.o Conjunto.o kruskal.o Heap.o 
	g++ -g  Aresta.o  Vertice.o Grafo.o ranking.o lambda.o Conjunto.o kruskal.o Heap.o  -lm -o ranking
	