#pragma once

#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "math.h"

class MonteCarlo
{
  public:
    BlackScholesModel* mod_; /*! pointeur vers le modèle */
    Option* opt_;            /*! pointeur sur l'option */
    PnlRng* rng_;            /*! pointeur sur le générateur */
    long nbSamples_;         /*! nombre de tirages Monte Carlo */

    MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, long nbSamples);

    void priceAndDeltas(const PnlMat* past, double t, double& prix, double& std_dev, PnlVect *deltas, PnlVect *stdDeltas, double epsilon);
};
