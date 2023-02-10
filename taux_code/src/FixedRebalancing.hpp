#include "pnl/pnl_vector.h"
#include "pnl/pnl_random.h"
#include "Rebalancing.hpp"

class FixedRebalancing : public Rebalancing
{
    public:
        int period; /*! période de rebalancement */

        /**
         * Constructeur de la classe
         *
         * @param period période de rebalancement
         */
        FixedRebalancing(int period);

        /**
         * Fonction qui détermine si la date en question est une date de rebalancement
         *
         * @param dateT date passée en paramètre
         * @return renvoie true si la date est une date de rebalancement, false sinon
         */
        bool IsRebalanceDate(int dateT) override;

};
