#include "lambda.h"
#include <utility>      // std::pair
#include <climits>

using namespace std;

/*Algoritmo 1 de Galand et al (2010) que computa o lambda estrela
Como estamos tratando com bi-objetivo, retornamos apenas um par para lambda do primeiro e do segundo objetivo,
Esta implementacao é exclusiva para o caso Bi-objetivo
Portanto, N = {1, 2} P(N) = {0,{1},{2},{1,2}}. Nao ha necessidade de se fazer laços
algorithm1 recebe como parâmetro a capacidade v, representada por v[0] = vazio, v[1] = v({1}), v[2] = v({2}), v[3] = v({1, 2}),  */
pair<float , float> algorithm1(float v[4]){

	float c1, c2;
	int A[2] = {0, 0};
	int B[2] = {0, 0};
	int sizeB = 0; /* deve ser menor que 2 */

	/*primeira iteracao : B vazio*/
	float min = INT_MAX;
	float va = v[3]/2;//{1, 2}
	if (va<min){
		A[0] = 1; // 1
		A[1] = 1; // 2
		sizeB = 2;
		min = va;
	}
	
	va = v[1];// {1}
	if (va<min){
		A[0] = 1; // 1
		A[1] = 0; // 2
		sizeB = 1;
		min = va;
	}

	va = v[2];// {2}
	if (va<min){
		A[0] = 0; // 1
		A[1] = 1; // 2
		sizeB = 1;
		min = va;
	}
	
	if (A[0] == 1) { // {1}
		B[0] = 1;
		c1 = min;
	}
	if (A[1] == 1) {// {2}
		B[1] = 1;
		c2 = min;
	}
	A[0] = 0;
	A[1] = 0;
	if (sizeB == 1){
		min = INT_MAX;
		if (B[0] == 0){ // {1}
			va = v[3] - v[2];
			if (va<min){
				A[0] = 1; // 1
				sizeB = 2;
				min = va;
			}
		} else if (B[1] == 0){ // {2}
			va = v[3] - v[1];
			if (va<min){
				A[1] = 1; // 2
				sizeB = 2;
				min = va;
			}
		}
	}

	if (A[0] == 1) { // {1}
		B[0] = 1;
		c1 = min;
	}
	if (A[1] == 1) {// {2}
		B[1] = 1;
		c2 = min;
	}
	
	pair<float, float> ret = make_pair(c1, c2);
	return ret;
	
}