/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Knowles's (2002) graph generator, 
# to generate the correlated and anticorrelated grid graphs  
#=======================================================================
*/

#include <iostream>
#include <stdlib.h>     /* atoi */
#include <time.h>       /* time */

using namespace std;

int main (int argc, char *argv[]){

	int linha=0, coluna=0;
	double alpha;
	linha = atoi(argv[1]);
	coluna = atoi(argv[2]);
	alpha =  atof(argv[3]);
	cout<<linha*coluna<<endl; // quantidade de vertices
	srandom (time(NULL));
	//float teste = ((float)(random())/(float)(RAND_MAX) ) ;

	//int teste2 = ((random()%2) + 1) ;
	
	double betha, gama;
	betha = 0.5*((double)(1-alpha)); // Correçao do Chen et al (2007)
	if (alpha>=0){
		gama = betha;
	} else{
		gama = 1 - betha;
	}
	//cout<<"betha = "<<betha<<" gama = "<<gama<<endl;
	int cont = 0;
	for (int i=0; i<linha; i++){
		for (int j=0; j<coluna; j++){
			if (j!=coluna-1){
				cout<<cont<<" "<<cont+1<<" ";
				float obj1 = ((float)(random())/(float)(RAND_MAX));
				int sinal = 1;
				if (((random()%2) + 1) == 2) sinal = -1;
				float ra = ((float)(random())/(float)(RAND_MAX));
				float obj2 = alpha*obj1 + betha + gama*ra*sinal;
				cout<<(int)(obj1*100)<<" "<<(int) (obj2*100)<<endl;
			}
			if (cont+coluna<linha*coluna){
				cout<<cont<<" "<<cont+coluna<<" ";
				float obj1 = ((float)(random())/(float)(RAND_MAX));
				int sinal = 1;
				if (((random()%2) + 1) == 2) sinal = -1;
				float ra = ((float)(random())/(float)(RAND_MAX));
				float obj2 = alpha*obj1 + betha + gama*ra*sinal;
				cout<<(int) (obj1*100)<<" "<<(int) (obj2*100)<<endl;
			}
			cont++;
		}
	}
	return 0;
}