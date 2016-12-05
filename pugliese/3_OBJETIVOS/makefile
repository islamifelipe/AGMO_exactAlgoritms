
all: Aresta.o Vertice.o Grafo.o pugliese

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
main.o : main.cpp
	g++ -pthread -g -c  main.cpp
		
pugliese : main.o  Grafo.o Vertice.o Aresta.o   
	g++ -pthread -g  Aresta.o  Vertice.o Grafo.o main.o -lm -o pugliese
	