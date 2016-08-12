
all: Aresta.o Vertice.o Grafo.o ranking

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
		
ranking : ranking.o  Grafo.o Vertice.o Aresta.o   lambda.o ranking.o
	g++ -g  Aresta.o  Vertice.o Grafo.o ranking.o lambda.o -lm -o ranking
	