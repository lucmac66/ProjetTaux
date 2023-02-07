#pragma once
#include "Currency.hpp"
#include "pnl/pnl_vector.h"

class RiskyAsset{
    public:
    Currency *currency_;
    double drift_;
    PnlVect *sigma_;

    RiskyAsset(Currency *currency, PnlVect *sigma);   
};