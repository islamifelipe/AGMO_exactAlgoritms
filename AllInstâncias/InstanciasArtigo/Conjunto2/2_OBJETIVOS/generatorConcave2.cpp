/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Knowles's (2002) graph generator, 
# to generate concave 2-objetives complete graphs  
#=======================================================================
*/

#include <iostream>
#include <stdlib.h>     /* atoi */
#include <time.h>       /* time */

using namespace std;

#define multiplicador 100//100000

int main (int argc, char *argv[]){
	srand (time(NULL));

	int n=0; // quantidade de vertices
	double eta, zeta, obj1, obj2;
	n =  atoi(argv[1]); 
	eta = atof(argv[2]); 
	zeta =  atof(argv[3]);
	int a_eta = eta*multiplicador, a_zeta = zeta*multiplicador;
	cout<<n<<endl; // quantidade de vertices
	for (int i=0; i<n; i++){
		for (int j=i+1; j<n; j++){
			cout<<i<<" "<<j<<" ";

			if (i==0 && j == 1){
				obj1 = zeta;
				obj2 = zeta;
			} else if (i==0 && j == 2 ){
				obj1 = 0;
				obj2 = 1-zeta;
			} else if (i==1 && j==2){
				obj1 = 1-zeta;
				obj2 = 0;
			} else if ((i==0 && j==3) 
				|| (i==1 && j==3) 
				|| (i==2 && j==3 )
				|| (i==4 && j==5)
				|| (i==4 && j==6)
				|| (i==4 && j==7)
				|| (i==5 && j==6)
				|| (i==5 && j==7 )
				|| (i==6 && j==7 )
				|| (i==8 && j==9 )
				|| (i==8 && j==10 )
				|| (i==8 && j==11 )
				|| (i==9 && j==10 )
				|| (i==9 && j==11 )
				|| (i==10 && j==11 )
				|| (i==12 && j==13 )
				|| (i==12 && j==14 )
				|| (i==12 && j==15 )
				|| (i==13 && j==14)
				|| (i==13 && j==15 )
				|| (i==14 && j==15 )
				|| (i==16 && j==17 )
				|| (i==16 && j==18 )
				|| (i==16 && j==19 )
				|| (i==17 && j==18 )
				|| (i==17 && j==19 )
				|| (i==18 && j==19 )
				|| (i==20 && j==21 )
				|| (i==20 && j==22 )
				|| (i==20 && j==23 )
				|| (i==21 && j==22 )
				|| (i==21 && j==23 )
				|| (i==22 && j==23 )
				|| (i==24 && j==25)
				|| (i==24 && j==26 )
				|| (i==24 && j==27)
				|| (i==25 && j==26)
				|| (i==25 && j==27)
				|| (i==26 && j==27)
				|| (i==28 && j==29)
				|| (i==28 && j==30)
				|| (i==28 && j==31)
				|| (i==29 && j==30)
				|| (i==29 && j==31)
				|| (i==30 && j==31)
				|| (i==32 && j==33 )
				|| (i==32 && j==34)
				|| (i==32 && j==35)
				|| (i==33 && j==34)
				|| (i==33 && j==35)
				|| (i==34 && j==35)
				|| (i==36 && j==37)
				|| (i==36 && j==38)
				|| (i==36 && j==39)
				|| (i==37 && j==38)
				|| (i==37 && j==39)
				|| (i==38 && j==39)
				|| (i==40 && j==41)
				|| (i==40 && j==42)
				|| (i==40 && j==43)
				|| (i==41 && j==42)
				|| (i==41 && j==43)
				|| (i==42 && j==43)
				|| (i==44 && j==45)
				|| (i==44 && j==46)
				|| (i==44 && j==47)
				|| (i==45 && j==46)
				|| (i==45 && j==47)
				|| (i==46 && j==47)
				|| (i==48 && j==49)){
					int divisor= multiplicador - (int)((1.0 - zeta)*multiplicador) + 1;
				
					int a_obj1 = ((rand()%(divisor) + (int)((1.0 - zeta)*multiplicador))); // entre 1-zeta e 1
					int a_obj2 = ((rand()%(divisor) + (int)((1.0 - zeta)*multiplicador))); // entre 1-zeta e 1
					// cout<<"aqui : "<<a_obj1<<" "<<a_obj2<<endl;
					obj1 = (double)a_obj1/multiplicador;
					obj2 = (double)a_obj2/multiplicador;
			}else {
				int a_obj1 = ((rand()%(a_eta - a_zeta + 1)) + a_zeta); // entre zeta e eta
				int a_obj2 = ((rand()%(a_eta - a_zeta + 1)) + a_zeta);
				obj1 = (double) a_obj1/multiplicador;
				obj2 = (double)a_obj2/multiplicador;
			}
			cout<<(int)(obj1*100)<<" "<<(int)(obj2*100)<<endl;
		}
	}
}
