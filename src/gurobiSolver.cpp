#include<string>
#include<sstream>
#include<vector>
#include<iostream>
#include "gurobi_c++.h"
#include "DKP_Data.hpp"

using namespace std;

void solver(const string& filename) {
    

    C_DKPData data(filename); 

    // Création Env

    GRBEnv env = GRBEnv(true); 
    env.start(); 

    // création modèle (vide)

    GRBModel model = GRBModel(env);

    // Déclaration variables 

    // variable x : 
    vector<GRBVar> x(data._nbItems); 
    for(int i = 0; i < data._nbItems; ++i) {
        stringstream ss; 
        ss << "x " << i; 
        x[i] = model.addVar(0.0, 1.0, 1.0, GRB_CONTINUOUS, ss.str()); 
    }

    // Fonction objectif 

    GRBLinExpr obj = 0; 
    for(int i = 0; i < data._nbItems; ++i) {
        obj += data._profits[i]*x[i]; 
    }
    model.setObjective(obj, GRB_MAXIMIZE); 

    // Création des contraintes 

    // contrainte 1 (respect capacité)
    GRBLinExpr c1 = 0; 
    for(int i = 0; i < data._nbItems; ++i) {
        c1 += x[i]*data._weights[i]; 
    }
    model.addConstr(c1 <= data._capacity); 

    // contrainte 2 (respect des cliques)
    for(int i = 0; i < data._nbCliques; ++i) { // pour toute clique Q
        GRBLinExpr c2 = 0; 
        for(size_t j = 0; j < data._cliques[i].size(); ++j) { // pour tout élément dans cette clique. 
            c2 += x[data._cliques[i][j]]; 
        }
        model.addConstr(c2 <= 1); 
    }

    // configuration du solveur (tmps limite, nb threads ...)
    model.set(GRB_DoubleParam_TimeLimit, 600.0); 
    model.set(GRB_IntParam_Threads, 1); 

    // Résolution 

    model.optimize(); 
     
    // récupération et affichage des résultats 

    int status = model.get(GRB_IntAttr_Status); 
    if(status == GRB_OPTIMAL || (status == GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount)>0))
    {
        cout << "Success ! Status : " << status << endl; 
        
        // valeur de la solution 
        cout << "Objective Value : " << model.get(GRB_DoubleAttr_ObjVal) << endl;

        // Affichage solution trouvée 
        cout << "["; 
        for(int i = 0; i < data._nbItems; ++i) {
            cout << x[i].get(GRB_DoubleAttr_X) << ", "; 
        }
        cout << "]" << endl << endl;

    } 
    else {
        // modèle irréalisable ou faux 
        // ou bien aucune solution n'a pu être calculée dans le temps limite imparti
        cerr << "Fail : (Status : )" << status << ")" << endl;
    }
}


int main(int argc, char* argv[]) {
    string filename = argv[1]; 
    solver(filename);
    return 0;  
}