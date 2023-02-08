#include "ForeignAsian.hpp"

ForeignAsian::ForeignAsian(double T, double domestricRate){
    this->T_ = T;
    this->domesticRate_ = domestricRate;
}

double ForeignAsian::payoff(const PnlMat* path, double t){
    double payoff = 0;
    for (int i = 0; i < path->m; i++){
        payoff += pnl_mat_get(path, i, 1);
    }
    payoff /= path->m;
    payoff -= pnl_mat_get(path, path->m-1, 0);
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)) * payoff;
    }
    return 0.;

}