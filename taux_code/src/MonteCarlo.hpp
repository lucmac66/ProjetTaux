#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "math.h"
#include <numeric>

class MonteCarlo
{
  public:
    BlackScholesModel* mod_; /*! pointeur vers le modèle */
    Option* opt_;            /*! pointeur sur l'option */
    PnlRng* rng_;            /*! pointeur sur le générateur */
    long nbSamples_;         /*! nombre de tirages Monte Carlo */

    /**
     * Constructeur de la classe MonteCarlo
     *
     * @param mod
     * @param opt
     * @param rng
     * @param nbSamples
     */
    MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, long nbSamples);

    /**
     * Calcul des prix, deltas et leurs écart-types
     *
     * @param past matrice des observations passées
     * @param t temps auquel on calcule les prix et deltas
     * @param prix pointeur vers le prix qui sera modifié dans la fonction
     * @param std_dev pointeur vers l'écart-type du prix qui sera modifié dans la fonction
     * @param deltas vecteur des deltas qui sera modifié dans la fonction
     * @param stdDeltas vecteur des écart-types des deltas
     * @param epsilon paramètre pour simuler la trajectoire shiftée
     */
    void priceAndDeltas(const PnlMat* past, double t, double& prix, double& std_dev, PnlVect *deltas, PnlVect *stdDeltas, double epsilon);
};
