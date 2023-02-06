#include "ForeignAsian.hpp"

ForeignAsian::ForeignAsian(double T, int size, double domestricRate, int nbDates){
    this->T_ = T;
    this->size_ = size;
    this->domesticRate_ = domestricRate;
    this->nbDates_ = nbDates;
}

double ForeignAsian::payoff(const PnlMat* path, double t){
    double payoff = 0;
    for (int i = 1; i < this->nbDates_ + 1; i++){
        payoff += pnl_mat_get(path, i, 1);

    }
    payoff /= this->nbDates_;
    payoff -= pnl_mat_get(path, path->m-1, 0);
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)) * payoff;
    }
    return 0.;

}