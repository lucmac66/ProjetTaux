#include "GridRebalancing.hpp"

GridRebalancing::GridRebalancing(PnlVect * dates){
    this->lastRebalance = 0;
    this->dates = dates;
}

bool GridRebalancing::IsRebalanceDate(int dateT){
    for(int i = 0; i < this->dates->size; i++){
        if (pnl_vect_get(dates, i) == dateT){
            this->lastRebalance = dateT;
            return true;
        }
    }
    return false;
}