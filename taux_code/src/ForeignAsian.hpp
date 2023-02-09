#include "Option.hpp"
#include "math.h"

class ForeignAsian: public Option{
    public :

    ForeignAsian(double T, double domesticRate, int year, vector<int> sizemarket);
    double payoff(const PnlMat *path, double t);
};