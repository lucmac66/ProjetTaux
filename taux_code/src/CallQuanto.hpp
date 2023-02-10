#include "Option.hpp"
#include "math.h"

class CallQuanto: public Option{
    public :
    double strike_; /*! strike de l'option */

    /**
     * Constructeur de l'option
     *
     * @param T maturité
     * @param strike strike
     * @param domesticRate taux d'intérêt domestique
     * @param year nombre de jours dans l'année
     * @param sizemarket taille des marchés
     */
    CallQuanto(double T, double strike, double domesticRate, int year, vector<int> sizemarket);

    /**
     * Calcule le payoff
     *
     * @param path matrice des trajectoires simulées
     * @param t temps de calcul du payoff
     * @return payoff de l'option
     */
    double payoff(const PnlMat *path, double t);
};