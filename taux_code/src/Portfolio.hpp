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
        
        Portfolio(int n);
        void ChangeAllQuantities(const PnlVect *values, const PnlVect *deltas, int t);
};
