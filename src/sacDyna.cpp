#include "sacDyna.hpp"

// fonction qui permet à l'utilisateur de saisir les u_Q manuellement

void saisir_u_Q(vector<double>& u_Q, const C_DKPData& data) {

	double u; 
	for(int i = 0; i < data._nbCliques; ++i) {
		cout << "u_" << i << " : "; 
		cin >> u; 
		u_Q.push_back(u); 
		cout << endl;
	}

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
		/*}
		if(c_courant == 0) {
			// si c_courant est nul, c'est qu'on a pas pris les objets précédents cet état donc inutile de continuer
			break;*/
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

// fonction principale qui résoud le probleme du sac à dos borné

double sacDynamique(const C_DKPData& data,const vector<double>& u_Q, vector<int>& sol,const vector<vector<int>>& K) {
	

	// taille : nb lignes / nbObj = nb colonnes
	vector<vector<pair<double,int>>> tab(data._capacity+1, vector<pair<double,int>>(data._nbItems+1)); 

	// intialisation de la première colonne a 0 (états initiaux) :  
	for(int i = 0; i < data._capacity+1; ++i) {
		
		tab[i][0].first = 0; 
		tab[i][0].second = 0; 
	}
	cout << "profit lagrangien: "; 
	for(int i = 1; i < data._nbItems+1; ++i) { // d'objet 1 au n-ème*
		
		// on fait le produit scalaire entre u_Q (vecteur de pénalités) et K[i], 
		// le vecteur qui représente l'appartenance de l'objet i aux différentes classes
		double somme_u_Q; 
		somme_u_Q = produitScalaire(u_Q, K[i-1]); 
		cout << data._profits[i-1] - somme_u_Q << " ";
		for(int j = 0; j < data._capacity+1; ++j) { // de capacité 0 à C
			
			int m; 
			m = min(j/data._weights[i-1], 1); 
			double max = 0; 
			
			// pred permettra de stocker le c de l'état (i-1, c) d'où l'on vient. 
			// on pourra accéder au prédécesseur d'un état grâce à tab[c][i].second
			int pred;

			// modif : au lieu d'itérer sur 0,...,min(..) on boucle sur 0,m qui vaut soit 0 soit 1
			// revient à étudier le cas 0 ou 1. 
			for(int k = 0; k <= m; ++k) { 

				double courant;  

				courant = k*(data._profits[i-1] - somme_u_Q) + tab[j-(k*data._weights[i-1])][i-1].first; 
				if(courant > max) {
					max = courant; 
					// pred prends la valeur du c d'où l'on vient
					pred = j-(k*data._weights[i-1]);
				}

			}
			tab[j][i].first = max; 
			tab[j][i].second = pred; 
		}
	}
	cout << endl;
	
	// récupérage de la solution 
	 
	sol = getSolution(tab, data._nbItems, data._capacity);

	// avant l'affichage, on ajoute le terme constant somme_[Q \in \mathcal{Q}] u_Q  qui nous permettra d'obtenir la borne duale 
	double somme = 0; 
	for(uint i = 0; i < u_Q.size(); ++i) {
		somme += u_Q[i]; 
	}
	cout << "S " << somme << endl;
	cout << "tab " << tab[data._capacity][data._nbItems].first << endl;
	double val = tab[data._capacity][data._nbItems].first + somme;  
/*
	// DEBUG ############################################################################
	double sum_profit = 0;
	vector<double> clique_penalties(u_Q.size(), 0.0);
	
	// Partie 1 : somme des profits
	for (int i = 0; i < data._nbItems; ++i) {
		if (sol[i] == 1) {
			sum_profit += data._profits[i];
			// On met à jour les pénalités par clique
			for (int j = 0; j < K[i].size(); ++j) {
				if (K[i][j] == 1) {
					clique_penalties[j] += 1.0;
				}
			}
		}
	}
	
	// Partie 2 : pénalité
	double penalty = 0;
	for (int q = 0; q < u_Q.size(); ++q) {
		penalty += u_Q[q] * (clique_penalties[q] - 1);
	}
	
	double L_manual = sum_profit - penalty;
	
	cout << "### DEBUG CALCUL MANUEL ###" << endl;
	cout << "Solution x : ";
	for (int x : sol) cout << x << " ";
	cout << "\nProfit sum: " << sum_profit << endl;
	cout << "Clique penalties : ";
	for (double v : clique_penalties) cout << v << " ";
	cout << "\nPenalty term: " << penalty << endl;
	cout << "Manual Lagrange value: " << L_manual << endl;
	cout << "Computed value: " << val << endl;
	
*/

	return val; // renvoie la valeur optimale du lagrangien. 
}

// fonction qui vérifie qu'une solution est bien réalisable 

bool isRealisable(const C_DKPData& data, const vector<int>& sol, double& solValue, const vector<vector<int>>& K) {
	// vérifier la contrainte de capacité et en même temps calculer solValue la valeur de la solution dans le PLNE
	solValue = 0; 
	double capacityUsed = 0; 

	for(int i = 0; i < data._nbItems; ++i) { // calcul valeur et capacité utilisée
		solValue += sol[i]*data._profits[i]; 
		capacityUsed += sol[i]*data._weights[i]; 
	}

	if(capacityUsed <= data._capacity) { // si ça respecte la capacité du sac, on va vérifier des contraintes de conflit

		vector<int> cliquesImplicated(data._nbCliques, 0); // cliquesImplicated[i] représente le nb d'obj de la clique présent dans la sol. 
		// suffit de détecter un indice i tel que cliquesImplicated[i]>1 et on sait que la contrainte est violée
		for(int i = 0; i < data._nbItems; ++i) {
			if(sol[i]) { // si i utilisé
				for(int j = 0; j < K[i].size(); ++j) {
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


void nouveau_u(const C_DKPData& data, const vector<int>& sol, vector<double>& u_Q , double bornePrimale, double valLagrangien, double alpha) {

	vector<double> constrValues; 
	double val; 
	// calcul du dénominateur (norme euclidienne de la contrainte relaxée)
	for(int i = 0; i < data._nbCliques; ++i) { // pr tt clique
		val = 0; 
		for(int j = 0; j < data._cliques[i].size(); ++j) {
			val += sol[data._cliques[i][j]]; // on ajoute les xi qui sont dans la clique pr la solution courante 
		}
		constrValues.push_back(val-1);
	}
	// DEBUG  affichage constrValues
	cout << "CONSTRVALUES : "; 
	for(uint i = 0; i < constrValues.size(); ++i) {
		cout << constrValues[i] << " "; 
	}
	cout << endl;

	// produit scalaire 
	double denominateur = 0; 
	for(uint i = 0; i < constrValues.size(); ++i) {
		denominateur += constrValues[i]*constrValues[i]; 
	}
	cout << "DENOMINATEUR : " << denominateur <<  endl;

	//double S = max(0.0, alpha*(valLagrangien - bornePrimale)/denominateur);
	double S; 
	if (denominateur == 0) {
		S == -1; // permet de renvoyer nuew_UQ avec que des 0
	} else  {
		S = alpha*(valLagrangien - bornePrimale)/denominateur; 
	} 

	vector<double> new_u_Q; 
	for(uint i = 0; i < u_Q.size(); ++i) {
		new_u_Q.push_back(max(u_Q[i] + S*constrValues[i], 0.0)); // on prend le max car on a l'opérateur + a coté de l'accolade qui est un opérateur de projection signifiant que chq comp du vec est >=0
	}

	u_Q = new_u_Q; // maj du u_Q
}

// fonction qui implémente les sous-gradients 

void sousGradients(const C_DKPData& data, double alpha, int M) {

	vector<double> u_Q(data._nbCliques, 0.0); // initie les u_Q à 0. 
	vector<int> sol; // vecteur des solutions 
	vector<int> optSol(data._nbItems, 0); // stockera solution optimale. Au début, on a choisis de prendre BD = 0 avec solOpt : prendre 0 objet
	int count = 0; // compteur qui permettra de stopper l'algorithme après un trop grand nombre d'itérations.
	double lagrangeValue, plneValue;  // valeur trouvée dans Lagragien, valeur ds plne, S le pas s^t
	double bornePrimale = 0; // au début on va dire qu'une solution réalisable est de ne rien prendre donc BP = 0
	double borneDuale = numeric_limits<double>::infinity(); // au début initialisé à l'infini 
	bool isRea; 

	vector<vector<int>> K; // contiendra l'appartenance des objets aux cliques. 
	K = inClique(data); 

	do {
		cout << "### starting new iteration ###" << endl;

		isRea = false; 

		lagrangeValue = sacDynamique(data, u_Q, sol, K); // résoud lagrangien pour u fixé

		if (lagrangeValue - borneDuale < -1e-3) { // si on améliore BD
			borneDuale = lagrangeValue;  
			count = 0; // remet compteur à 0
		} else {
			++ count; 
		}

		isRea = isRealisable(data, sol, plneValue, K); // regarde si c'est réalisable et extrait la valeur du plne 
		// DEBUG

		cout << "FOUND SOL : "; 
		cout << "["; 
		for(uint i = 0; i < sol.size(); ++i) {
			cout << sol[i] << ", "; 
		} cout << "]" << endl;
		cout << "WITH VALUE : " << plneValue << endl;
		cout << "GOT : " << isRea << endl;
		cout << "LAGRANGEVALUE AT THIS STADE : " << lagrangeValue << endl;
		cout << "BD AND BP AND THIS RATE : " << borneDuale << "/" << bornePrimale << endl;
		cout << "VECTOR OF UQ  : "; 
		for(uint i= 0; i < u_Q.size(); ++i) {
			cout << u_Q[i] << ", "; 
		} cout << endl;
		
		cout << "profits : ["; 
		for(uint i = 0;i<data._nbItems; ++i) {
			cout << data._profits[i] << ", "; 
		}
		cout << "]" << endl;

		cout << "poids : ["; 
		for(uint i = 0; i < data._nbItems; ++i) {
			cout << data._weights[i] << ", "; 
		} 
		cout << "]" << endl;

		if(isRea) {
			
			if(abs(lagrangeValue - plneValue) < 1e-4) { // tolérance
				cout << "OPTIMUM DETECTE ICI" << endl;
				optSol = sol; 
				bornePrimale = plneValue; 
				break; // on sait que plneValue est optimale
			}
			if(plneValue > bornePrimale) {
				bornePrimale = plneValue; // maj BP
				optSol = sol; // on stocke la solution la + optimale
			}
		}

		// maj du u 
		nouveau_u(data, sol, u_Q, bornePrimale, lagrangeValue, alpha);
		alpha = alpha * 0.99; 
		cout << "count : " << count << endl;
		if(count > M*0.9) {
			alpha = alpha *1.5; 
		} 
	} while(count <= M); 
	
	// affichage des résultats 
	cout << "## Solution optimale trouvée : [ "; 
	for(uint i = 0; i < optSol.size(); ++i) {
		cout << optSol[i] << " "; 
	} 
	cout << "]" << endl;
	cout << "de valeur : " << bornePrimale << endl;

	cout << "Best dual bound : " << borneDuale << endl;
}