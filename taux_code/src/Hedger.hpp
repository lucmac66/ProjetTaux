#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Portfolio.hpp"
#include "MonteCarlo.hpp"
#include "Rebalancing.hpp"

class Hedger
{
    public:
        Portfolio* portfolio_;
        PnlMat * marketData_;
        MonteCarlo *mc_;
        Rebalancing *rebalancingTool_;
        
        Hedger(Portfolio *portfolio , string csvFileName, MonteCarlo *mc, Rebalancing *rebalancingTool);
        void RebalanceAll();
        void RebalanceOnce(int date, PnlMat *marketData);
        PnlMat *ExtractMarketData(int date);
        PnlMat *ExtractCsv(string name);
};
