#include "Option.hpp"
#include "math.h"

class CallCurrency: public Option{
    public :
    double strike_;
    double foreignRate_;

    CallCurrency(double T, double strike, double domesticRate, double foreignRate, int year, vector<int> sizemarket);
    double payoff(const PnlMat *path, double t);
};