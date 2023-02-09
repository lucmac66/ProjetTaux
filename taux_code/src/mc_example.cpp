#include <iostream>
#include <fstream>
#include "json_reader.hpp"
#include "Portfolio.hpp"
#include "Hedger.hpp"
#include "MonteCarlo.hpp"
#include "BlackScholesModel.hpp"
#include "QuantoExchange.hpp"
#include "Currency.hpp"
#include "RiskyAsset.hpp"
#include "CallCurrency.hpp"
#include "CallQuanto.hpp"
#include "ForeignAsian.hpp"
#include "ForeignPerfBasket.hpp"
#include "Rebalancing.hpp"
#include "FixedRebalancing.hpp"
#include "GridRebalancing.hpp"
#include <map>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Wrong number of arguments. Exactly one argument is required" << std::endl;
        std::exit(0);
    }

    std::ifstream ifs(argv[1]);
    nlohmann::json jsonParams = nlohmann::json::parse(ifs);

    int assetNb = jsonParams.at("Assets").size();
    int currNb = jsonParams.at("Currencies").size();
    PnlMat *correlation;
    
    jsonParams.at("Correlations").get_to(correlation);
    pnl_mat_chol(correlation);

    pnl_mat_print(correlation);

    map<string, int> mapping;
    map<string, int> marketsize;
    vector<Currency*> currencies;
    std::cout << "-- currencies" << std::endl;

    std::string domesticCurrencyId;
    jsonParams.at("DomesticCurrencyId").get_to(domesticCurrencyId);
    double domesticRate = 0;

    int market = 0;

    auto jsonCurrencies = jsonParams.at("Currencies");
    for (auto jsonCurrency : jsonCurrencies) {
        string currencyId = jsonCurrency.at("id").get<std::string>();
        if (currencyId == domesticCurrencyId){
            domesticRate = jsonCurrency.at("InterestRate").get<double>();
            double foreignRate = jsonCurrency.at("InterestRate").get<double>();
            double sigma = jsonCurrency.at("Volatility").get<double>();
            currencies.push_back(new Currency(currencyId, domesticRate, foreignRate, pnl_vect_create_from_zero(correlation->m), sigma));
            mapping[currencyId] = market;
            marketsize[currencyId] = 0;
            market++;
        }
        else{
            PnlVect* corr = pnl_vect_create_from_zero(correlation->m);
            double foreignRate = jsonCurrency.at("InterestRate").get<double>();
            pnl_mat_get_col(corr, correlation, market + assetNb - 1);
            double sigma = jsonCurrency.at("Volatility").get<double>();
            currencies.push_back(new Currency(currencyId, domesticRate, foreignRate, corr, sigma));
            if (mapping.find(currencyId) == mapping.end()){
                mapping[currencyId] = market;
                marketsize[currencyId] = 0;
                market++;
            }
        }
    }
        
    vector<RiskyAsset*> assets;
    std::cout << "-- assets" << std::endl;
    int col = 0;
    auto jsonAssets = jsonParams.at("Assets");
    for (auto jsonAsset : jsonAssets) {
        PnlVect* corr = pnl_vect_create_from_zero(correlation->m);
        string id = jsonAsset.at("CurrencyId").get<std::string>();
        double sigma = jsonAsset.at("Volatility").get<double>();
        pnl_mat_get_col(corr, correlation, col);
        RiskyAsset *asset = new RiskyAsset(currencies[mapping[id]], corr, sigma);
        assets.push_back(asset);
        marketsize[id] += 1;
        col++;

    }
    currencies.erase(currencies.begin());

    vector<int> sizemarket;
    map<string, int>::iterator itr;
    for(itr=marketsize.begin();itr!=marketsize.end();itr++)
    {
        sizemarket.push_back(itr->second);
    }

    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    int maturity = jsonParams.at("Option").at("MaturityInDays").get<int>();
    std::string label = jsonParams.at("Option").at("Type").get<std::string>();
    Option *opt;
    if(label == "call_currency"){
        double strike = jsonParams.at("Option").at("Strike").get<int>();
        opt = new CallCurrency(maturity, strike, domesticRate, currencies[0]->foreignRate_, numberOfDaysPerYear, sizemarket);
    }else{
        if(label == "call_quanto"){
            double strike = jsonParams.at("Option").at("Strike").get<int>();
            opt = new CallQuanto(maturity, strike, domesticRate, numberOfDaysPerYear, sizemarket);
        }
        else{
            if(label == "foreign_asian"){
                opt = new ForeignAsian(maturity, domesticRate, numberOfDaysPerYear, sizemarket);
            }
            else{
                if(label == "quanto_exchange"){
                    double strike = jsonParams.at("Option").at("Strike").get<int>();
                    opt = new QuantoExchange(maturity, strike, domesticRate, numberOfDaysPerYear, sizemarket);
                }
                else{
                    double strike = jsonParams.at("Option").at("Strike").get<int>();
                    
                    opt = new ForeignPerfBasket(maturity, strike, domesticRate, numberOfDaysPerYear, sizemarket);
                }
            }
        }
    }

    PnlVect * importantDates;
    jsonParams.at("Option").at("FixingDatesInDays").at("DatesInDays").get_to(importantDates);



    int nbSample;
    jsonParams.at("SampleNb").get_to(nbSample);

    Portfolio *portfolio = new Portfolio(assetNb+currNb-1, numberOfDaysPerYear, domesticRate);
    BlackScholesModel *bs = new BlackScholesModel(importantDates, assets, currencies, numberOfDaysPerYear);


    //random Initialisation
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, time(NULL));

    std::cout << nbSample << std::endl;
    MonteCarlo *mc = new MonteCarlo(bs, opt, rng, nbSample);

    string type = jsonParams.at("PortfolioRebalancingOracleDescription").at("Type").get<std::string>();
    Rebalancing *rebalancing;
    if(type == "Fixed"){
        rebalancing = new FixedRebalancing(jsonParams.at("PortfolioRebalancingOracleDescription").at("Period").get<int>());
    }
    else{
        PnlVect * dates;
        jsonParams.at("PortfolioRebalancingOracleDescription").at("DatesInDays").get_to(dates);
        rebalancing = new GridRebalancing(dates);
    }

    Hedger *hedger = new Hedger(portfolio, argv[2], mc, rebalancing, sizemarket);
    hedger->RebalanceAll();

    pnl_mat_free(&correlation);
    std::exit(0);
}