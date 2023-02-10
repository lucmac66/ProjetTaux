#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <iostream>
#include <vector>

using namespace std;

/// \brief Classe Option abstraite
class Option
{
  public:
    double T_;               /*! maturité */
    double domesticRate_;    /*! taux d'intérêt domestique */
    int year_;               /*! nombre de jours dans l'année */
    vector<int> sizemarket_; /*! tailles des marchés */

    /**
     * Calcul du payoff
     *
     * @param path matrice de la trajectoire simulée
     * @param t temps auquel on calcule le payoff
     * @return le payoff de l'option
     */
    virtual double payoff(const PnlMat* path, double t) = 0;
};
