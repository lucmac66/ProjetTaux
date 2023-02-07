#include "BlackScholesModel.hpp"

BlackScholesModel::BlackScholesModel(int n, PnlVect* size, PnlVect* r, PnlMat* sigma, PnlVect* spot, vector<RiskyAsset> assets, vector<Currency> currencies){
    n_ = n;
    size_ = size;
    r_ = r;
    sigma_ = sigma;
    spot_ = spot;
    assets_ = assets;
    currencies_ = currency;
}

void BlackScholesModel::asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng){
    int n = (int) pnl_vect_sum(size_); 
    int n_max = (int) pnl_vect_max(size_);

    ///Génération du vecteur G
    PnlVect* G = pnl_vect_new();

    ///Initialisation de la première ligne de la matrice path
    pnl_mat_set_row(path, spot_, 0);

    ///variables globales
    double dt = T / nbTimeSteps; /// pas de temps qui apparait dans l'exponentielle

    ///iteration sur le nombre de dates
    for (int i = 1; i < nbTimeSteps; i++){
        ///remplissage du vecteur G
        pnl_vect_rng_normal(G, n+n_, rng);

        ///remplissage pour les Stildes
        for (int j = 0; j < assets_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(assets_[j].drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }
        /// remplissage pour les Xi
        for (int j = assets_.size; j < assets_.size + currencies_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(currencies_[j].drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(currencies_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }
    }
}

void BlackScholesModel::asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, PnlMat* past){
    int n = (int) pnl_vect_sum(size_); 
    int n_max = (int) pnl_vect_max(size_);

    ///on set la matrice past comme sous-bloc de path
    pnl_mat_set_subblock(path, past, 0, 0);

    ///Génération du vecteur G
    PnlVect* G = pnl_vect_new();


    ///variables globales
    double dt = T / nbTimeSteps; /// pas de temps qui apparait dans l'exponentielle
    int dernier_i = (int) nbTimeSteps*t/T;
    double first_dt = t - (dernier_i*T)/nbTimeSteps;
    if (t == dernier_i * dt){
        dernier_i++; // t est un tj de la subdivision
    }

    if (dernier_i < nbTimeSteps){
        pnl_vect_rng_normal(G, n+n_, rng);

        ///remplissage pour les Stildes
        for (int j = 0; j < assets_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(assets_[j].drift_ *first_dt + sqrt(first_dt) * pnl_vect_scalar_prod(assets_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }

        /// remplissage pour les Xi
        for (int j = assets_.size; j < assets_.size + currencies_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(currencies_[j].drift_ *first_dt + sqrt(first_dt) * pnl_vect_scalar_prod(currencies_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }
    }

    ///iteration sur le nombre de dates
    for (int i = dernier_i+1; i < nbTimeSteps; i++){
        ///remplissage du vecteur G
        pnl_vect_rng_normal(G, n+n_, rng);

        ///remplissage pour les Stildes
        for (int j = 0; j < assets_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(assets_[j].drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }
        /// remplissage pour les Xi
        for (int j = assets_.size; j < assets_.size + currencies_.size; j++){
            double st = pnl_mat_get(path, i-1, j);
            st *= exp(currencies_[j].drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(currencies_[j].sigma_, G));
            pnl_mat_set(path, i, j, st);
        }
    }

}



void BlackScholesModel::shiftAsset(PnlMat* path, PnlMat* past, double T, int nbTimeSteps, double epsilon, double t, int column){
    double st = pnl_mat_get(past, past->m-1, column);
    double st_shift = st + epsilon;
    double change = st_shift / st;

    double dt = T / nbTimeSteps;
    int row = (int) nbTimeSteps*t/T;
    double first_dt = t - (row*T)/nbTimeSteps;
    if (t == row * dt){
        row++; // t est un tj de la subdivision
    }

    for (int i = row; i < path->m; i++){
        double new_value = pnl_mat_get(path, i, column) * change;
        pnl_mat_set(path, i, column, new_value);
    }
}