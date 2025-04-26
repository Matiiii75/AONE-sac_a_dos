#include "sacDyna.hpp"

double sacDynamique(const C_DKPData& data,const vector<double>& u_Q, vector<int>& sol,const vector<vector<int>>& K) {
	

	// taille : nb lignes / nbObj = nb colonnes
	vector<vector<pair<double,int>>> tab(data._capacity+1, vector<pair<double,int>>(data._nbItems+1)); 

	// intialisation de la première colonne a 0 (états initiaux) :  
	for(int i = 0; i < data._capacity+1; ++i) {
		
		tab[i][0].first = 0; 
		tab[i][0].second = 0; 
	}
	for(int i = 1; i < data._nbItems+1; ++i) { // d'objet 1 au n-ème*
		
		// on fait le produit scalaire entre u_Q (vecteur de pénalités) et K[i], 
		// le vecteur qui représente l'appartenance de l'objet i aux différentes classes
		double somme_u_Q; 
		somme_u_Q = produitScalaire(u_Q, K[i-1]); 
		
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
	
	// récupérage de la solution 
	 
	sol = getSolution(tab, data._nbItems, data._capacity);

	// avant l'affichage, on ajoute le terme constant somme_[Q \in \mathcal{Q}] u_Q  qui nous permettra d'obtenir la borne duale 
	double somme = 0; 
	for(uint i = 0; i < u_Q.size(); ++i) {
		somme += u_Q[i]; 
	}

	double val = tab[data._capacity][data._nbItems].first + somme;  

	return val; // renvoie la valeur optimale du lagrangien. 
}