#include "CallQuanto.hpp"

CallQuanto::CallQuanto(double T, double strike, double domestricRate, int year, vector<int> sizemarket){
    this->T_ = T;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
    this->year_ = year;
    this->sizemarket_ = sizemarket; 
}

double CallQuanto::payoff(const PnlMat* path, double t){
    ///calcul direct du payoff en utilisant la formule associée
    double payoff = pnl_mat_get(path, path->m-1, 0) - this->strike_;
    ///renvoie le max entre 0 et le payoff
    if (payoff > 0){
        return exp(this->domesticRate_ * (t - this->T_)/year_) * payoff;
    }
    return 0.;

}