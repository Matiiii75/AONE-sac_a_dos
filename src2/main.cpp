#include "sacDyna.hpp"
#include "sousGradient.hpp"


int main(int argc, char** argv) {

    string filename; 
    double alpha; 
    int M;

    if(argc == 2) {   // récup nom de l'instance 
        filename = argv[1]; 
    } else {
        cerr << "Expected exactly 2 args" << endl;
        return -1; 
    }

    C_DKPData data(filename); 

    cout << "Saisir l'alpha (< 2 ; > 0) : "; 
    cin >> alpha; 
    cout << endl;

    while(alpha >= 2 || alpha <= 0) {
        cout << "alpha incorrect, doit être compris entre 2 et 0" << endl;
        cout << "Saisir l'alpha (< 2 ; > 0) :"; 
        cin >> alpha;  
        cout << endl;
    }

    cout << "Saisir M :"; 
    cin >> M; 
    cout << endl;

    sousGradients(data, alpha, M, filename); 

    return 0; 
}


