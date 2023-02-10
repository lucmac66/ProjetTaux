#include "Hedger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <string>

using namespace std;

Hedger::Hedger(Portfolio *portfolio, char* csvDocName, MonteCarlo *mc, Rebalancing *rebalancingTool, vector<int> marketsize, double epsilon){
    this->portfolio_ = portfolio;
    this->marketData_ = ExtractCsv(csvDocName, marketsize);
    this->mc_ = mc;
    this->rebalancingTool_ = rebalancingTool;
    this->epsilon_ = epsilon;
}

void Hedger::RebalanceAll(){
    PnlMat * past = ExtractMarketData(0);
    RebalanceOnce(0, past);
    pnl_mat_free(&past);
    for(int jour = 1; jour < this->marketData_->m; jour++){
        if (rebalancingTool_->IsRebalanceDate(jour)){
            PnlMat * past = ExtractMarketData(jour);
            RebalanceOnce(jour, past);
            pnl_mat_free(&past);
        }
    } 
}

void Hedger::RebalanceOnce(int date, PnlMat* marketData){

    double prix = 0;
    double std_dev = 0;
    PnlVect *deltas = pnl_vect_create_from_zero(this->portfolio_->quantity->size);
    PnlVect *stdDeltas = pnl_vect_create_from_zero(deltas->size);
    
    this->mc_->priceAndDeltas(marketData, date,prix, std_dev, deltas, stdDeltas, this->epsilon_);
    
    if (date == 0){
        this->portfolio_->ChangeAllQuantities(marketData, deltas, date, prix);
    }
    else{
        this->portfolio_->ChangeAllQuantities(marketData, deltas, date);
    }

    Position position(date, prix, std_dev, pnl_vect_copy(deltas), pnl_vect_copy(stdDeltas), this->portfolio_->value);
    this->positions.push_back(position);
    // pnl_vect_free(&deltas);
    // pnl_vect_free(&stdDeltas);     
}

PnlMat *Hedger::ExtractMarketData(int date){
    int nbDates = 1;
    for(int i = 0; i < this->mc_->mod_->importantDates_->size; i++){
        int dateReference = pnl_vect_get(this->mc_->mod_->importantDates_, i);
        if (date > dateReference){
            nbDates++;
        }
        else{
            break;
        }
    }
    // Creation du market Data
    PnlMat * past = pnl_mat_create_from_zero(nbDates, this->marketData_->n);
    PnlVect * row = pnl_vect_create_from_zero(this->marketData_->n);

    // Extraction

    for(int i = 0; i < nbDates-1; i++){
        double date = pnl_vect_get(this->mc_->mod_->importantDates_, i);
        pnl_mat_get_row(row, this->marketData_, date);
        pnl_mat_set_row(past, row, i);
    }

    pnl_mat_get_row(row, this->marketData_, date);
    pnl_mat_set_row(past, row, nbDates - 1);

    pnl_vect_free(&row);
    return past;

}


PnlMat *Hedger::ExtractCsv(char* name, vector<int> marketsize){

    PnlMat *marketData = pnl_mat_create_from_file(name);

    int nbAsset = std::accumulate(marketsize.begin(), marketsize.end(), 0);

    for(int i = 0; i < marketData->m; i++){
        int col = marketsize[0];
        for(int k = 1; k < marketsize.size(); k++){
            for(int j = col; j < col + marketsize[k]; j++){
                pnl_mat_set(marketData, i, j, pnl_mat_get(marketData, i, j) * pnl_mat_get(marketData, i, nbAsset + k - 1));
            }
        }
        col += marketsize[i];
    }
    
    return marketData;
}