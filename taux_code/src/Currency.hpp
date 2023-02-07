#pragma once
#include "pnl/pnl_vector.h"

class Currency{
    public:
    double domesticRate_;
    double foreignRate_;
    double drift_;
    PnlVect *sigma_;
    Currency(double domesticRate, double foreignRate, PnlVect *sigma);
};

