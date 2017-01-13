/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Lokman and Koksalan's (2013) algorithm 
# based on the Multiobjective Integer Program for p = 2
# This code implements uses Gurobi for resolve the Pmn model
#=======================================================================

m = 1 
*/


#include <iostream>
#include <map> 
#include <string>
#include <climits>
#include <stack>   
#include <utility>      // std::pair
#include <cmath>
#include <list>
#include "gurobi_c++.h"
#include <sys/times.h>
#include <unistd.h>
using namespace std;

int n;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinal; // para medir o tempo
//double coeficienteObjetv[n][n],matrix_peso1[n][n],matrix_peso2[n][n];;
double **coeficienteObjetv,**matrix_peso1,**matrix_peso2;
short **arestas;
std::vector<short**> S;

void printResultado(){
	for (int pp=0; pp<S.size(); pp++){
		double cont11=0, cont22 = 0;
		short **result = S[pp];
		cout<<"Arvore "<<pp+1<<endl;
		for (int i=0; i<n; i++){
			for (int j=i+1; j<n; j++){
				if (arestas[i][j] == 1){
					if (result[i][j] == 1){
						cout <<i<<" "<<j<<" "<<matrix_peso1[i][j]*(-1)<<" "<<matrix_peso2[i][j]*(-1)<<endl;
						cont11+=matrix_peso1[i][j]*(-1);
						cont22+=matrix_peso2[i][j]*(-1);
					}
				}
			}
		}
		cout<<"("<<cont11<<", "<<cont22<<")"<<endl;
		cout<<endl;
	}	
}

void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec>20){ // se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			cout<<sec<<endl;
			cout<<"TEMPO LIMITE ATINGIDO...   " <<endl;

			printResultado();
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			exit(-1);
		}
	}
}

int main(){

	
	
	
	float peso1, peso2;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	arestas = new short*[n];
	coeficienteObjetv = new double*[n];
	matrix_peso1 = new double*[n];
	matrix_peso2 = new double*[n];
	for (int i=0; i<n; i++){
		arestas[i] = new short[n];
		coeficienteObjetv[i] = new double[n];
		matrix_peso1[i] = new double[n];
		matrix_peso2[i] = new double[n];
	}


	GRBEnv env = GRBEnv();;
	env.set("OutputFlag","0");
	GRBModel model = GRBModel(env);;

	GRBVar **y, **x;

	float epslon = 0.001;
	//cin>>epslon;



  	y = new GRBVar*[n]; 
   	x = new GRBVar*[n];


   	for (int i=0; i<n;i++){
        y[i] = new GRBVar[n];
        x[i] = new GRBVar[n];
   	}

	int constrCont=0;
    // Create variables

	for (int i=0; i<n; i++){
       for (int j=0; j<n; j++){
       	arestas[i][j] = 0;
       }
   }

	while (cin>>origem){
		cin>>destino;
		cin>>peso1;
		cin>>peso2;
		coeficienteObjetv[origem][destino] = (peso1 + epslon*peso2)*(-1); // o problema é de maximizacao
		x[origem][destino] = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(origem)+std::to_string(destino));
        x[destino][origem] = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(destino)+std::to_string(origem));
      	y[origem][destino] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y"+std::to_string(origem)+std::to_string(destino));
      	arestas[origem][destino] = 1;
      	arestas[destino][origem] = 1;
      	matrix_peso1[origem][destino] = peso1*(-1);
      	matrix_peso2[origem][destino] = peso2*(-1);
		id++;
	}
	int nA = id; // quantidade de arestas do grafo	

	int m = 1;// por default, o m falado por Lokman and Koksalan sera igual a 1


    model.update();

    // Set objective: 
    GRBLinExpr exprObjet;
    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
      	if (arestas[i][j] == 1)
       		exprObjet.addTerms(&coeficienteObjetv[i][j], &y[i][j],1);
      }
    }
    cout<<endl;
    model.setObjective(exprObjet,GRB_MAXIMIZE); 



    GRBLinExpr constr5 ;
    double co = 1;
    double com = -1;
    for (int j=0+1; j<n; j++){
    	if (arestas[0][j] == 1)
        	constr5.addTerms(&co,&x[0][j],1);
    }
    model.addConstr(constr5, GRB_EQUAL, n-1,std::to_string(constrCont++));
  


    // // Add constraint:
    for (int j=1; j<n; j++){
      GRBLinExpr constr2 = 0;
      for (int i=0; i<n; i++){
        if (arestas[i][j] == 1){
          constr2.addTerms(&co,&x[i][j],1);
          constr2.addTerms(&com,&x[j][i],1);
        }
      }
      model.addConstr(constr2, GRB_EQUAL, 1,std::to_string(constrCont++));
    }

    double coef = (double) n - 1;
    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
      	if (arestas[i][j] == 1){
	        GRBLinExpr constr8;
	        GRBLinExpr constr9;
	        constr8.addTerms(&coef,&y[i][j],1);
	        constr9.addTerms(&co  ,&x[i][j],1);
	        constr9.addTerms(&co  ,&x[j][i],1);
	      	model.addConstr(constr8, GRB_GREATER_EQUAL, constr9,std::to_string(constrCont++));
    	}
      }
    }

    for (int i=0; i<n; i++){

      for (int j=i+1; j<n; j++){
      	if (arestas[i][j] == 1){
	        GRBLinExpr constr22;
	        GRBLinExpr constr33;
	        constr22.addTerms(&co  ,&y[i][j],1);
	        constr33.addTerms(&co  ,&x[i][j],1);
	        constr33.addTerms(&co  ,&x[j][i],1);
	       // cout<<constr22<<GRB_LESS_EQUAL<<constr33<<endl;     
	        model.addConstr(constr22, GRB_LESS_EQUAL, constr33,std::to_string(constrCont++));
    	}
      }
    }

    int nn = 0; // n do algoritmo de Lokman and Koksalan 	
    /* 
	* Algoritmo 1 de Lokman and Koksalan 	
	*/
	try {
		times(&tempsInit);


		// para medir o tempo em caso limite
		pthread_t thread_time; 
		pthread_attr_t attr;
		int nnnnnnnn=0;
		if(pthread_create(&thread_time, NULL, &tempo, (void*)nnnnnnnn)){ // on criee efectivement la thread de rechaufage
	        printf("Error to create the thread");
	        exit(-1);
	    }
	    //


		int optimstatus;
		//std::vector<short**> S;
		 do{
			//cout<<"Inicio"<<endl;
			short **result = new short*[n];
			for (int ii=0; ii<n; ii++){
				result[ii] = new short[n];
			}
			model.optimize();
			optimstatus = model.get(GRB_IntAttr_Status);
			if (optimstatus != GRB_INFEASIBLE){
				double sum = 1; // z_tj + 1
				 for (int i=0; i<n; i++){
				    for (int j=i+1; j<n; j++){
				     	if (arestas[i][j] == 1){
				         	result[i][j] = y[i][j].get(GRB_DoubleAttr_X);
				     		sum+=(matrix_peso2[i][j])*result[i][j];
				     	}
				    }
				 }

				 GRBLinExpr constr77;
				 for (int i=0; i<n; i++){
				 	for (int j=i+1; j<n; j++){
				 		if (arestas[i][j] == 1){
				 			constr77.addTerms(&matrix_peso2[i][j], &y[i][j],1);
				 		}
				 	}
				 }
				// cout<<constr77<<"<="<<sum<<endl;
				 model.addConstr(constr77, GRB_GREATER_EQUAL, sum,std::to_string(constrCont++));
		    	
				S.push_back(result);
				nn++;

			 }
			 //cout<<"fim"<<endl;
	  	 	} while (optimstatus != GRB_INFEASIBLE)	;

	  	 	times(&tempsFinal1);   /* current time */ // clock final
			clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
			cout<<user_time1<<endl;
			cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	



		   	printResultado();

  	 	} catch(GRBException e) {
	    cout << "Error code = " << e.getErrorCode() << endl;
	    cout << e.getMessage() << endl;
	  } catch(...) {
	    cout << "Exception during optimization" << endl;
	  }
   	
	return 0;
}