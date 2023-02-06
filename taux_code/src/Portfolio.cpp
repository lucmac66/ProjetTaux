#include "Portfolio.hpp"

Portfolio::Portfolio(int n, int maxN){
    this->freeRiskQuantity = 0;
    this->quantity = pnl_mat_create_from_zero(n, maxN);
    this->value = 0;
}

void Portfolio::ChangeAllQuantities(const PnlMat *values, const PnlMat * deltas, int t){
    double capitalisation = 1; // exp à faire
    this->freeRiskQuantity *= capitalisation;
    double price = 0;
    for (int i = 0; i < deltas->m; i++){
        for (int j = 0; j < deltas->n; j ++){
            double delta = pnl_mat_get(deltas, i, j);
            double value = pnl_mat_get(values, i, j);
            this->freeRiskQuantity += value * (pnl_mat_get(this->quantity, i , j) - delta);
            pnl_mat_set(this->quantity, i, j, delta);
            price += value * delta;
        }
    }
    this->value = price + this->freeRiskQuantity;
    this->lastRebalance = t;
}

