#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"

class Portfolio
{
    public:
        double freeRiskQuantity;
        PnlMat *quantity;
        double value;
        int lastRebalance;
        
        Portfolio(int n , int maxN);
        void ChangeAllQuantities(const PnlMat *values, const PnlMat *deltas, int t);
}
