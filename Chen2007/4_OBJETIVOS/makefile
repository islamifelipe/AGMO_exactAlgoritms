
all: Aresta.o Vertice.o Grafo.o chen

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
main.o : main.cpp
	g++ -lpthread -g -c  main.cpp
		
chen : main.o  Grafo.o Vertice.o Aresta.o   
	g++ -lpthread -g  Aresta.o  Vertice.o Grafo.o main.o -lm -o chen
	