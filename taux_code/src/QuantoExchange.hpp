#include "Option.hpp"
#include "math.h"

class QuantoExchange: public Option{
    public :
    double strike_; /*! strike de l'option */

    /**
     * Constructeur de la classe QuantoExchange
     *
     * @param T
     * @param strike
     * @param domesticRate
     * @param year
     * @param sizemarket
     */
    QuantoExchange(double T, double strike, double domesticRate, int year, vector<int> sizemarket);

    /**
     * Calcule le payoff de l'option à la date t
     *
     * @param path matrice de la trajectoire simulée
     * @param t date de calcul du payoff
     * @return le payoff de l'option à la date t
     */
    double payoff(const PnlMat *path, double t);
};