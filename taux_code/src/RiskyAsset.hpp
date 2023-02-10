#pragma once
#include "Currency.hpp"
#include "pnl/pnl_vector.h"
#include <iostream>
#include <string>

class RiskyAsset{
    public:
    double drift_;   /*! drift de l'asset risqué */
    PnlVect *sigma_; /*! vecteur de volatilité */

    /**
     * Constructeur de la classe RiskyAsset
     *
     * @param currency
     * @param corr
     * @param sigma
     */
    RiskyAsset(Currency *currency, PnlVect *corr, double sigma);   
};