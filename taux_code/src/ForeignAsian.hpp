#include "Option.hpp"
#include "math.h"

class ForeignAsian: public Option{
    public :

    /**
     * Constructeur de la classe
     *
     * @param T maturité
     * @param domesticRate taux d'intérêt domestique
     * @param year nombre de jours dans l'année
     * @param sizemarket taille des marchés
     */
    ForeignAsian(double T, double domesticRate, int year, vector<int> sizemarket);

    /**
     * Fonction qui calcule le payoff de l'option
     *
     * @param path matrice des trajectoire simulées
     * @param t temps auquel on calcule le payoff
     * @return le payoff de l'option
     */
    double payoff(const PnlMat *path, double t);
};