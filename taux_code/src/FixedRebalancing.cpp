#include "FixedRebalancing.hpp"

FixedRebalancing::FixedRebalancing(int startDate, int period){
    this->lastRebalance = startDate;
    this->period = period;
}

bool FixedRebalancing::IsRebalanceDate(int dateT){
    if (dateT - this->lastRebalance > this->period){
        this->lastRebalance = dateT;
        return true;
    }
    else{
        return false;
    }
}