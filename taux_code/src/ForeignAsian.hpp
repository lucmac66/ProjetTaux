#include "Option.hpp"
#include "math.h"

class ForeignAsian: public Option{
    public :

    ForeignAsian(double T, double domesticRate, int year);
    double payoff(const PnlMat *path, double t);
};