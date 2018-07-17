
all: Aresta.o Vertice.o Grafo.o Conjunto.o dominanceRelation kruskal 

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c Conjunto.cpp

dominanceRelation.o : dominanceRelation.cpp
	g++ -g -c dominanceRelation.cpp

kruskal_4obj.o : kruskal_4obj.cpp
	g++ -g -c kruskal_4obj.cpp

kruskal : kruskal_4obj.o  Grafo.o Vertice.o Aresta.o Conjunto.o  
	g++ -pthread -g  Aresta.o  Vertice.o Grafo.o Conjunto.o kruskal_4obj.o -lm -o kruskal


dominanceRelation : dominanceRelation.o Grafo.o Vertice.o Aresta.o 
	g++ -g  Aresta.o  Vertice.o Grafo.o dominanceRelation.o -lm -o dominanceRelation

