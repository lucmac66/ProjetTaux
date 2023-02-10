#include "ForeignPerfBasket.hpp"

ForeignPerfBasket::ForeignPerfBasket(double T, double strike, double domestricRate, int year, vector<int> sizemarket){
    this->T_ = T;
    this->strike_ = strike;
    this->domesticRate_ = domestricRate;
    this->year_ = year;
    this->sizemarket_ = sizemarket; 
}

double ForeignPerfBasket::payoff(const PnlMat *path, double t){
    ///initialisation du payoff à 0
    double payoff = 0;

    ///variable représentant le marché qui réalise le maximum de rentabilité entre t1 et t2
    int imax;

    ///variable représentant le maximum de rentabilité entre t1 et t2
    double ratio = 0;

    ///intialisation de la colonne à la première colonne du premier marché étranger
    int col = sizemarket_[0];

    ///nombre d'actifs risqués au total (domestiques et étrangers)
    int nbAsset = std::accumulate(sizemarket_.begin(), sizemarket_.end(), 0);

    ///on boucle sur les marchés étrangers pour déterminer celui qui réalise le maximum de rentabilité
    for(int i = 1; i < sizemarket_.size(); i++){

        ///sommes des valeurs des actifs aux temps t1 et t2
        double somme_foreign_t1 = 0;
        double somme_foreign_t2 = 0;

        ///on incrémente les sommes par Si/Xi, car les Si de la matrice path sont sous probabilité risque neutre domestique
        for(int j = col; j < col + sizemarket_[i]; j++){
            somme_foreign_t1 += pnl_mat_get(path, 0, j) / pnl_mat_get(path, 0, nbAsset + i - 1) ;
            somme_foreign_t2 += pnl_mat_get(path, 1, j) / pnl_mat_get(path, 1, nbAsset + i - 1);
        }

        ///si la rentabilité est supérieure au maximum observé, le marché i devient la référence
        if(somme_foreign_t2 / somme_foreign_t1 > ratio){
            imax = i;
            ratio = somme_foreign_t2 / somme_foreign_t1;
        }
        ///on incrémente la colonne par la taille du marché i, pour passer au marché étranger suivant
        col += sizemarket_[i];
    }

    ///initialisation des variables de moyennes arithmétiques des actifs risqués domestiques et ceux du marché imax
    double moyenne_dom = 0;
    double moyenne_foreign = 0;

    ///calcul de la moyenne arithmétique
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

    ///calcul du payoff
    if(moyenne_foreign > moyenne_dom + strike_){
        payoff = moyenne_foreign - moyenne_dom - strike_;
        return exp(this->domesticRate_ * (t - this->T_)/ this->year_) * payoff;
    }
    else{
        return 0;
    }

}

