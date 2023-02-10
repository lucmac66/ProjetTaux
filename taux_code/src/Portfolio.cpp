#include "Portfolio.hpp"
#include <iostream>
#include <cmath>

Portfolio::Portfolio(int n, int nbDays, double rate, vector<RiskyAsset*> assets, vector<Currency*> currencies){
    this->freeRiskQuantity = 0;
    this->quantity = pnl_vect_create_from_zero(n);
    this->value = 0;
    this->rate_ = rate;
    this->nbDays_ = nbDays;
    this->lastRebalance = 0;
    this->assets_ = assets;
    this->currencies_ = currencies;
}

void Portfolio::ChangeAllQuantities(const PnlMat *values, PnlVect * deltas, int t){
    double capitalisation = exp(this->rate_ * (t - this-> lastRebalance) / this->nbDays_); 
    this->freeRiskQuantity *= capitalisation;
    this->value = this->freeRiskQuantity;
    double price = 0;
    int nbAssets = this->assets_.size();
    for (int i = 0; i < this->quantity->size; i++){
        if(i < nbAssets){
            double delta_i_plus = pnl_vect_get(deltas, i);
            double delta_i_moins = pnl_vect_get(this->quantity, i);
            double value = pnl_mat_get(values, values->m-1, i);
            this->freeRiskQuantity += value * (delta_i_moins - delta_i_plus);
            pnl_vect_set(this->quantity, i, delta_i_plus);
            price += value * delta_i_moins;

        }
        else{
            double delta_i_plus = pnl_vect_get(deltas, i) * exp(2 * this->currencies_[i - nbAssets]->foreignRate_ * (t - this-> lastRebalance) / this->nbDays_);
            double delta_i_moins = pnl_vect_get(this->quantity, i);
            pnl_vect_set(deltas, i, delta_i_plus);
            double value = pnl_mat_get(values, values->m-1, i);
            this->freeRiskQuantity += value * (delta_i_moins - delta_i_plus);
            pnl_vect_set(this->quantity, i, delta_i_plus);
            price += value * delta_i_moins;
        }
        
    }
    this->value += price;
    this->lastRebalance = t;
}

void Portfolio::ChangeAllQuantities(const PnlMat *values, PnlVect * deltas, int t, double prix){

    double price = 0;
    int nbAssets = this->assets_.size();
    for (int i = 0; i < this->quantity->size; i++){
        if(i < nbAssets){
            double delta = pnl_vect_get(deltas, i);
            double value = pnl_mat_get(values, values->m-1, i);
            pnl_vect_set(this->quantity, i, delta);
            price += value * delta;
        }
        else{
            double delta = pnl_vect_get(deltas, i) * exp(2 * this->currencies_[i - nbAssets]->foreignRate_ * (t - this-> lastRebalance) / this->nbDays_);
            pnl_vect_set(deltas, i, delta);
            double value = pnl_mat_get(values, values->m-1, i);
            pnl_vect_set(this->quantity, i, delta);
            price += value * delta;
        }
    }
    this->freeRiskQuantity = prix - price;
    this->value = price + this->freeRiskQuantity;
    this->lastRebalance = t;
}

