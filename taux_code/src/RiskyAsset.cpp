#include "RiskyAsset.hpp"


RiskyAsset::RiskyAsset(Currency *currency, PnlVect* corr, double sigma){
    this->sigma_ = pnl_vect_new();
    pnl_vect_clone(sigma_, corr);
    pnl_vect_mult_scalar(this->sigma_, sigma);
    pnl_vect_plus_vect(this->sigma_, currency->sigma_);
    double norm = pnl_vect_norm_two(this->sigma_);
    this->drift_ = currency->domesticRate_ - norm * norm / 2;
};