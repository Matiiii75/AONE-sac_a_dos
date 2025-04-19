#include "sacDyna.hpp"


int main(int argc, char** argv) {

    string filename; 
    double alpha; 
    int M;
    cout << argc << endl;
    if(argc == 4) {   // récup nom de l'instance 
        cout << "ici" << endl;
        filename = argv[1]; 
        alpha = atof(argv[2]); 
        M = atoi(argv[3]); 
    } else {
        cout << "Enter instance file name :";
        cin >> filename;  
    }

    cout << "ici"; 

    C_DKPData data(filename); 

    cout << data; 

    // cout << "Saisir l'alpha (< 2 ; > 0) : "; 
    // cin >> alpha; 
    // cout << endl;

    // while(alpha >= 2 || alpha <= 0) {
    //     cout << "alpha incorrect, doit être compris entre 2 et 0" << endl;
    //     cout << "Saisir l'alpha (< 2 ; > 0) :"; 
    //     cin >> alpha;  
    //     cout << endl;
    // }

    // cout << "Saisir M :"; 
    // cin >> M; 
    // cout << endl;

    sousGradients(data, alpha, M); 

    return 0; 
}


