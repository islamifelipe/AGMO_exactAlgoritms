#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 
#define NUMOBJETIVOS 2
#define NUMEROVERTICES 100
#define NUMARESTAS ((NUMEROVERTICES*(NUMEROVERTICES-1))/2)
#define PRECISAO 0.00001
#define MAX_K_BEST 260000//6000000//240000// 200000 - 3000000 // este valor representa somente a k-ésima best árvore encontrada e NAO O TAMANHO DA HEAP
#define MAX_SIZE_HEAP 260000
// anti 260000
// conc 6000000
#endif