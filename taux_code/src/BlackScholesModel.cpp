#include "BlackScholesModel.hpp"

BlackScholesModel::BlackScholesModel(PnlVect* importantDates, vector<RiskyAsset*> assets, vector<Currency*> currencies, int year){
    assets_ = assets;
    currencies_ = currencies;
    importantDates_ = importantDates;
    year_ = year;
}

void BlackScholesModel::asset(PnlMat* path, const PnlMat *past, double t, PnlRng* rng){

    int index = 0;

    for (int i = 0; i < importantDates_-> size; i++){
        if(t > pnl_vect_get(importantDates_, index)){
            index++;
        }
        else{
            break;
        }
    }
    ///Génération du vecteur G
    PnlVect* G = pnl_vect_new();

    ///Initialisation de la partie déterministe de path
    if (t == pnl_vect_get(importantDates_, index)){
        pnl_mat_set_subblock(path, past, 0, 0);
    }
    else{
        PnlMat *extract = pnl_mat_create_from_zero(past->m-1, past->n);
        pnl_mat_extract_subblock(extract, past, 0, past->m-1, 0, past->n);
        pnl_mat_set_subblock(path, extract, 0, 0);
        pnl_mat_free(&extract);
    }

    ///variables globales
    double dt;

    ///iteration sur le nombre de dates importantes de l'option
    for (int i = index; i < importantDates_->size; i++){
        ///remplissage du vecteur G

        pnl_vect_rng_normal(G, path->n, rng);

        if (i == 0){
            dt = (pnl_vect_get(importantDates_, index) - t)/this->year_;
            ///remplissage pour les Stildes
            for (int j = 0; j < assets_.size(); j++){
                double st = pnl_mat_get(past, past->m-1, j);
                // std::cout << " drift : " << assets_[j]->drift_ <<std::endl;
                // std::cout << " G : " << std::endl;
                // pnl_vect_print(G);
                // std::cout << " Sigma : " << std::endl;
                // pnl_vect_print(assets_[j] -> sigma_);
                // std::cout << " dt : " << dt <<std::endl;
                // std::cout << "argument :" << assets_[j]->drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j]->sigma_, G) << std::endl;
                // std::cout << "exp :" << exp(assets_[j]->drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j]->sigma_, G)) << std::endl;
                st *= exp(assets_[j]->drift_ * dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j]->sigma_, G));
                pnl_mat_set(path, i, j, st);
            }
            /// remplissage pour les Xi
            for (int j = assets_.size(); j < assets_.size() + currencies_.size(); j++){
                double st = pnl_mat_get(past, past->m-1, j);
                st *= exp(currencies_[j-assets_.size()]->drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(currencies_[j-assets_.size()]->sigma_, G));
                pnl_mat_set(path, i, j, st);
            }

        }
        else{
            dt = (pnl_vect_get(importantDates_, index) - pnl_vect_get(importantDates_, index-1))/this->year_;
            ///remplissage pour les Stildes
            for (int j = 0; j < assets_.size(); j++){
                double st = pnl_mat_get(path, i-1, j);
                st *= exp(assets_[j]->drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(assets_[j]->sigma_, G));
                pnl_mat_set(path, i, j, st);
            }
            /// remplissage pour les Xi
            for (int j = assets_.size(); j < assets_.size() + currencies_.size(); j++){
                double st = pnl_mat_get(path, i-1, j);
                st *= exp(currencies_[j]->drift_ *dt + sqrt(dt) * pnl_vect_scalar_prod(currencies_[j]->sigma_, G));
                pnl_mat_set(path, i, j, st);
            }
        }

        
    }
    pnl_vect_free(&G);
}

void BlackScholesModel::shiftAsset(PnlMat* path, const PnlMat* past, double epsilon, double t, int column){
    int index = 0;

    for (int i = 0; i < importantDates_-> size; i++){
        if(t > pnl_vect_get(importantDates_, index)){
            index++;
        }
        else{
            break;
        }
    }
    
    double st = pnl_mat_get(past, past->m-1, column);
    double st_shift = st + epsilon;
    double change = st_shift / st;

    for (int i = index; i < path->m; i++){
        double new_value = pnl_mat_get(path, i, column) * change;
        pnl_mat_set(path, i, column, new_value);
    }
}