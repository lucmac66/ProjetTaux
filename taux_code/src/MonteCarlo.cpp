#include "MonteCarlo.hpp"


MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, long nbSamples){
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    nbSamples_ = nbSamples;
}


void MonteCarlo::priceAndDeltas(const PnlMat* past, double t, double& prix, double& std_dev, PnlVect *deltas, PnlVect *stdDeltas, double epsilon){
    ///création de la matrice path
    PnlMat* path = pnl_mat_create(this->mod_->importantDates_->size, past->n);
    PnlMat* pathEpsilonP = pnl_mat_create(this->mod_->importantDates_->size, past->n);
    PnlMat* pathEpsilonN = pnl_mat_create(this->mod_->importantDates_->size, past->n);

    ///initialisation price et std dev

    prix = 0;
    std_dev = 0;
    double payoff;
    double deltapayoff;

    ///tirages Monte Carlo
    for (int i = 0; i < nbSamples_; i++){
        mod_->asset(path, past, t,  rng_);
        payoff = opt_->payoff(path, t);
        prix += payoff;
        std_dev += payoff * payoff;
        for (int j = 0; j < path->n; j++){
            pnl_mat_clone(pathEpsilonP, path);
            pnl_mat_clone(pathEpsilonN, path);
            mod_->shiftAsset(pathEpsilonP, past, epsilon, t, j);
            mod_->shiftAsset(pathEpsilonN, past, -epsilon, t, j);
            deltapayoff = opt_->payoff(pathEpsilonP, t) - opt_->payoff(pathEpsilonN, t);
            pnl_vect_set(deltas, j, pnl_vect_get(deltas, j) + deltapayoff);
            pnl_vect_set(stdDeltas, j, pnl_vect_get(stdDeltas, j) + deltapayoff*deltapayoff);           
        }

    }
    for (int j = 0; j < path->n; j++){
        pnl_vect_set(deltas, j, pnl_vect_get(deltas, j)/ (2*epsilon*nbSamples_));
        pnl_vect_set(stdDeltas, j, sqrt(abs(pnl_vect_get(stdDeltas, j)/ (2*epsilon*nbSamples_) - pnl_vect_get(deltas, j)*pnl_vect_get(deltas, j))/ nbSamples_));
    }
    
    prix /= nbSamples_;
    std_dev = sqrt(abs(std_dev/nbSamples_ - prix*prix)/nbSamples_);

    pnl_mat_free(&path);
    pnl_mat_free(&pathEpsilonP);
    pnl_mat_free(&pathEpsilonN);
}