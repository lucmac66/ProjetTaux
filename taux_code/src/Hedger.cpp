#include "Hedger.hpp"

Hedger::Hedger(Portfolio portfolio, string csvDocName){
    this->portfolio = portfolio;
    this->marketData = pnl_mat_create_from_zero(1,1); // faut qu'on parse le fichier
}

void Hedger::RebalanceAll(){
}

void Hedger::RebalanceOnce(int date, PnlMat* marketData){
    
}