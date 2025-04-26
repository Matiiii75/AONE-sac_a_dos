#pragma once 

#include "common.hpp"

/**
 * @brief Résoud un problème de sac à dos borné à 1 exemplaire par objets
 * @param data données du pb
 * @param u_Q vecteur des coefficients de Lagrange
 * @param sol passé en référence, permet de récupérer la solution trouvée
 * @param K vecteur d'appartenance aux cliques
 * @return la valeur optimale trouvée pour le sac à dos
 */
double sacDynamique(const C_DKPData& data, const vector<double>& u_Q, vector<int>& sol, const vector<vector<int>>& K);