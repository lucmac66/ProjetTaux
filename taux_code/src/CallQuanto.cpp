#include "CallQuanto.hpp"

CallQuanto::CallQuanto(double T, int size, double strike, double domestricRate){
    this->T_ = T;
    this->size_ = size;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
}

double CallQuanto::payoff(const PnlMat* path, double t){
    double payoff = pnl_mat_get(path, path->m-1, 0) - this->strike_;
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)) * payoff;
    }
    return 0.;

}