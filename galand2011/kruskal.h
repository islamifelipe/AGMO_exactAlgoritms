#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_
#include "Grafo.h"

//bool equalfloat(float a, float b);


//bool maiorQuefloat(float a, float b);

//bool maiorIgualQuefloat(float a, float b);

//void intercala(float lambda1, float lambda2, int p, int q, int r, Aresta **v, int size, int direto);
//void mergesort(float lambda1, float lambda2, Aresta **v, int size, int direto);
bool kruskal (Grafo *g, int  *A, Aresta **listaAresta, float lambda1, float lambda2, float &custo, float &x, float &y, int direto);

#endif