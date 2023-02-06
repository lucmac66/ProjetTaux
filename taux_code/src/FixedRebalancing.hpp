#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Rebalancing.hpp"

class FixedRebalancing : public Rebalancing
{
    public:
        int period;
        FixedRebalancing(int startDate, int period);
        bool IsRebalanceDate(int dateT) override;

};
