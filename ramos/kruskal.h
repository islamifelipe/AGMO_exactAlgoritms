#ifndef _KRUSKAL_H_
#define _KRUSKAL_H_
#include "Grafo.h"
#define PRECISAO 0.00001

bool equalfloat(float a, float b);


bool maiorQuefloat(float a, float b);

bool maiorIgualQuefloat(float a, float b);

void intercala(float xl, float yl, float xll, float yll, int p, int q, int r, Aresta **v, int size, int direto);
void mergesort(float xl, float yl, float xll, float yll, Aresta **v, int size, int direto);
bool kruskal (Grafo *g, int  *A, float xl, float yl, float xll, float yll,float &custo, int direto);

#endif