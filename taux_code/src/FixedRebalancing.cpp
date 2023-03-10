#include "FixedRebalancing.hpp"

FixedRebalancing::FixedRebalancing(int period){
    this->lastRebalance = 0;
    this->period = period;
}

bool FixedRebalancing::IsRebalanceDate(int dateT){
    if (dateT - this->lastRebalance >= this->period){
        this->lastRebalance = dateT;
        return true;
    }
    else{
        return false;
    }
}