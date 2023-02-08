#include "Option.hpp"
#include "math.h"
#include "Currency.hpp"
#include "RiskyAsset.hpp"
#include <map>
#include "pnl/pnl_vector.hpp"
#include "pnl/pnl_matrix.hpp"

class ForeignPerfBasket: public Option{
  public :
    double T_; ///maturité
    double strike_; ///strike apparaissant dans le payoff
    double dates_ [3]; ///tableau contenant les 3 dates t1, t2 et t3
    int n_; ///nombre de marchés étrangers
    map<Currency, map<int, RiskyAsset>> foreignCurrencies_; ///dictionnaire contenant une monnaie comme index et une liste d'actifs risqués comme valeurs
    map<Currency, RiskyAsset> domesticCurrency_; ///dictionnaire contenant la liste d'actifs risqués comme valeur et la monnaie domestique comme clef
    int indices_ [n_]; ///liste des indices pour le payoff
    int sizes_ [n_+1]; ///liste du nombre d'actifs dans chacun des marchés (domestique + étrangers)

    ForeignPerfBasket(double T, int n, double strike, map<Currency, map<int, RiskyAsset>> foreignCurrencies, map<Currency, RiskyAsset> domesticCurrency, int dates [], int[] size);
    double payoff(const PnlMat *path);
};
