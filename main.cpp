#include "sacDyna.hpp"

int main(int argc, char** argv) {

    string filename; 
    if(argc > 1) {
        filename = argv[1]; 
    } else {
        cout << "Enter instance file name :";
        cin >> filename;  
    }

    C_DKPData data(filename); 

    cout << data; 
    
    double alpha; 
    int M; 

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

    sousGradients(data, alpha, M); 

    return 0; 
}