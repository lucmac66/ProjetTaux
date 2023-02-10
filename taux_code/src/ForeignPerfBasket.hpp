#include "Option.hpp"
#include "math.h"
#include "Currency.hpp"
#include "RiskyAsset.hpp"
#include <map>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include <numeric>

class ForeignPerfBasket: public Option{
  public :
    double T_;      /*! maturité */
    double strike_; /*! strike apparaissant dans le payoff */

    /**
     * Constructeur de la classe
     *
     * @param T maturité
     * @param strike strike
     * @param domestricRate taux d'intérêt domestique
     * @param year nombre de jours dans l'année
     * @param sizemarket taille des marchés
     */
    ForeignPerfBasket(double T, double strike, double domestricRate, int year, vector<int> sizemarket);

    /**
     * Fonction qui calcule le payoff de l'option
     *
     * @param path matrice des observations simulées
     * @param t temps auquel on calcule le payoff
     * @return le payoff de l'option
     */
    double payoff(const PnlMat *path, double t);
};
