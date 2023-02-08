#include "Option.hpp"
#include "math.h"

class QuantoExchange: public Option{
    public :
    double strike_;

    QuantoExchange(double T, double strike, double domesticRate, int year);
    double payoff(const PnlMat *path, double t);
};