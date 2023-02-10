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
    vector<RiskyAsset*> assets_;   /*! vecteur des actifs risqués */
    vector<Currency*> currencies_; /*! vecteur des monnaies */
    PnlVect* importantDates_;      /*! vecteur des dates importantes (d'observation) */
    int year_;                     /*! number of days in a year*/

    /**
     * Constructeur de la classe
     *
     * @param importantDates
     * @param assets
     * @param currencies
     * @param year
     */
    BlackScholesModel(PnlVect* importantDates, vector<RiskyAsset*> assets, vector<Currency*> currencies, int year);

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * @param[in] past matrice des observations historiques
     * @param[in] t date à partir de laquelle on génère la matrice path
     * @param[in] rng générateur de nombres aléatoires
     */
    void asset(PnlMat* path, const PnlMat *past, double t, PnlRng* rng);

    /**
     * Génère une trajectoire shiftée de epsilon de la column "column" de la matrice path
     *
     * @param path contient la trajectoire du modèle
     * @param past contient les observations historiques
     * @param epsilon paramètre de shift
     * @param t temps à partir duquel on génère la trajectoire shiftée
     * @param column colonne concernée par le shift
     */
    void shiftAsset(PnlMat* path, const PnlMat* past, double epsilon, double t, int column);
};
