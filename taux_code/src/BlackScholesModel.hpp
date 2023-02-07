#pragma once

#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "RiskyAsset.hpp"
#include "Currency.hpp"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

/// \brief Modèle de Black Scholes
class BlackScholesModel
{
  public:
    int n_; /// nombre de marchés étrangers
    vector<RiskyAsset> assets_;
    vector<Currency> currencies_;
    PnlVect* size_;  /// nombre d'actifs de chacun des marchés (dans l'ordre : dom, etrangers)
    PnlVect* r_;     /// taux d'intérêt de chacun des marchés
    PnlMat* sigma_; /// matrice de correlation
    PnlVect* spot_;  /// valeurs initiales des sous-jacents

    BlackScholesModel(int n, PnlVect* size, PnlVect* r, PnlMat* sigma, PnlVect* spot, std::vector<RiskyAsset> assets, std::vector<Currency> currencies);

    /**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
     */

    void asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng);

    void simulDomestic(PnlMat* path, int i, int j, PnlVect* G, double dt);
    void simulForeign(PnlMat* path, int i, int j, PnlVect* G, double dt, int k, int n);
    void simulRate(PnlMat* path, int i, int j, PnlVect* G, double dt, int l);

    void asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, PnlMat* past);

    void shiftAsset(PnlMat* path, PnlMat* past, double epsilon, double t, int column);

    /**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent
     * donnée jusqu'à l'instant t par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
     */
    void asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past);

    /**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path
     * dont la composante d a été shiftée par (1+h)
     * à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
     */
    void shiftAsset(PnlMat* path, PnlMat* past, double T, int nbTimeSteps, double epsilon, double t, int column);

};
