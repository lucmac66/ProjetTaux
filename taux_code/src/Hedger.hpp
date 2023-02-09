#pragma once

#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Portfolio.hpp"
#include "MonteCarlo.hpp"
#include "Rebalancing.hpp"
#include <list>
#include "Position.hpp"

using namespace std;
class Hedger
{
    public:
        Portfolio* portfolio_;
        PnlMat * marketData_;
        MonteCarlo *mc_;
        Rebalancing *rebalancingTool_;
        list<Position> positions;
        double epsilon_;
        
        Hedger(Portfolio *portfolio , char* csvFileName, MonteCarlo *mc, Rebalancing *rebalancingTool, vector<int> marketsize, double epsilon);
        void RebalanceAll();
        void RebalanceOnce(int date, PnlMat *marketData);
        PnlMat *ExtractMarketData(int date);
        PnlMat *ExtractCsv(char* name, vector<int> marketsize);
};
