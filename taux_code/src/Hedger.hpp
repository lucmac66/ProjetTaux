#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Portfolio.hpp"
#include "MonteCarlo.hpp"
#include "Rebalancing.hpp"
#include <list>
#include "Position.hpp"

using namespace std;
class Hedger
{
    public:
        Portfolio* portfolio_;         /*! portefeuille */
        PnlMat * marketData_;          /*! matrice des données de marché */
        MonteCarlo *mc_;               /*! objet Monte Carlo pour calculer le prix et les deltas */
        Rebalancing *rebalancingTool_; /*! objet de rebalancement */
        list<Position> positions;      /*! liste des positions pour le rebalancement à tout instant t */
        double epsilon_;               /*! variable pour le simuler une trajectoire shiftée */

        /**
         * Constructeur de la classe
         *
         * @param portfolio
         * @param csvFileName
         * @param mc
         * @param rebalancingTool
         * @param marketsize
         * @param epsilon
         */
        Hedger(Portfolio *portfolio , char* csvFileName, MonteCarlo *mc, Rebalancing *rebalancingTool, vector<int> marketsize, double epsilon);

        /**
         * Fonction qui effectue le rebalancement du portefeuille jour par jour sur le nombre de jours associé aux paramètres de l'option
         */
        void RebalanceAll();

        /**
         * Fonction qui effectue un seul rebalancement à la date t
         *
         * @param date date à laquelle on rebalnce le portefeuille
         * @param marketData matrice des données de marché
         */
        void RebalanceOnce(int date, PnlMat *marketData);

        /**
         * Fonction qui extrait les données de marché à la date passée en paramètre
         *
         * @param date date à laquelle on extrait les données de marché
         * @return une matrice des données de marché
         */
        PnlMat *ExtractMarketData(int date);

        /**
         * Extraction de données depuis un fichier csv
         *
         * @param name nom du fichier csv
         * @param marketsize taille des marchés
         * @return données de marché extraite sous forme de matrice
         */
        PnlMat *ExtractCsv(char* name, vector<int> marketsize);
};
