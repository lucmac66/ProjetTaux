#pragma once
#include "pnl/pnl_vector.h"
#include <iostream>
#include <string>

using namespace std;

class Currency{
    public:
    string id_;           /*! identifiant de la monnaie */
    double domesticRate_; /*! taux d'intérêt domestique */
    double foreignRate_;  /*! taux d'intérêt étranger */
    double drift_;        /*! drift */
    PnlVect* sigma_;      /*! volatilité */

    /**
     * Constructeur de la classe
     *
     * @param id identifiant de la monnaie
     * @param domesticRate taux d'intérêt domestique
     * @param foreignRate taux d'intérêt étranger
     * @param corr vecteur de corrélation
     * @param sigma volatilité de la monnaie
     */
    Currency(string id, double domesticRate, double foreignRate, PnlVect* corr, double sigma);
};

