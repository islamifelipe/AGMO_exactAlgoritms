
all: Aresta.o Vertice.o Grafo.o santos

Aresta.o : Aresta.cpp Aresta.h
	g++ -std=c++11 -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -std=c++11 -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -std=c++11 -g -c Grafo.cpp
	
main.o : main.cpp
	g++ -std=c++11 -lpthread -g -c  main.cpp
		
santos : main.o  Grafo.o Vertice.o Aresta.o   
	g++ -std=c++11 -lpthread -g  Aresta.o  Vertice.o Grafo.o main.o -lm -o santos
	