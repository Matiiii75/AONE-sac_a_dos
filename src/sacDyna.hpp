#pragma once

#include "DKP_Data.hpp"

void afficheResults(const vector<vector<pair<double,int>>>& tab);

/**
 * @brief récupère la solution trouvée en parcourant à revers le tableau grâce aux pred stockés. En faisant la différence entre chaque labels,
 *  on sait quels objets se trouvent dans la solution.
 * @param tab le tableau dynamique 
 * @param nbObj nombre objets
 * @param taille la capacité du sac
 * @return vecteur représentant la solution au problème
 */
vector<int> getSolution(const vector<vector<pair<double,int>>>& tab, int nbObj, int taille);

/**
 * @brief Résoud un problème de sac à dos borné à 1 exemplaire par objets
 * @param data données du pb
 * @param u_Q vecteur des coefficients de Lagrange
 * @param sol passé en référence, permet de récupérer la solution trouvée
 * @param K vecteur d'appartenance aux cliques
 * @return la valeur optimale trouvée pour le sac à dos
 */
double sacDynamique(const C_DKPData& data, const vector<double>& u_Q, vector<int>& sol, const vector<vector<int>>& K);

/**
 * @brief fonction qui créer un une matrice K représentant l'appartenance des objets aux cliques; 
 * + précisemment : K[i] est un vecteur binaire où K[i][j] vaut 1 si i est dans la clique j
 * @param data les données du pb
 * @return K la matrice des appartenances aux cliques
 */
vector<vector<int>> inClique(const C_DKPData& data); 

void afficheK(const vector<vector<int>>& K); 

/**
 * @brief Vérifie si une solution trouvée par le Lagragien est réalisable. Permet aussi d'obtenir sa valeur dans le PLNE.
 * @param data données du pb
 * @param sol la solution trouvée par le lagrangien pour laquelle on fais les vérifs
 * @param solValue la valeur de la solution du PLNE sera retournée grâce au passage en référence de ceci
 * @param K le tableau inCliques
 * @return true si c'est réalisable et false sinon
 */
bool isRealisable(const C_DKPData& data, const vector<int>& sol, double& solValue, const vector<vector<int>>& K);

void saisir_u_Q(vector<double>& u_Q, const C_DKPData& data); 

/**
 * @brief calcule produit scalaire. Attention, le premier vecteur saisit en argument doit être un vecteur de doubles
 * @param X1 vecteur de double généralement le vecteur uQ pour moi
 * @param X2 vecteur d'entiers
 * @return résultat du produit scalaire
 */
double produitScalaire(const vector<double>& X1, const vector<int>& X2); 

/**
 * @brief Calcule le nouveau u
 * @param data données du pb
 * @param sol solution x (étape 2 du pseudo code) du lagrangien
 * @param u_Q c'est le vecteur de uQ actuels, passé par référence, il sera maj
 * @param bornePrimale BP actuelle 
 * @param valLagragien la valeur de la solution sol dans le lagrangien
 * @param alpha alpha choisit 
 * 
 */
void nouveau_u(const C_DKPData& data, const vector<int>& sol, vector<double>& u_Q , double bornePrimale, double valLagrangien, double alpha);

/**
 * @brief applique les sous-gradients pour trouver les u optimaux et ainsi résoudre le problème de sac à dos avec contraintes de compatibilité
 * @param data données du pb
 * @param alpha entre 0 et 2, fournit par l'utilisateur
 * @param M nombre d'irétation max sans améliorer la solution, saisit par l'utilisateur
 */
void sousGradients(const C_DKPData& data, double alpha, int M); 