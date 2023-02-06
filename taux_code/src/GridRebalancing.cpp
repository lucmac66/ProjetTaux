#include "GridRebalancing.hpp"

GridRebalancing::GridRebalancing(int startDate, PnlVect * dates){
    this->lastRebalance = startDate;
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