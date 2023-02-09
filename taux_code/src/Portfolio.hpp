#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "json_reader.hpp"

class Portfolio
{
    public:
        double freeRiskQuantity;
        PnlVect *quantity;
        double value;
        int lastRebalance;
        double rate_;
        int nbDays_;
        
        Portfolio(int n, int nbDays, double rate);
        void ChangeAllQuantities(const PnlMat *values, const PnlVect *deltas, int t);
        void ChangeAllQuantities(const PnlMat *values, const PnlVect *deltas, int t, double prix);
};

class Position {
public:
    int date;
    PnlVect *deltas;
    PnlVect *deltasStdDev;
    double price;
    double priceStdDev;
    double portfolioValue;

    Position(int date, double price, double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev, double portfolioValue);
    friend void to_json(nlohmann::json &j, const Position &positions);
    void print() const;
};
