#include "ForeignPerfBasket.hpp"

ForeignPerfBasket::ForeignPerfBasket(double T, double strike, double domestricRate, int year, vector<int> sizemarket){
    this->T_ = T;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
    this->year_ = year;
    this->sizemarket_ = sizemarket; 
}

double ForeignPerfBasket::payoff(const PnlMat *path, double t){
    double payoff = 0;
    int imax;
    double ratio = 0;
    int col = sizemarket_[0];
    int nbAsset = std::accumulate(sizemarket_.begin(), sizemarket_.end(), 0);
    for(int i = 1; i < sizemarket_.size(); i++){

        double somme_foreign_t1 = 0;
        double somme_foreign_t2 = 0;

        for(int j = col; j < col + sizemarket_[i]; j++){
            somme_foreign_t1 += pnl_mat_get(path, 0, j) / pnl_mat_get(path, 0, nbAsset + i - 1) ;
            somme_foreign_t2 += pnl_mat_get(path, 1, j) / pnl_mat_get(path, 1, nbAsset + i - 1);
        }

        if(somme_foreign_t2 / somme_foreign_t1 > ratio){
            imax = i;
            ratio = somme_foreign_t2 / somme_foreign_t1;
        }
        col += sizemarket_[i];
    }

    double moyenne_dom = 0;
    double moyenne_foreign = 0;

    col = 0;
    for(int i = 0; i < sizemarket_.size(); i++){
        if(i == 0){
            for(int j = col; j < col + sizemarket_[0]; j++){
                moyenne_dom += pnl_mat_get(path, path->m-1, j);
            }
            moyenne_dom /= sizemarket_[0];
        }
        else{
            if(i == imax){
                for(int j = col; j < col + sizemarket_[imax]; j++){
                    moyenne_foreign += pnl_mat_get(path, path->m-1, j);
                }
                moyenne_foreign /= sizemarket_[imax];

            }
        }
        col += sizemarket_[i];
    }

    if(moyenne_foreign > moyenne_dom + strike_){
        payoff = moyenne_foreign - moyenne_dom - strike_;
        return exp(this->domesticRate_ * (t - this->T_)/ this->year_) * payoff;
    }
    else{
        return 0;
    }

}

