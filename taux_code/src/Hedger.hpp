#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Portfolio.hpp"
#include "MonteCarlo.hpp"

class Hedger
{
    public:
        Portfolio portfolio;
        PnlMat * marketData;
        MonteCarlo *mc;
        
        Hedger(Portfolio portfolio , string csvFileName, MonteCarlo *mc);
        void RebalanceAll();
        void RebalanceOnce(int date, PnlMat *marketData);
};
