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

    // ///Transformation de Cholesky de la matrice de correlation
    // PnlMat* sigma_correl = pnl_mat_new();
    // pnl_mat_clone(sigma_correl, sigma_);
    // pnl_mat_chol(sigma_correl);

    ///variables globales
    double dt = T / nbTimeSteps; /// pas de temps qui apparait dans l'exponentielle

    ///iteration sur le nombre de dates
    for (int i = 1; i < nbTimeSteps; i++){
        ///remplissage du vecteur G
        pnl_vect_rng_normal(G, n+n_, rng);

        // ///multiplication par la transformée de Cholesky pour avoir la bonne correlation
        // PnlVect *Gtilde = pnl_mat_mult_vect(sigma_correl, G);
                
        // ///colonne actuelle
        // int col = 0;

        for (int j = 0; j < )

        // for (int j = 0; j < size_->size; j++){
        //     ///taille du marché en question
        //     int taille = (int) pnl_vect_get(size_, j);
            
        //     if (j == 0){
        //         for (int m = 0; m < taille; m++){
        //             simulDomestic(path, i, col, Gtilde, dt);
        //             col++;
        //         }
        //     }

        //     else{
        //         for (int m = 0; m < taille; m++){
        //             simulForeign(path, i, col, Gtilde, dt, j-1, n);
        //             col++;
        //         }
        //     }
        // }

        // ///simuler Xi
        // for (int l = 0; l < n_; l++){
        //     simulRate(path, i, n+l, Gtilde, dt, l);
        // }
    }

}

void BlackScholesModel::simulDomestic(PnlMat* path, int i, int j, PnlVect* G, double dt){
    double st = pnl_mat_get(path, i-1, j);
    PnlVect* s = pnl_vect_new();
    pnl_mat_get_col(s, sigma_, j);
    double sigma_squared = pnl_vect_norm_two(s)/2;
    st *= exp( (pnl_vect_get(r_, 0) - sigma_squared)*dt + pnl_vect_get(G, j) * sqrt(dt));
    pnl_mat_set(path, i, j, st);
}


void BlackScholesModel::simulForeign(PnlMat* path, int i, int j, PnlVect* G, double dt, int k, int n){
    double st = pnl_mat_get(path, i-1, j);
    PnlVect* s = pnl_vect_new(); ///sigma Si
    PnlVect* s1 = pnl_vect_new(); ///sigma Xi
    pnl_mat_get_col(s, sigma_, j);
    pnl_mat_get_col(s1, sigma_, n+k);
    pnl_vect_plus_vect(s, s1); ///somme des sigma
    double sigma_squared = pnl_vect_norm_two(s)/2;
    st *= exp( (pnl_vect_get(r_, 0) - sigma_squared)*dt + pnl_vect_get(G, j) * sqrt(dt));
    pnl_mat_set(path, i, j, st);
}

void BlackScholesModel::simulRate(PnlMat* path, int i, int j, PnlVect* G, double dt, int l){
    double st = pnl_mat_get(path, i-1, j);
    PnlVect* s = pnl_vect_new();
    pnl_mat_get_col(s, sigma_, j);
    double sigma_squared = pnl_vect_norm_two(s)/2;
    double sigma_Wt = pnl_vect_scalar_prod(s, G);
    st *= exp( (pnl_vect_get(r_, 0) - pnl_vect_get(r_, l) - sigma_squared)*dt + pnl_vect_get(G, j) * sqrt(dt));
    pnl_mat_set(path, i, j, st);
}




void BlackScholesModel::asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, PnlMat* past){
    int n = (int) pnl_vect_sum(size_); 
    int n_max = (int) pnl_vect_max(size_);

    ///on set la matrice past comme sous-bloc de path
    pnl_mat_set_subblock(path, past, 0, 0);

    ///Génération du vecteur G
    PnlVect* G = pnl_vect_new();

    ///Transformation de Cholesky de la matrice de correlation
    PnlMat* sigma_correl = pnl_mat_new();
    pnl_mat_clone(sigma_correl, sigma_);
    pnl_mat_chol(sigma_correl);

    ///variables globales
    double dt = T / nbTimeSteps; /// pas de temps qui apparait dans l'exponentielle
    int dernier_i = (int) nbTimeSteps*t/T;
    double first_dt = t - (dernier_i*T)/nbTimeSteps;
    if (t == dernier_i * dt){
        dernier_i++; // t est un tj de la subdivision
    }

    if (dernier_i < nbTimeSteps){
            pnl_vect_rng_normal(G, n+n_, rng);
            PnlVect *Gtilde = pnl_mat_mult_vect(sigma_correl, G);

            ///traitement des assets domestiques
            int colonne = 0;
            for (int i = 0; i < pnl_vect_get(size_, 0); i++){
                simulDomestic(path, dernier_i, colonne, Gtilde, first_dt);
                colonne++;
            }

            ///traitement des assets étrangers
            for (int j = 1; j < size_->size; j++){
                ///taille du marché en question
                int taille = (int) pnl_vect_get(size_, j);

                for (int m = 0; m < taille; m++){
                    simulForeign(path, dernier_i, colonne, Gtilde, first_dt, j-1, n);
                    colonne++;
                    }
            }int dernier_i = (int) nbTimeSteps*t/T;
    double first_dt = t - (dernier_i*T)/nbTimeSteps;
    if (t == dernier_i * dt){
        dernier_i++; // t est un tj de la subdivision
    }

            ///traitement des taux de change
            for (int l = 0; l < n_; l++){
                simulRate(path, dernier_i, n+l, Gtilde, first_dt, l);
            }
    }

    ///iteration sur le nombre de dates
    for (int i = dernier_i+1; i < nbTimeSteps; i++){
        ///remplissage du vecteur G
        pnl_vect_rng_normal(G, n+n_, rng);

        ///multiplication par la transformée de Cholesky pour avoir la bonne correlation
        PnlVect *Gtilde = pnl_mat_mult_vect(sigma_correl, G);
                
        ///colonne actuelle
        int col = 0;

        for (int j = 0; j < size_->size; j++){
            ///taille du marché en question
            int taille = (int) pnl_vect_get(size_, j);
            
            if (j == 0){
                for (int m = 0; m < taille; m++){
                    simulDomestic(path, i, col, Gtilde, dt);
                    col++;
                }
            }

            else{
                for (int m = 0; m < taille; m++){
                    simulForeign(path, i, col, Gtilde, dt, j-1, n);
                    col++;
                }
            }
        }

        ///simuler Xi
        for (int l = 0; l < n_; l++){
            simulRate(path, i, n+l, Gtilde, dt, l);
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