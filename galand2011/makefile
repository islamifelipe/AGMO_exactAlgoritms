
all: Aresta.o Vertice.o Grafo.o ranking

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
ranking.o : ranking.cpp
	g++ -g -c  ranking.cpp
		
ranking : ranking.o  Grafo.o Vertice.o Aresta.o   
	g++ -g  Aresta.o  Vertice.o Grafo.o ranking.o -lm -o ranking
	