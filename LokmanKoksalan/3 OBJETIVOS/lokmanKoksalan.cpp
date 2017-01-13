/*
#=======================================================================
# Islame Felipe DA COSTA FERNANDES --- Copyright 2016
#-----------------------------------------------------------------------
# This code implements the Lokman and Koksalan's (2013) algorithm 
# based on the Multiobjective Integer Program for p = 3
# This code uses Gurobi for resolve the Pmn model
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


typedef struct Pesos{
	int peso1;
	int peso2;
	int peso3;
}Pesos;

int n;
struct tms tempsInit, tempsFinal1,tempsFinal2, tempsFinal; // para medir o tempo
//double coeficienteObjetv[n][n],matrix_peso1[n][n],matrix_peso2[n][n];;
double **coeficienteObjetv;//para guardar os coeficientes da funcao objetivo
double **matrix_peso1,**matrix_peso2,**matrix_peso3; // para guardar os pesos das arestas
short **arestas;
std::vector<short**> S; // guarda as solucoes nao dominadas
std::vector<Pesos> Z; // guarda os pontos associados às solucoes nao dominadas


void printResultado(){
	for (int pp=0; pp<S.size(); pp++){
		double cont11=0, cont22 = 0;
		short **result = S[pp];
		cout<<"Arvore "<<pp+1<<endl;
		for (int i=0; i<n; i++){
			for (int j=i+1; j<n; j++){
				if (arestas[i][j] == 1){
					if (result[i][j] == 1){
						cout <<i<<" "<<j<<" "<<matrix_peso1[i][j]*(-1)<<" "<<matrix_peso2[i][j]*(-1)<<" "<<matrix_peso3[i][j]*(-1)<<endl;
					
					}
				}
			}
		}
		cout<<"("<<Z[pp].peso1*(-1)<<", "<<Z[pp].peso2*(-1)<<", "<<Z[pp].peso3*(-1)<<")"<<endl;
		cout<<endl;
	}	
}

void *tempo(void *nnnn){
	while (true){
		times(&tempsFinal);   /* current time */ // clock final
		clock_t user_time = (tempsFinal.tms_utime - tempsInit.tms_utime);
		float sec = (float) user_time / (float) sysconf(_SC_CLK_TCK);
		
		if (sec==3600){ 
			cout<<"RESULTADO AO FIM DE 1H"<<endl;
			printResultado();
			sleep(3510); // é importante pra nao ficar verificando todo o tempo
		} else if (sec==7200){
			cout<<"RESULTADO AO FIM DE 2H"<<endl;
			printResultado();
			sleep(3510); // é importante pra nao ficar verificando todo o tempo
		} else if (sec==10800){// se o tempo limite for atingido, esse if é ativado, o resultado (na ultima iteraçao, se for o caso) é escrito e o programa para 
			
			cout<<"RESULTADO AO FIM DE 3H"<<endl;
			cout<<"TEMPO LIMITE ATINGIDO..."<<endl;

			printResultado();
			//cout<<"saindo... valor de ppp="<<ppp<<endl;
			exit(-1);
		}

		// if (sec>20){ 
		// 	cout<<sec<<endl;
		// 	cout<<"TEMPO LIMITE ATINGIDO...   " <<endl;

		// 	printResultado();
		// 	//cout<<"saindo... valor de ppp="<<ppp<<endl;
		// 	exit(-1);
		// }
	}
}

int main(){

	
	
	
	float peso1, peso2, peso3;
	int origem, destino; // vértices para cada aresta;
	int id = 0; // id das arestas que leremos do arquivo para criar o grafo
	cin>>n; // quantidade de vértices do grafo;
	arestas = new short*[n];
	coeficienteObjetv = new double*[n];
	matrix_peso1 = new double*[n];
	matrix_peso2 = new double*[n];
	matrix_peso3 = new double*[n];
	for (int i=0; i<n; i++){
		arestas[i] = new short[n];
		coeficienteObjetv[i] = new double[n];
		matrix_peso1[i] = new double[n];
		matrix_peso2[i] = new double[n];
		matrix_peso3[i] = new double[n];
	}


	GRBEnv env = GRBEnv();;
	env.set("OutputFlag","0");
	GRBModel model = GRBModel(env);;

	GRBVar **y, **x;

	float epslon = 0.0001;
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
		cin>>peso3;
		coeficienteObjetv[origem][destino] = (peso1*epslon + peso2*epslon + peso3)*(-1); // o problema é de maximizacao
		x[origem][destino] = model.addVar(0.0, 100000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(origem)+std::to_string(destino));
        x[destino][origem] = model.addVar(0.0, 100000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(destino)+std::to_string(origem));
      	y[origem][destino] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y"+std::to_string(origem)+std::to_string(destino));
      	arestas[origem][destino] = 1;
      	arestas[destino][origem] = 1;
      	matrix_peso1[origem][destino] = peso1*(-1);
      	matrix_peso2[origem][destino] = peso2*(-1);
      	matrix_peso3[origem][destino] = peso3*(-1);
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
  
    model.setObjective(exprObjet,GRB_MAXIMIZE); 


    // constraint 3.9 (FERNANDES, 2016)
    GRBLinExpr constr5 ;
    double coefff = 1;
    for (int j=0+1; j<n; j++){
    	if (arestas[0][j] == 1)
        	constr5.addTerms(&coefff,&x[0][j],1);
    }
    model.addConstr(constr5, GRB_EQUAL, n-1,std::to_string(constrCont++));
  


    // // Add constraint 3.10 (FERNANDES, 2016)
     double com = -1;
    for (int j=1; j<n; j++){
      GRBLinExpr constr2 = 0;
      for (int i=0; i<n; i++){
        if (arestas[i][j] == 1){
          constr2.addTerms(&coefff,&x[i][j],1);
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
	        constr9.addTerms(&coefff  ,&x[i][j],1);
	        constr9.addTerms(&coefff  ,&x[j][i],1);
	      	model.addConstr(constr8, GRB_GREATER_EQUAL, constr9,std::to_string(constrCont++));
    	}
      }
    }

     //cout<<"Modelo carregado"<<endl;
    for (int i=0; i<n; i++){

      for (int j=i+1; j<n; j++){
      	if (arestas[i][j] == 1){
	        GRBLinExpr constr22;
	        GRBLinExpr constr33;
	        constr22.addTerms(&coefff  ,&y[i][j],1);
	        constr33.addTerms(&coefff  ,&x[i][j],1);
	        constr33.addTerms(&coefff  ,&x[j][i],1);
	       // cout<<constr22<<GRB_LESS_EQUAL<<constr33<<endl;     
	        model.addConstr(constr22, GRB_LESS_EQUAL, constr33,std::to_string(constrCont++));
    	}
      }
    }

    int nn = 0; // o 'n' do algoritmo de Lokman and Koksalan 	
    int kk_estrela = 0; // o 'k*' do algoritmo 2 de Lokman and Koksalan
    int MM = 100000000; // o 'M' do algoritmo 2 de Lokman and Koksalan 	
    int z3_k_estrela; // pra guardar o Z_p^(P^(b^(k*,n))) do algoritmo 2 de Lokman and Koksalan
    /* 
	* Algoritmo 2 de Lokman and Koksalan 	
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


	    bool auxbol = false; // vira true (e o será pra sempre) quando resolvemos um modelo diferente do SIMPLES
		int optimstatus;

		short **result = new short*[n];
		for (int ii=0; ii<n; ii++){
			result[ii] = new short[n];
		}
		
		model.optimize(); // P0,3 --> n=0 (modelo SIMPLES)
		optimstatus = model.get(GRB_IntAttr_Status);
		int z1=0,z2=0,z3=0;
		if (optimstatus != GRB_INFEASIBLE){
			for (int i=0; i<n; i++){
				for (int j=i+1; j<n; j++){
				    if (arestas[i][j] == 1){
				        result[i][j] = y[i][j].get(GRB_DoubleAttr_X); // GUARDA O RESULTADO
				    	z1+=result[i][j]*matrix_peso1[i][j]; // calcula os pesos
				    	z2+=result[i][j]*matrix_peso2[i][j];
				    	z3+=result[i][j]*matrix_peso3[i][j];
				    }
				}
			}
			S.push_back(result);
			Pesos ppp = (Pesos){z1,z2,z3};
			Z.push_back(ppp);
			nn++;
		}

		do{ // esse loop para quando z3_k_estrela==-MM
			kk_estrela = 0;
			z3_k_estrela =(-1)*MM; // guarda o maximo
			short **z_n_plus_1 = new short*[n];
			for (int ii=0; ii<n; ii++){
				z_n_plus_1[ii] = new short[n];
			}
			int z1_estrela, z2_estrela;
			for (int k=-1; k<nn; k++){ // no algoritmo original, k deve variar de 0 à n (existindo solucoes de 1 à n). 
				//aqui, portanto, fazemos k de -1 à n-1, porque as solucoes vao de 0 à n-1
				
				//Primeiramente, prepara o b1 e b2
				int b1,b2;
				if (k==-1) b1=(-1)*MM; // -M
				else b1 = Z[k].peso1 + 1;

				b2 = (-1)*MM; // Snk = vazio

				for (int ii=0; ii<S.size(); ii++){
					if (Z[ii].peso1>=b1) {
						if (Z[ii].peso2 > b2){
							b2 = Z[ii].peso2;
						}
					}
				}
				if (b2!=(-1)*MM) b2=b2+1; // max + 1
				//cout <<"b1= "<<b1<<"  b2= "<<" "<<b2<<"  k="<<k<<endl;
				if (auxbol == true){ // remove as restricoes de z2>b2 e adiciona novas
					
					GRBConstr cb1 = model.getConstrByName("cb1");
					GRBConstr cb2 = model.getConstrByName("cb2");
					model.remove(cb1);
					model.remove(cb2);
				}
				GRBLinExpr cb1;
				GRBLinExpr cb2;
				 for (int i=0; i<n; i++){
				 	for (int j=i+1; j<n; j++){
				 		if (arestas[i][j] == 1){
				 			cb1.addTerms(&matrix_peso1[i][j], &y[i][j],1);
				 			cb2.addTerms(&matrix_peso2[i][j], &y[i][j],1);
				 		}
				 	}
				 }
				model.addConstr(cb1, GRB_GREATER_EQUAL, b1,"cb1");
			  	model.addConstr(cb2, GRB_GREATER_EQUAL, b2,"cb2");

				//Agora resolvemos o modelo P^(b^(k,n))
			  	auxbol=true;
			  	model.optimize(); 
				optimstatus = model.get(GRB_IntAttr_Status);
		
				//precisa-sa guardar o maximo de z3 entre todos o modelos P^(b^(k,n)) resulvidos para todo k 
				if (optimstatus != GRB_INFEASIBLE){
					short **result = new short*[n];
					for (int ii=0; ii<n; ii++){
						result[ii] = new short[n];
					}
					z1=0,z2=0,z3=0;
					for (int i=0; i<n; i++){
						for (int j=i+1; j<n; j++){
				  			if (arestas[i][j] == 1){
				        		result[i][j] = y[i][j].get(GRB_DoubleAttr_X); // GUARDA O RESULTADO
						    	z1+=result[i][j]*matrix_peso1[i][j]; // calcula os pesos
						    	z2+=result[i][j]*matrix_peso2[i][j];
						    	z3+=result[i][j]*matrix_peso3[i][j];
				    		}
						}
					}
					if (z3>z3_k_estrela){
						z1_estrela = z1;
						z2_estrela = z2;
						z3_k_estrela = z3;
						kk_estrela = k;
						for (int i=0; i<n; i++){
							for (int j=i+1; j<n; j++){
								z_n_plus_1[i][j] = result[i][j];
							}
						}
					}
				}
			}
			if (z3_k_estrela!=(-1)*MM){
				S.push_back(z_n_plus_1);
				Pesos pppp = (Pesos){z1_estrela,z2_estrela,z3_k_estrela};
				Z.push_back(pppp);
				nn++;
			}
		} while (z3_k_estrela!=(-1)*MM);


	  	 	times(&tempsFinal1);   /* current time */ // clock final
			clock_t user_time1 = (tempsFinal1.tms_utime - tempsInit.tms_utime);
			cout<<user_time1<<endl;
			cout<<(float) user_time1 / (float) sysconf(_SC_CLK_TCK)<<endl;//"Tempo do usuario por segundo : "
   	


			cout<<"RESULTADO FINAL..."<<endl;
		   	printResultado();

  	 	} catch(GRBException e) {
	    cout << "Error code = " << e.getErrorCode() << endl;
	    cout << e.getMessage() << endl;
	  } catch(...) {
	    cout << "Exception during optimization" << endl;
	  }
   	
	return 0;
}