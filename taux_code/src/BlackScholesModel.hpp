#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "RiskyAsset.hpp"
#include "Currency.hpp"
#include <string>
#include <iostream>
#include <vector>
#include "math.h"

using namespace std;

/// \brief Modèle de Black Scholes
class BlackScholesModel
{
  public:
    vector<RiskyAsset*> assets_;
    vector<Currency*> currencies_;
    PnlVect* importantDates_;
    int year_; ///number of days in a year

    BlackScholesModel(PnlVect* importantDates, vector<RiskyAsset*> assets, vector<Currency*> currencies, int year);

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */

    void asset(PnlMat* path, const PnlMat *past, double t, PnlRng* rng);

    void shiftAsset(PnlMat* path, const PnlMat* past, double epsilon, double t, int column);

    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */
};
