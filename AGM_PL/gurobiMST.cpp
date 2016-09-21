
#include "gurobi_c++.h"
using namespace std;

int
main(int   argc, char *argv[]) {
  try {
    int n;
    cin >> n; // numero de vértices do grafo
    GRBEnv env = GRBEnv();

    GRBModel model = GRBModel(env);

    GRBVar **y = new GRBVar*[n]; 
    GRBVar **x = new GRBVar*[n];
    double coeficienteObjetv[n][n];

    for (int i=0; i<n;i++){
        y[i] = new GRBVar[n];
        x[i] = new GRBVar[n];
    }

    int origem, destino;
    double peso1, peso2;

    while (cin>>origem){
      cin>>destino;
      cin>>peso1;
      cin>>peso2;
      coeficienteObjetv[origem][destino] = peso1*(-1); // o problema é de maximizacao
    }
    // coeficienteObjetv[0][1] = -1;
    // coeficienteObjetv[0][2] = -20;
    // coeficienteObjetv[0][3] = -10;
    // coeficienteObjetv[1][2] = -8;
    // coeficienteObjetv[1][3] = -45;
    // coeficienteObjetv[2][3] = -4;

    int constrCont=0;
    // Create variables

    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
        y[i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y"+std::to_string(i)+std::to_string(j));
      }
    }

    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
          x[i][j] = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(i)+std::to_string(j));
          x[j][i] = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x"+std::to_string(j)+std::to_string(i));
      }
    }    
    // GRBVar y12 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y12");
    // GRBVar y13 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y13");
    // GRBVar y14 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y14");
    // GRBVar y23 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y23");
    // GRBVar y24 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y24");
    // GRBVar y34 = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, "y34");

    // GRBVar x12 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x12");
    // GRBVar x21 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x21");
        
    // GRBVar x13 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x13");
    // GRBVar x31 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x31");

    // GRBVar x14 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x14");
    // GRBVar x41 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x41");
    
    // GRBVar x23 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x23");
    // GRBVar x32 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x32");
    
    // GRBVar x24 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x24");
    // GRBVar x42 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x42");
    
    // GRBVar x34 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x34");
    // GRBVar x43 = model.addVar(0.0, 1000, 0.0, GRB_CONTINUOUS, "x43");
    
    // Integrate new variables

    model.update();

    // Set objective: 
    GRBLinExpr exprObjet;
    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
        exprObjet.addTerms(&coeficienteObjetv[i][j], &y[i][j],1);
        //cout<<coeficienteObjetv[i][j]<<"y"<<i<<j<<" ";
      }
    }
    cout<<endl;
    //model.setObjective(-1*y12 - 20*y13 - 8*y14 - 10*y23 - 45*y24 - 4*y34, GRB_MAXIMIZE);
    model.setObjective(exprObjet,GRB_MAXIMIZE); 
    // Add constraint

    GRBLinExpr constr5 ;
    double co = 1;
    double com = -1;
    for (int j=0+1; j<n; j++){
        constr5.addTerms(&co,&x[0][j],1);
        //constr5+=x[0][j];
      }
    
    //model.addConstr(x12 + x13 + x14 == n-1, "c0");
    model.addConstr(constr5, GRB_EQUAL, n-1,std::to_string(constrCont++));
   // model.addConstr(constr5 == n-1,std::to_string(constrCont++));
    
    // // Add constraint:
    for (int j=1; j<n; j++){
      GRBLinExpr constr2 = 0;
      for (int i=0; i<n; i++){
        if (i!=j){
          constr2.addTerms(&co,&x[i][j],1);
          constr2.addTerms(&com,&x[j][i],1);
        }
      }
      model.addConstr(constr2, GRB_EQUAL, 1,std::to_string(constrCont++));
    }
    // model.addConstr(x12 + x32 + x42 - x24 - x23 - x21 == 1, "c1"); // j = 2
    // model.addConstr(x13 + x23 + x43 - x34 - x31 - x32 == 1, "c2"); // j = 3
    // model.addConstr(x14 + x24 + x34 - x41 - x42 - x43 == 1, "c3"); // j = 4


    //  // Add constraint:

    double coef = (double) n - 1;
    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
        GRBLinExpr constr8;
        GRBLinExpr constr9;
        constr8.addTerms(&coef,&y[i][j],1);
        constr9.addTerms(&co  ,&x[i][j],1);
        constr9.addTerms(&co  ,&x[j][i],1);
      model.addConstr(constr8, GRB_GREATER_EQUAL, constr9,std::to_string(constrCont++));
    
      }
    }

    // model.addConstr((n-1)*y12 >= x12 + x21, "c4"); 
    // model.addConstr((n-1)*y13 >= x13 + x31, "c5"); 
    // model.addConstr((n-1)*y14 >= x14 + x41, "c6"); 
    // model.addConstr((n-1)*y23 >= x23 + x32, "c7"); 
    // model.addConstr((n-1)*y24 >= x24 + x42, "c8"); 
    // model.addConstr((n-1)*y34 >= x34 + x43, "c9"); 

    //  // Add constraint:

 
    for (int i=0; i<n; i++){

      for (int j=i+1; j<n; j++){

        GRBLinExpr constr22;
        GRBLinExpr constr33;
        constr22.addTerms(&co  ,&y[i][j],1);
        constr33.addTerms(&co  ,&x[i][j],1);
        constr33.addTerms(&co  ,&x[j][i],1);
       // cout<<constr22<<GRB_LESS_EQUAL<<constr33<<endl;     
        model.addConstr(constr22, GRB_LESS_EQUAL, constr33,std::to_string(constrCont++));
    
      }
    }

    // model.addConstr(y12 <= x12 + x21, "c10"); 
    // model.addConstr(y13 <= x13 + x31, "c11"); 
    // model.addConstr(y14 <= x14 + x41, "c12"); 
    // model.addConstr(y23 <= x23 + x32, "c13"); 
    // model.addConstr(y24 <= x24 + x42, "c14"); 
    // model.addConstr(y34 <= x34 + x43, "c15"); 

    // // Optimize model

     model.optimize();

    for (int i=0; i<n; i++){
      for (int j=i+1; j<n; j++){
          cout << y[i][j].get(GRB_StringAttr_VarName) << " "<< y[i][j].get(GRB_DoubleAttr_X) << endl;
      }
    }

    for (int i=0; i<n; i++){
      for (int j=0; j<n; j++){
          if (i!=j){
            cout << x[i][j].get(GRB_StringAttr_VarName) << " "<< x[i][j].get(GRB_DoubleAttr_X) << endl;
          }
      }
    }
    // cout << y12.get(GRB_StringAttr_VarName) << " "
    //      << y12.get(GRB_DoubleAttr_X) << endl;
    
    // cout << y13.get(GRB_StringAttr_VarName) << " "
    //      << y13.get(GRB_DoubleAttr_X) << endl;
    
    // cout << y14.get(GRB_StringAttr_VarName) << " "
    //      << y14.get(GRB_DoubleAttr_X) << endl;

    // cout << y23.get(GRB_StringAttr_VarName) << " "
    //      << y23.get(GRB_DoubleAttr_X) << endl;

    // cout << y24.get(GRB_StringAttr_VarName) << " "
    //      << y24.get(GRB_DoubleAttr_X) << endl;

    // cout << y34.get(GRB_StringAttr_VarName) << " "
    //      << y34.get(GRB_DoubleAttr_X) << endl;


    // cout << x12.get(GRB_StringAttr_VarName) << " "
    //      << x12.get(GRB_DoubleAttr_X) << endl;
    
    // cout << x13.get(GRB_StringAttr_VarName) << " "
    //      << x13.get(GRB_DoubleAttr_X) << endl;
    
    // cout << x14.get(GRB_StringAttr_VarName) << " "
    //      << x14.get(GRB_DoubleAttr_X) << endl;

    // cout << x23.get(GRB_StringAttr_VarName) << " "
    //      << x23.get(GRB_DoubleAttr_X) << endl;

    // cout << x24.get(GRB_StringAttr_VarName) << " "
    //      << x24.get(GRB_DoubleAttr_X) << endl;

    // cout << x34.get(GRB_StringAttr_VarName) << " "
    //      << x34.get(GRB_DoubleAttr_X) << endl;

    cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}
