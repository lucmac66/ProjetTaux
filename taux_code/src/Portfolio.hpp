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

    Portfolio();
    ChangeQuantity(const PnlMat *deltas);
}
