#pragma once 

#include "common.hpp"
#include "sacDyna.hpp"

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
void nouveau_u(const C_DKPData& data, const vector<int>& sol, vector<double>& u_Q, int bornePrimale, double valLagrangien, double alpha);

/**
 * @brief applique les sous-gradients pour trouver les u optimaux et ainsi résoudre le problème de sac à dos avec contraintes de compatibilité
 * @param data données du pb
 * @param alpha entre 0 et 2, fournit par l'utilisateur
 * @param M nombre d'irétation max sans améliorer la solution, saisit par l'utilisateur
 * @param filename instance traitée  
 */
void sousGradients(const C_DKPData& data, double alpha, int M, const string& filename); 