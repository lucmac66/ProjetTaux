#pragma once
#include "pnl/pnl_vector.h"
#include <iostream>
#include <string>

using namespace std;

class Currency{
    public:
    string id_;
    double domesticRate_;
    double foreignRate_;
    double drift_;
    PnlVect* sigma_;
    Currency(string id, double domesticRate, double foreignRate, PnlVect* sigma);
};

