#include "QuantoExchange.hpp"

QuantoExchange::QuantoExchange(double T, double strike, double domestricRate, int year){
    this->T_ = T;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
    this->year_ = year;
}

double QuantoExchange::payoff(const PnlMat* path, double t){
    double payoff = pnl_mat_get(path, path->m-1, 0) - pnl_mat_get(path, path->m-1, 1) - strike_;
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)/this->year_) * payoff;
    }
    return 0.;

}