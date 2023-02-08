#include "RiskyAsset.hpp"
#include <iostream>

RiskyAsset::RiskyAsset(Currency *currency, PnlVect* sigma){
    this->currency_ = currency;

    this->sigma_ = pnl_vect_copy(sigma);
    pnl_vect_plus_vect(this->sigma_, this->currency_->sigma_);
    this->drift_ = currency->domesticRate_ - pnl_vect_norm_two(this->sigma_) / 2;
};