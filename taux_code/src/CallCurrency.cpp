#include "CallCurrency.hpp"

CallCurrency::CallCurrency(double T, double strike, double domestricRate, double foreignRate){
    this->T_ = T;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
    this->foreignRate_ = foreignRate;
}

double CallCurrency::payoff(const PnlMat* path, double t){
    double payoff = pnl_mat_get(path, path->m-1, 0) * exp(this->foreignRate_ * this-> T_) - this->strike_;
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)) * payoff;
    }
    return 0.;

}