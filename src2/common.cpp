#include "common.hpp"

void saisir_u_Q(vector<double>& u_Q, const C_DKPData& data) {

	double u; 
	for(int i = 0; i < data._nbCliques; ++i) {
		cout << "u_" << i << " : "; 
		cin >> u; 
		u_Q.push_back(u); 
		cout << endl;
	}

}


void writeInFile(int bestPrimal, double bestDual, double time, char critereSTOP, string filename, double alpha, int M) {
	
	// extraction du nom de l'instance (on enlève le path)
	size_t pos = filename.find_last_of("/");
	filename = filename.substr(pos+1); 

	ofstream myfile; 
	myfile.open("results.txt", ios::app); 

	myfile << "Instance: " << filename << endl;
	myfile << "Algo: dyna" << endl;
	myfile << "Alpha: " << alpha << endl;
	myfile << "M: " << M << endl;
	myfile << "BestPrimal: " << bestPrimal << endl;
	myfile << "BestDual: " << bestDual << endl;
	myfile << "Time: " << time/1000 << endl;
	myfile << "stopCrit: " << critereSTOP << endl;
	myfile << "_____________________________________________________" << endl << endl;
	
}


vector<int> getSolution(const vector<vector<pair<double,int>>>& tab, int nbObj, int taille) {
	// on sait que la solution se trouve à : tab[taille][nbObj] (taille = capacité max du sac)
	// on va reconstruire la solution et la stocker la dedans
	vector<int> solution(nbObj, 0); // initialisé à la taille du du nombre d'objets et avec que des 0
	// on va incrémenter solution[i] dès que l'objet est ajouté une fois. 

	// on pourra retrouver l'objet qui a été ajouté en comparant la valeur de tab[c][i] avec tab[pred(c)][i-1]
	// en effet, si tab[c][i] - tab[pred[c]][i-1] = profit de i , alors c'est qu'on a ajouté l'objet i 

	int c_courant = taille; 
	int i_courant = nbObj; 
	int val; 
	int pred; 

	do {
		if(tab[c_courant][i_courant].first == 0) { 
			// si la valeur située à tab[taille][nbObj] est nulle, c'est qu'on n'a jamais pris d'objets dans le sac avant l'état (i_courant, c_courant)
			// donc break
			break;
		
		} else {
			val = tab[c_courant][i_courant].first; 
			pred = tab[c_courant][i_courant].second; 

			//cout << "P(" << i_courant << "," << c_courant << ")" << " : " << val << endl;
			//cout << "On vient de : (" << i_courant -1 << "," << pred << ")" << endl << endl; 
		
			if(val - tab[pred][i_courant -1].first > 0) {
				// si c'est != 0, c'est qu'on a ajouté l'objet i_courant à cette étape. Donc ++
				++solution[i_courant-1];
			}
			--i_courant; 
			c_courant = pred; 
		}
	} while(i_courant > 0); // tant qu'on a pas atteint l'état initial 

	return solution; 
}


double produitScalaire(const vector<double>& X1, const vector<int>& X2) {
	
	if (X1.size() != X2.size()) {
		cerr << "Produit scalaire impossible entre vecteurs de tailles différentes " << endl; 
		cout << "c_Q : " << endl;
		for(double el : X1) {
			cout << el << " "; 
		}
		cout << endl << "K[i] : " << endl; 
		for(int el : X2) {
			cout << el << " "; 
		}
		cout << endl;
	}
	
	double result = 0; 

	for(uint i = 0; i < X1.size(); ++i) {
		result += X1[i]*X2[i]; 
	}

	return result; 
}


void afficheResults(const vector<vector<pair<double,int>>>& tab) {
	 
	cout << "## Affichage du tableau ##" << endl << endl;
	for(uint i = 0; i < tab.size(); ++i) {
		for(uint j = 0; j < tab[i].size(); ++j) {
			cout << tab[i][j].first << " "; 
		} cout << endl;
	}
}


vector<vector<int>> inClique(const C_DKPData& data) {
	vector<vector<int>> K(data._nbItems, vector<int>(data._nbCliques, 0));
	// parcours de _cliques : 
	for(int i = 0; i < data._nbCliques; ++i) {
		for(uint j = 0; j < data._cliques[i].size(); ++j) {
			// à cliques[i][j] se situe un objet appartenant à la clique i. 
			// on incrémente donc le vecteur associé à cet objet dans K, pour la clique i. 
			if(K[data._cliques[i][j]][i] == 0) {
				++K[data._cliques[i][j]][i]; 
			} else {
				cerr << "Erreur d'appartenance pour la clique " << i << 
				" avec l'objet " << data._cliques[i][j] << endl; // détecte une erreur dans le cas où un objet appartient plusieurs fois à une même clique (ce serait bizarre)
			}
		}
	}
	return K; 
}


void afficheK(const vector<vector<int>>& K) {
	
	cout << "## Affichage de K ##" << endl << endl;

	for(uint i = 0; i < K.size(); ++i) {
		for(uint j = 0; j < K[i].size(); ++j) {
			cout << K[i][j] << " "; 
		} 
		cout << endl;
	}
	cout << endl;
}


bool isRealisable(const C_DKPData& data, const vector<int>& sol, int& solValue, const vector<vector<int>>& K) {
	// vérifier la contrainte de capacité et en même temps calculer solValue la valeur de la solution dans le PLNE
	solValue = 0; 
	int capacityUsed = 0; 

	for(int i = 0; i < data._nbItems; ++i) { // calcul valeur et capacité utilisée
		solValue += sol[i]*data._profits[i]; 
		capacityUsed += sol[i]*data._weights[i]; 
	}

	if(capacityUsed <= data._capacity) { // si ça respecte la capacité du sac, on va vérifier des contraintes de conflit

		vector<int> cliquesImplicated(data._nbCliques, 0); // cliquesImplicated[i] représente le nb d'obj de la clique présent dans la sol. 
		// suffit de détecter un indice i tel que cliquesImplicated[i]>1 et on sait que la contrainte est violée
		for(int i = 0; i < data._nbItems; ++i) {
			if(sol[i]) { // si i utilisé
				for(uint j = 0; j < K[i].size(); ++j) {
					if(K[i][j]) { // si i est ds la clique j 
						if(cliquesImplicated[j] == 0) {
							++cliquesImplicated[j];
						} else {
							return false; // car deux objets i selectionnés partagent une mm clique
						}
					}
				}
			}
		}

	} else {
		return false; 
	}
	
	return true; 
}