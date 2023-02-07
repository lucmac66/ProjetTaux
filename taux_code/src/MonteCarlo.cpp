#include "MonteCarlo.hpp"


MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, double fdStep, long nbSamples){
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples_;
}


void MonteCarlo::priceAndDeltas(const PnlMat* past, double t, double& prix, double& std_dev, PnlVect *deltas, PnlVect *stdDeltas, int nbTimeSteps, double epsilon){
    ///création de la matrice path
    int n = (int) pnl_vect_sum(opt_->size_);
    PnlMat* path = pnl_mat_create(nbTimeSteps, n + opt_->n_);
    PnlMat* pathEpsilonP = pnl_mat_create(nbTimeSteps, n + opt_->n_);
    PnlMat* pathEpsilonN = pnl_mat_create(nbTimeSteps, n + opt_->n_);

    ///initialisation price et std dev
    prix = 0;
    std_dev = 0;
    double payoff;
    double deltapayoff;
    ///tirages Monte Carlo
    for (int i = 0; i < nbSamples_; i++){
        mod_->asset(path, t, this->opt_->T_, nbTimeSteps, rng_, past);
        payoff = opt_->payoff(path, t);
        prix += payoff;
        std_dev += payoff * payoff;
        for (int j = 0; j < n + opt_->n_; j++){
            pathEpsilonN = pnl_mat_copy(path);
            pathEpsilonP = pnl_mat_copy(path);
            mod_->shiftAsset(pathEpsilonP, past, this->opt_->T_, nbTimeSteps, epsilon, t, j);
            mod_->shiftAsset(pathEpsilonN, past, this->opt_->T_, nbTimeSteps, -epsilon, t, j);

            deltapayoff = opt_->payoff(pathEpsilonP, t) - opt_->payoff(pathEpsilonN, t);
            pnl_vect_set(deltas, j, pnl_vect_get(deltas, j) + deltapayoff);
            pnl_vect_set(stdDeltas, j, pnl_vect_get(stdDeltas, j) + deltapayoff);            
        }

    }
    for (int j = 0; j < n + opt_->n_; j++){
        pnl_vect_set(deltas, j, pnl_vect_get(deltas, j)/ (2*epsilon*nbSamples_));
        pnl_vect_set(stdDeltas, j, sqrt(abs(pnl_vect_get(stdDeltas, j)/ (2*epsilon*nbSamples_) - pnl_vect_get(deltas, j)*pnl_vect_get(deltas, j))));
    }

    prix /= nbSamples_;
    std_dev = sqrt(abs(std_dev/nbSamples_ - prix*prix));
}