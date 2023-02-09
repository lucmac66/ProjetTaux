#include "Hedger.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>

using namespace std;

Hedger::Hedger(Portfolio *portfolio, string csvDocName, MonteCarlo *mc, Rebalancing *rebalancingTool, vector<int> marketsize){
    this->portfolio_ = portfolio;
    this->marketData_ = ExtractCsv(csvDocName, marketsize);
    this->mc_ = mc;
    this->rebalancingTool_ = rebalancingTool;
}

void Hedger::RebalanceAll(){
    PnlMat * past = ExtractMarketData(0);
    RebalanceOnce(0, past);
    pnl_mat_free(&past);
    for(int jour = 1; jour < this->marketData_->m; jour++){
        std::cout << " " << std::endl;
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
    PnlVect *deltas = pnl_vect_create(this->portfolio_->quantity->size);
    PnlVect *stdDeltas = pnl_vect_create(deltas->size);
    this->mc_->priceAndDeltas(marketData, date,prix, std_dev, deltas, stdDeltas, 0.1);
    std::cout << "---------------" << std::endl;
    std::cout << "date :" << date << std::endl;
    std::cout << "prix :" << prix << std::endl;
    std::cout << "std prix :" << std_dev << std::endl;
    std::cout << "deltas :" << std::endl;
    pnl_vect_print(deltas);
    std::cout << "std deltas :" << std::endl;
    pnl_vect_print(stdDeltas);
    if (date == 0){
        this->portfolio_->ChangeAllQuantities(marketData, deltas, date, prix);
    }
    else{
        this->portfolio_->ChangeAllQuantities(marketData, deltas, date);
    }
    
    std::cout << "valeur portefeuille :" << this->portfolio_->value << std::endl;   
    pnl_vect_free(&deltas);
    pnl_vect_free(&stdDeltas);     
}

PnlMat *Hedger::ExtractMarketData(int date){
    int nbDates = 0;
    for(int i = 0; i < this->mc_->mod_->importantDates_->size; i++){
        if (date > pnl_vect_get(this->mc_->mod_->importantDates_, i)){
            nbDates++;
        }
        else{
            break;
        }
    }

    if(pnl_vect_get(this->mc_->mod_->importantDates_, nbDates) != date){
        nbDates++;
    }

    // Creation du market Data

    PnlMat * past = pnl_mat_create_from_zero(nbDates, this->marketData_->n);
    PnlVect * row = pnl_vect_create_from_zero(this->marketData_->n);

    // Extraction

    for(int i = 0; i < nbDates - 1; i++){
        pnl_mat_get_row(row, this->marketData_, pnl_vect_get(this->mc_->mod_->importantDates_, i));
        pnl_mat_set_row(past, row, i);
    }

    pnl_mat_get_row(row, this->marketData_, date);
    pnl_mat_set_row(past, row, nbDates - 1);

    pnl_vect_free(&row);

    return past;

}


PnlMat *Hedger::ExtractCsv(string name, vector<int> marketsize){

    // Parser le csv

    std::ifstream  data(name);
    std::string line;
    int nbLine = 0;
    int nbCol = 0;

    std::vector<std::vector<double> > parsedCsv;
    while(std::getline(data,line))
    {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<double> parsedRow;
        while(std::getline(lineStream,cell,','))
        {
            parsedRow.push_back(stod(cell));

            if(nbLine == 0){
                nbCol++;
            }
        }

        parsedCsv.push_back(parsedRow);
        nbLine++;
    }

    std::cout << nbLine << std::endl;
    PnlMat *marketData = pnl_mat_create_from_zero(nbLine, nbCol);

    for(int i = 0; i < nbLine; i++){
        for(int j = 0; j < nbCol; j++){
            pnl_mat_set(marketData, i, j, parsedCsv[i][j]);
        }
    }

    int nbAsset = std::accumulate(marketsize.begin(), marketsize.end(), 0);

    for(int i = 0; i < marketData->m; i++){
        int col = marketsize[0];
        for(int k = 1; k < marketsize.size(); k++){
            for(int j = col; j < col + marketsize[k]; j++){
                pnl_mat_set(marketData, i, j, pnl_mat_get(marketData, i, j) * pnl_mat_get(marketData, i, nbAsset + k - 1) );
            }
        }
        col += marketsize[i];
    }
    
    return marketData;
}