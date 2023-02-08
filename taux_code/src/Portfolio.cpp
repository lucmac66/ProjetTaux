#include "Portfolio.hpp"
#include <iostream>
Portfolio::Portfolio(int n){
    this->freeRiskQuantity = 0;
    this->quantity = pnl_vect_create_from_zero(n);
    this->value = 0;
}

void Portfolio::ChangeAllQuantities(const PnlMat *values, const PnlVect * deltas, int t){
    double capitalisation = 1; // exp à faire
    this->freeRiskQuantity *= capitalisation;
    double price = 0;
    for (int i = 0; i < this->quantity->size; i++){
        double delta = pnl_vect_get(deltas, i);
        double value = pnl_mat_get(values, values->m-1, i);
        this->freeRiskQuantity += value * (pnl_vect_get(this->quantity, i) - delta);
        pnl_vect_set(this->quantity, i, delta);
        price += value * delta;
    }
    this->value = price + this->freeRiskQuantity;
    this->lastRebalance = t;
}

