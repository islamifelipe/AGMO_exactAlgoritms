
all: Aresta.o Vertice.o Grafo.o Conjunto.o kruskal.o ramos

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c  Conjunto.cpp
	
kruskal.o : kruskal.cpp kruskal.h
	g++ -g -c kruskal.cpp

main.o : main.cpp
	g++ -g -c  main.cpp
		
ramos : main.o  Grafo.o Vertice.o Aresta.o Conjunto.o kruskal.o   
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal.o main.o -lm -o ramos
	