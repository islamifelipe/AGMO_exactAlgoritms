/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the improved Knowles's (2002) graph generator, 
# to generate the correlated and anticorrelated m-objetives complete graphs  
#=======================================================================
*/

// Este gerador recebe como entrada o << n alfa m >> onde m é a quantidade arbitrária de objetivos

#include <iostream>
#include <stdlib.h>     /* atoi */
#include <time.h>       /* time */

using namespace std;

// n alfa m
int main (int argc, char *argv[]){

	int n=0; // quantidade de vertices
	double alpha;
	n = atoi(argv[1]); // quantidade de vertices
	alpha =  atof(argv[2]);
	cout<<n<<endl; // quantidade de vertices
	int m = atof(argv[3]);
	srandom (time(NULL));

	double betha, gama;
	betha = 0.5*((double)(1.0-alpha)); // Correçao do Chen et al (2007)
	if (alpha>=0){
		gama = betha;
	} else{
		gama = 1.0 - betha;
	}
	//cout<<"betha = "<<betha<<" gama = "<<gama<<endl;
	//int cont = 0;
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			cout<<i<<" "<<j<<" ";
			float obj1 = ((float)(random())/(float)(RAND_MAX));
			cout<<(int)(obj1*100)<<" ";
			for (int k_obj = 1; k_obj < m; k_obj++){
				int sinal = 1;
				if (((random()%2) + 1) == 2) sinal = -1;
				float ra = ((float)(random())/(float)(RAND_MAX));
				float obj = alpha*obj1 + betha + gama*ra*sinal;
				cout<<(int)(obj*100)<<" ";
			}
			cout<<endl;
		}
	}
	return 0;
}