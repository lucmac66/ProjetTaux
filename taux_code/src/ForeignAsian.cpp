#include "ForeignAsian.hpp"

ForeignAsian::ForeignAsian(double T, double domestricRate, int year, vector<int> sizemarket){
    this->T_ = T;
    this->domesticRate_ = domestricRate;
    this->year_ = year;
    this->sizemarket_ = sizemarket; 
}

double ForeignAsian::payoff(const PnlMat* path, double t){
    double payoff = 0;
    ///calcul de la somme des Xi.Si
    for (int i = 0; i < path->m; i++){
        payoff += pnl_mat_get(path, i, 1);
    }
    ///on divise par le nombre d'observations pour avoir la moyenne arithmétique
    payoff /= (path->m);
    payoff -= pnl_mat_get(path, path->m-1, 0);

    ///renvoie le max entre 0 et le payoff
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)/ this->year_) * payoff;
    }
    return 0.;

}