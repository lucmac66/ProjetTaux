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
    double T_; ///maturité
    double strike_; ///strike apparaissant dans le payoff

    ForeignPerfBasket(double T, double strike, double domestricRate, int year, vector<int> sizemarket);
    double payoff(const PnlMat *path, double t);
};
