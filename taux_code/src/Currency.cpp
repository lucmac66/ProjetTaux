#include "Currency.hpp"

Currency::Currency(string id, double domesticRate, double foreignRate, PnlVect *sigma){
    this->id_ = id;
    this->domesticRate_ = domesticRate;
    this->foreignRate_ = foreignRate;
    this->sigma_ = sigma;
    this->drift_ = this->domesticRate_ - this->foreignRate_ - pnl_vect_norm_two(this->sigma_)/ 2;
}