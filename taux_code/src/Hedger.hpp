#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Portfolio.hpp"

class Hedger
{
    public:
        Portfolio portfolio;
        PnlMat * marketData;
        
        Hedger(Portfolio portfolio , string csvFileName);
        void RebalanceAll();
        void RebalanceOnce(int date, PnlMat *marketData);
}
