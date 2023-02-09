#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"

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
