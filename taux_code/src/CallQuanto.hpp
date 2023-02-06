#include "Option.hpp"
#include "math.h"

class CallQuanto: public Option{
    public :
    double strike_;

    CallQuanto(double T, int size, double strike, double domesticRate);
    double payoff(const PnlMat *path, double t);
};