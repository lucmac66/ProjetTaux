#include "Portfolio.hpp"
#include <iostream>
#include <cmath>

Portfolio::Portfolio(int n, int nbDays, double rate){
    this->freeRiskQuantity = 0;
    this->quantity = pnl_vect_create_from_zero(n);
    this->value = 0;
    this->rate_ = rate;
    this->nbDays_ = nbDays;
    this->lastRebalance = 0;

}

void Portfolio::ChangeAllQuantities(const PnlMat *values, const PnlVect * deltas, int t){
    double capitalisation = exp(this->rate_ * (t - this-> lastRebalance) / this->nbDays_); 
    this->freeRiskQuantity *= capitalisation;
    this->value = this->freeRiskQuantity;
    double price = 0;
    for (int i = 0; i < this->quantity->size; i++){
        double delta_i_plus = pnl_vect_get(deltas, i);
        double delta_i_moins = pnl_vect_get(this->quantity, i);
        double value = pnl_mat_get(values, values->m-1, i);
        this->freeRiskQuantity += value * (delta_i_moins - delta_i_plus);
        pnl_vect_set(this->quantity, i, delta_i_plus);
        price += value * delta_i_moins;
    }
    this->value += price;
    this->lastRebalance = t;
}

void Portfolio::ChangeAllQuantities(const PnlMat *values, const PnlVect * deltas, int t, double prix){

    double price = 0;
    for (int i = 0; i < this->quantity->size; i++){
        double delta = pnl_vect_get(deltas, i);
        double value = pnl_mat_get(values, values->m-1, i);
        pnl_vect_set(this->quantity, i, delta);
        price += value * delta;
    }
    this->freeRiskQuantity = prix - price;
    this->value = price + this->freeRiskQuantity;
    this->lastRebalance = t;
}

Position::Position(int date, double price, double priceStdDev, PnlVect* deltas, PnlVect* deltasStdDev, double portfolioValue)
    : date(date), price(price), priceStdDev(priceStdDev), portfolioValue(portfolioValue), deltas(deltas), deltasStdDev(deltasStdDev) {

}

void to_json(nlohmann::json &j, const Position &position) {
    j["date"] = position->date;
    j["value"] = position->portfolioValue;
    j["price"] = position->price;
    j["priceStdDev"] = position->priceStdDev;
    j["deltas"] = position->deltas;
    j["deltasStdDev"] = position->deltasStdDev;
}

void Position::print() const {
    nlohmann::json j = *this;
    std::cout << j.dump(4);
}
