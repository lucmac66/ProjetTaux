#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "json_reader.hpp"

class Portfolio
{
    public:
        double freeRiskQuantity; /*! quantité d el'actif sans risque domestique pour la couverture */
        PnlVect *quantity;       /*! vecteur des quantités de chacun des actifs risqués */
        double value;            /*! valeur du portefeuille */
        int lastRebalance;       /*! entier représentant la date du dernier rebalancement */
        double rate_;            /*! taux d'intérêt domestique */
        int nbDays_;             /*! nombre de jours de rebalancement */

        /**
         * Constructeur de la classe Portfolio
         *
         * @param n
         * @param nbDays
         * @param rate
         */
        Portfolio(int n, int nbDays, double rate);

        /**
         * Fonction qui change les quantités des actifs risqués et de l'actif sans risque
         *
         * @param values matrice des valeurs des actifs à la date t
         * @param deltas deltas des actifs à la date t
         * @param t date de rebalancement
         */
        void ChangeAllQuantities(const PnlMat *values, const PnlVect *deltas, int t);

        /**
         * Fonction qui change les quantités des actifs risqués et de l'actif sans risque
         * @param values matrice des valeurs des actifs à la date t
         * @param deltas deltas des actifs à la date t
         * @param t date de rebalancement
         * @param prix prix de l'option à la date t
         */
        void ChangeAllQuantities(const PnlMat *values, const PnlVect *deltas, int t, double prix);
};


