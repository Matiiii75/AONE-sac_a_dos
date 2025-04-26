#include "sousGradient.hpp"

void nouveau_u(const C_DKPData& data, const vector<int>& sol, vector<double>& u_Q , int bornePrimale, double valLagrangien, double alpha) {

	vector<int> constrValues; 
	int val; 
	// calcul du dénominateur (norme euclidienne de la contrainte relaxée)
	for(int i = 0; i < data._nbCliques; ++i) { // pr tt clique
		val = 0; 
		for(uint j = 0; j < data._cliques[i].size(); ++j) {
			val += sol[data._cliques[i][j]]; // on ajoute les xi qui sont dans la clique pr la solution courante 
		}
		constrValues.push_back(val-1);
	}

	// produit scalaire 
	int denominateur = 0; 
	for(uint i = 0; i < constrValues.size(); ++i) {
		denominateur += constrValues[i]*constrValues[i]; 
	}

	//double S = max(0.0, alpha*(valLagrangien - bornePrimale)/denominateur);
	double S; 
	if (denominateur == 0) {
		S = -1; // permet de renvoyer nuew_UQ avec que des 0
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

void sousGradients(const C_DKPData& data, double alpha, int M,const string& filename) {

	vector<double> u_Q(data._nbCliques, 0.0); // initie les u_Q à 0. 
	vector<int> sol; // vecteur des solutions 
	vector<int> optSol(data._nbItems, 0); // stockera solution optimale. Au début, on a choisis de prendre BD = 0 avec solOpt : prendre 0 objet
	int count = 0; // compteur qui permettra de stopper l'algorithme après un trop grand nombre d'itérations.
	double lagrangeValue; 
	int plneValue;  // valeur trouvée dans Lagragien, valeur ds plne, S le pas s^t
	int bornePrimale = 0; // au début on va dire qu'une solution réalisable est de ne rien prendre donc BP = 0
	double borneDuale = numeric_limits<double>::infinity(); // au début initialisé à l'infini 
	bool isRea; 
	char critereStop = 'I'; 
	//double originalAlpha = alpha;  copie le alpha original pr les stats

	vector<vector<int>> K; // contiendra l'appartenance des objets aux cliques. 
	K = inClique(data); 

	auto deb = std::chrono::high_resolution_clock::now(); 

	do {

		isRea = false; 

		lagrangeValue = sacDynamique(data, u_Q, sol, K); // résoud lagrangien pour u fixé

		if (lagrangeValue - borneDuale < -1e-3) { // si on améliore BD
			borneDuale = lagrangeValue;  
			count = 0; // remet compteur à 0
		} else {
			++ count; 
		}

		isRea = isRealisable(data, sol, plneValue, K); // regarde si c'est réalisable et extrait la valeur du plne 

		if(isRea) {
	
			if(plneValue > bornePrimale) {
				bornePrimale = plneValue; // maj BP
				optSol = sol; // on stocke la solution la + optimale
			}
			if(lagrangeValue - bornePrimale < 1) {
				critereStop = 'E'; // critère d'écart
				break;
			}
		}

		// maj du u 
		nouveau_u(data, sol, u_Q, bornePrimale, lagrangeValue, alpha);
		alpha = alpha * 0.99; 

	} while(count <= M); 

	auto fin = std::chrono::high_resolution_clock::now();
	double temps = chrono::duration_cast<chrono::milliseconds>(fin-deb).count(); 

	// AFFICHAGE DES RESULTATS

	cout << "## Affichage des resultats ##" << endl;
	cout << "temps: " << temps/1000 << " s" << endl;
	if(critereStop == 'E') {
		cout << "Solution optimale trouvee de valeur : " << bornePrimale << endl; // on peut considérer cette valeur comme une borne duale puisqu'elle amene à la solution optimale du pb ? 
	} 
	else {
		cout << "Algorithme stop car nombre d'iteration max atteint sans amelioration de BD" << endl; 
		cout << "Meilleure borne duale trouvee: " << borneDuale << endl;
		cout << "Meilleure borne primale trouvee: " << bornePrimale << endl;
	}

	//writeInFile(bornePrimale, borneDuale, temps, critereStop, filename, originalAlpha, M); 
}