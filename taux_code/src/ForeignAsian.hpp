#include "Option.hpp"
#include "math.h"

class ForeignAsian: public Option{
    public :
    int nbDates_;

    ForeignAsian(double T, int size, double domesticRate, int nbDates);
    double payoff(const PnlMat *path, double t);
};