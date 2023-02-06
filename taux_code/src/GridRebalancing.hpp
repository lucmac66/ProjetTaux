#include "Rebalancing.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"

class GridRebalancing : public Rebalancing
{
    public:
        PnlVect* dates;

        GridRebalancing(int startDate, PnlVect *dates);
        bool IsRebalanceDate(int dateT);

};
