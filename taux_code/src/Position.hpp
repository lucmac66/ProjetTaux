#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "json_reader.hpp"
#include <list>

class Position {
public:
    int date;              /*! date de la position */
    PnlVect *deltas;       /*! vecteur des deltas */
    PnlVect *deltasStdDev; /*! vecteur des écart-types des deltas */
    double price;          /*! prix de l'option */
    double priceStdDev;    /*! écart-type du prix */
    double portfolioValue; /*! valeur du portefeuille de réplication */

    /**
     * Constructeur de la classe Position
     *
     * @param date
     * @param price
     * @param priceStdDev
     * @param deltas
     * @param deltasStdDev
     * @param portfolioValue
     */
    Position(int date, double price, double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev, double portfolioValue);

    /**
     * fonction qui transmet les données au json
     *
     * @param j
     * @param positions
     */
    friend void to_json(nlohmann::json &j, const Position &positions);

    /**
     * Fonction qui transcrit les résultats de la calibration sur le fichier json de l'output
     */
    void print() const;
};