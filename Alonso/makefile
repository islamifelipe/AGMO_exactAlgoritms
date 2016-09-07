
all: Aresta.o Vertice.o Grafo.o Conjunto.o Kruskal.o kruskal prim

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp

Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c Conjunto.cpp
	
main_kruskal.o : main_kruskal.cpp
	g++ -g -c  main_kruskal.cpp

dominanceRelation.o : dominanceRelation.cpp
	g++ -g -c dominanceRelation.cpp

main_prim.o : main_prim.cpp
	g++ -g -c main_prim.cpp

Kruskal.o : Kruskal.cpp
	g++ -g -c Kruskal.cpp

kruskal : main_kruskal.o  Grafo.o Vertice.o Aresta.o Conjunto.o  Kruskal.o
	g++ -g  Aresta.o  Vertice.o Grafo.o main_kruskal.o  Conjunto.o Kruskal.o -lm -o kruskal

prim : main_prim.o  Grafo.o Vertice.o Aresta.o   
	g++ -g  Aresta.o  Vertice.o Grafo.o main_prim.o -lm -o prim
