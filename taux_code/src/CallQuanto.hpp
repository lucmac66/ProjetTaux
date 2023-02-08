#include "Option.hpp"
#include "math.h"

class CallQuanto: public Option{
    public :
    double strike_;

    CallQuanto(double T, double strike, double domesticRate, int year);
    double payoff(const PnlMat *path, double t);
};