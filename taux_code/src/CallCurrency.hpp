#include "Option.hpp"
#include "math.h"

class CallCurrency: public Option{
    public :
    double strike_;
    double foreignRate_;

    CallCurrency(double T, int size, double strike, double domesticRate, double foreignRate);
    double payoff(const PnlMat *path, double t);
};