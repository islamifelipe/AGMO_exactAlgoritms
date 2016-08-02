
all: Aresta.o Vertice.o Grafo.o Conjunto.o steinerRadzik

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp
	g++ -g -c  Conjunto.cpp
	
main.o : main.cpp
	g++ -g -c  main.cpp
		
steinerRadzik : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o  
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o main.o -lm -o steinerRadzik
	