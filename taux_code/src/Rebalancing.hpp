#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"

class Rebalancing
{
    public:
        int lastRebalance;
        virtual bool IsRebalanceDate(int dateT) = 0;
};
