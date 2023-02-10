#include "Option.hpp"
#include "math.h"

class CallCurrency: public Option{
    public :
    double strike_;      /*! strike de l'option */
    double foreignRate_; /*! taux d'intérêt de la devise étrangère */

    /**
     * Constructeur de la classe
     *
     * @param T maturité
     * @param strike strike
     * @param domesticRate taux d'intérêt domestique
     * @param foreignRate taux d'intérêt étranger
     * @param year nombre de jours dans l'année
     * @param sizemarket taille des marchés
     */
    CallCurrency(double T, double strike, double domesticRate, double foreignRate, int year, vector<int> sizemarket);

    /**
     * Calcul du payoff
     *
     * @param path matrice de trajectoire simulée
     * @param t temps de calcul du payoff
     * @return le payoff de l'option
     */
    double payoff(const PnlMat *path, double t);
};