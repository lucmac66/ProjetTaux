#include "Currency.hpp"

Currency::Currency(string id, double domesticRate, double foreignRate, PnlVect *corr, double sigma){
    this->id_ = id;
    this->domesticRate_ = domesticRate;
    this->foreignRate_ = foreignRate;
    pnl_vect_mult_scalar(corr, sigma);
    this->sigma_ = corr;
    this->drift_ = this->domesticRate_ - this->foreignRate_ - pnl_vect_norm_two(this->sigma_)/ 2;
}