#ifndef LEXKRUSKAL_CPP
#define LEXKRUSKAL_CPP


#include "param.h"
#include "SolucaoEdgeSet.cpp"
#include "auxEdgeStruct.h"
#include "UnionFind.cpp"
#include <iostream>
using namespace std;

class LexKruskal {
private:
	UnionFind uf;

public:
	LexKruskal() {}

	void executar(SolucaoEdgeSet &s, auxEdgeStruct arestas []) {

		for(int i = 0; i < NUMOBJETIVOS; i++) {
			s.setObj(i,0.0);
		}

		uf.clear();
		
		// coloca NUMEROVERTICES-1 arestas do grafo sem formar ciclo
		int cont = 0, edge = 0;
		while (cont < NUMEROVERTICES-1) {

			// anda ate a proxima aresta que pode ser inserida
			while (uf.sameClass(arestas[edge].a,arestas[edge].b)) edge++;

			// coloca a aresta na solucao
			s.edges[cont][0] = arestas[edge].a;
			s.edges[cont][1] = arestas[edge].b;
			s.setObj(0,s.getObj(0)+f(0,s.edges[cont][0],s.edges[cont][1]));
			s.setObj(1,s.getObj(1)+f(1,s.edges[cont][0],s.edges[cont][1]));
			uf.unionClass( arestas[edge].a, arestas[edge].b );
			cont++;
		}
		// assert (s.confereArestas());
	}


	// distingue entre proibidas e obrigatorias
	// arestas[] ja devem vir ordenadas
	bool executar_particao(SolucaoEdgeSet &s, auxEdgeStruct arestas []) {

		for(int i = 0; i < NUMOBJETIVOS; i++) s.setObj(i,0.0);

		uf.clear();
			
		int cont = 0;

		// insere as obrigatórias
		for (int k=0; k<NUMARESTAS; k++){ // garantido que as arestas obrigatorias nao formam ciclo
			if (s.status[arestas[k].a][arestas[k].b] == 1){
				s.edges[cont][0] = arestas[k].a; // origem
				s.edges[cont][1] = arestas[k].b; // destino
				s.setObj(0,s.getObj(0)+f(0,s.edges[cont][0],s.edges[cont][1]));
				s.setObj(1,s.getObj(1)+f(1,s.edges[cont][0],s.edges[cont][1]));
				uf.unionClass( arestas[k].a, arestas[k].b );
				cont++;
			}
		}

		
		int edge = 0;
		while (cont < NUMEROVERTICES-1 && edge<NUMARESTAS) {

			if (s.status[arestas[edge].a][arestas[edge].b]==0 && uf.sameClass(arestas[edge].a,arestas[edge].b)==false){
			// coloca a aresta na solucao
				s.edges[cont][0] = arestas[edge].a;
				s.edges[cont][1] = arestas[edge].b;
				s.setObj(0,s.getObj(0)+f(0,s.edges[cont][0],s.edges[cont][1]));
				s.setObj(1,s.getObj(1)+f(1,s.edges[cont][0],s.edges[cont][1]));
				uf.unionClass( arestas[edge].a, arestas[edge].b );
				cont++;
			}
			edge++;
		}


		if (cont==NUMEROVERTICES-1) return true; /*grafo conexo*/
		else return false; /*grafo desconexo*/

	}
};

#endif

