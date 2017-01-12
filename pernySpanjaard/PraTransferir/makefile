
all: Aresta.o Vertice.o Grafo.o Conjunto.o dominanceRelation lexicographicRelation kruskal prim main_prim_pareto main_kruskal_pareto

Aresta.o : Aresta.cpp Aresta.h
	g++ -g -c Aresta.cpp
	
Vertice.o : Vertice.cpp Vertice.h
	g++ -g -c Vertice.cpp
	
Grafo.o : Grafo.cpp Grafo.h
	g++ -g -c Grafo.cpp
	
Conjunto.o : Conjunto.cpp Conjunto.h
	g++ -g -c Conjunto.cpp

main_kruskal.o : main_kruskal.cpp
	g++ -pthread -g -c  main_kruskal.cpp

dominanceRelation.o : dominanceRelation.cpp
	g++ -g -c dominanceRelation.cpp

lexicographicRelation.o : lexicographicRelation.cpp
	g++ -g -c lexicographicRelation.cpp

main_prim.o : main_prim.cpp
	g++ -pthread -g -c main_prim.cpp

main_prim_pareto.o : main_prim_pareto.cpp
	g++ -g -c main_prim_pareto.cpp

main_kruskal_pareto.o : main_kruskal_pareto.cpp
	g++ -g -c main_kruskal_pareto.cpp

kruskal : main_kruskal.o  Grafo.o Vertice.o Aresta.o Conjunto.o  
	g++ -pthread -g  Aresta.o  Vertice.o Grafo.o Conjunto.o main_kruskal.o -lm -o kruskal

prim : main_prim.o  Grafo.o Vertice.o Aresta.o   
	g++ -pthread -g  Aresta.o  Vertice.o Grafo.o main_prim.o -lm -o prim

main_kruskal_pareto : main_kruskal_pareto.o  Grafo.o Vertice.o Aresta.o Conjunto.o  
	g++ -g  Aresta.o  Vertice.o Grafo.o Conjunto.o main_kruskal_pareto.o -lm -o main_kruskal_pareto

main_prim_pareto : main_prim_pareto.o  Grafo.o Vertice.o Aresta.o
	g++ -g  Aresta.o  Vertice.o Grafo.o main_prim_pareto.o -lm -o main_prim_pareto


dominanceRelation : dominanceRelation.o Grafo.o Vertice.o Aresta.o 
	g++ -g  Aresta.o  Vertice.o Grafo.o dominanceRelation.o -lm -o dominanceRelation

lexicographicRelation : lexicographicRelation.o Grafo.o Vertice.o Aresta.o 
	g++ -g  Aresta.o  Vertice.o Grafo.o lexicographicRelation.o -lm -o lexicographicRelation
