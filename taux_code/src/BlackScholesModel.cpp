#include "BlackScholesModel.hpp"

BlackScholesModel::BlackScholesModel(int n, PnlVect* size, PnlVect* r, PnlMat* sigma, PnlVect* spot){
    n_ = n;
    size_ = size;
    r_ = r;
    sigma_ = sigma;
    spot_ = spot;
}

void BlackScholesModel::asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng){
    int n = (int) pnl_vect_sum(size_); 
    int n_max = (int) pnl_vect_max(size_);

    ///Génération du vecteur G
    PnlVect* G = pnl_vect_new();

    ///Initialisation de la première ligne de la matrice path
    pnl_mat_set_row(path, spot_, 0);

    ///Transformation de Cholesky de la matrice de correlation
    pnl_mat_chol(sigma_);

    ///variables globales
    double dt = T / nbTimeSteps; /// pas de temps qui apparait dans l'exponentielle

    ///iteration sur le nombre de dates
    for (int i = 1; i < nbTimeSteps; i++){
        ///remplissage du vecteur G
        pnl_vect_rng_normal(G, n+n_, rng);

        ///multiplication par la transformée de Cholesky pour avoir la bonne correlation
                
        /// marché actuel (0 pour domestique, i pour le marché i)
        int k = 0;
        ///colonne actuelle
        int col = 0;

        for (int j = 0; j < size_->size; j++){
            ///taille du marché en question
            int taille = (int) pnl_vect_get(size_, j);
            
            if (j == 0){
                simulDomestic(path, i, col, G, dt);
                col++;
            }

            else{
                simulForeign;
                col++;
            }

            k++;
        }

        ///simuler Xi

    }

}

void BlackScholesModel::simulDomestic(PnlMat* path, int i, int j, PnlVect* G, double dt){
    double st = pnl_mat_get(path, i-1, j);
    PnlVect* s = pnl_vect_new();
    pnl_mat_get_col(s, sigma_, j);
    double sigma_squared = pnl_vect_norm_two(s)/2;
    double sigma_Wt = pnl_vect_scalar_prod(s, G);
    st *= exp( (pnl_vect_get(r_, 0) - sigma_squared)*dt + sigma_Wt * sqrt(dt));
    pnl_mat_set(path, i, j, st);
}


void BlackScholesModel::simulForeign(PnlMat* path, int i, int j, PnlVect* G, double dt){
    double st = pnl_mat_get(path, i-1, j);
    PnlVect* s = pnl_vect_new();
    pnl_mat_get_col(s, sigma_, j);
    double sigma_squared = pnl_vect_norm_two(s)/2;
    double sigma_Wt = pnl_vect_scalar_prod(s, G);
    st *= exp( (pnl_vect_get(r_, 0) - sigma_squared)*dt + sigma_Wt * sqrt(dt));
    pnl_mat_set(path, i, j, st);
}

