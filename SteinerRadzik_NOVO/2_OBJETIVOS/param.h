#ifndef ZG_H
#define ZG_H

#include "rand64/mersenne64.c"


#define PI 3.14159265
#define f(k,i,j) custos[k][i][j] // objetivo k, vertice i j 
#define EPS 1e-9 // qualquer coisa menor que esse valor, é considerado 0
#define PROFUNDIDADEGRID 5 
#define NUMOBJETIVOS 2
#define NUMEROVERTICES 5
#define NUMARESTAS ((NUMEROVERTICES*(NUMEROVERTICES-1))/2)
#define PRECISAO 0.00001

#endif