#include "Currency.hpp"

Currency::Currency(double domesticRate, double foreignRate, PnlVect *sigma){
    this->domesticRate_ = domesticRate;
    this->foreignRate_ = foreignRate;
    this->sigma_ = sigma;
    this->drift_ = this->domesticRate_ - this->foreignRate_ - pnl_vect_norm_two(this->sigma_)/ 2;
}