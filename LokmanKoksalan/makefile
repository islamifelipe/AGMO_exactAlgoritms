
all: Aresta.o Vertice.o Grafo.o algoritm1

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
lokmanKoksalan.o : lokmanKoksalan.cpp
	g++ -g -c  lokmanKoksalan.cpp
		
algoritm1 : lokmanKoksalan.o  Grafo.o Vertice.o Aresta.o   
	g++ -g  Aresta.o  Vertice.o Grafo.o lokmanKoksalan.o -lm -o algoritm1
	