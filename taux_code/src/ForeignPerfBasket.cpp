#include "ForeignPerfBasket.hpp"

ForeignPerfBasket::ForeignPerfBasket(double T, int n, double strike, map<Currency, map<int, RiskyAsset>> foreignCurrencies, map<Currency, RiskyAsset> domesticCurrency, int dates [], int size){
    T_ = T;
    n_ = n;
    strike_ = strike;
    foreignCurrencies_ = foreignCurrencies;
    domesticCurrency_ = domesticCurrency;
    dates_ = dates;
    int indexes [n_] = { 0 };
    indices_ = indexes;
    sizes_ = size;
}

double
ForeignPerfBasket::payoff(const int* path){
    ///numéro de colonne
    int col = 0;

    ///on calcule la moyenne des actifs domestiques
    double moyenne_dom = 0;
    for (int j = 0; j < sizes_[0]; j++){
        moyenne_dom += pnl_mat_get(path, path->m - 1, j);
        col++;
    }
    moyenne_dom /= sizes_[0];

    ///on déclare le numéro de la devise qui réalise le max dans un entier
    int max_currency;
    double ratio = 0;

    ///on calcule les ratios et on attribue max_currency à la devise qui réalise le max
    for (int i = 1; i < sizes_.size(); i++){
        double somme_foreign_t1 = 0;
        double somme_foreign_t2 = 0;

        ///somme au temps t2
        for (int j = 0; j < sizes_[i]; j++){
            somme_foreign_t1 += pnl_mat_get(path, path->m - 1, col + j);
        }

        ///somme au temps t1
        for (int j = 0; j < sizes_[i]; j++){
            somme_foreign_t2 += pnl_mat_get(path, path->m - 1, col + j);
        }

        ///on incrémente la colonne
        col += sizes_[i];

        ///vérification de la condition de la max currency
        if (somme_foreign_t2/somme_foreign_t1 > ratio){
            max_currency = i;
        }
    }

    ///calcul de la moyenne arithmétique de la max currency en t
    double moyenne_for = 0;
    int colonne = 0; ///on recherche la colonne correspondante
    for int(j = 0; j<i; j++){
            colonne += sizes_[j];
        }
    for (int j = 0; j < sizes_[max_currency]; j++){
        moyenne_for += pnl_mat_get(path, path->m - 1, colonne + j);
    }
    moyenne_for /= sizes_[max_currency];

    ///calcul du payoff
    if (moyenne_for - moyenne_dom - strike_ > 0){
        return moyenne_for - moyenne_dom - strike_;
    }

    else{
        return 0;
    }

}
