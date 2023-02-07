#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

/// \brief Classe Option abstraite
class Option
{
  public:
    double T_;        /// maturité
    PnlVect* size_;  /// nombre d'actifs de chacun des marchés (dans l'ordre : dom, etrangers)
    int n_; /// nombre de marchés étrangers
    double domesticRate_;        /// dimension du modèle, redondant avec BlackScholesModel::size_
    /**
     * Calcule la valeur du payoff sur la trajectoire
     *
     * @param[in] path est une matrice de taille (N+1) x d
     * contenant une trajectoire du modèle telle que créée
     * par la fonction asset.
     * @return phi(trajectoire)
     */
    virtual double payoff(const PnlMat* path, double t) = 0;
};
