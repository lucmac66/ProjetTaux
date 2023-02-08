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
    // if (argc > 0) {
    //      std::cerr << "Wrong number of arguments. Exactly one argument is required" << std::endl;
    //      std::exit(0);
    // }

    std::ifstream ifs(argv[1]);
    nlohmann::json jsonParams = nlohmann::json::parse(ifs);

    int assetNb = jsonParams.at("Assets").size();
    int currNb = jsonParams.at("Currencies").size();
    PnlMat *correlation;
    
    jsonParams.at("Correlations").get_to(correlation);
    pnl_mat_chol(correlation);

    map<string, int> mapping;
    vector<Currency*> currencies;
    std::cout << "-- currencies" << std::endl;

    std::string domesticCurrencyId;
    jsonParams.at("DomesticCurrencyId").get_to(domesticCurrencyId);
    double domesticRate = 0;

    int market = 0;
    PnlVect* sigma = pnl_vect_create_from_zero(correlation->m);

    auto jsonCurrencies = jsonParams.at("Currencies");
    for (auto jsonCurrency : jsonCurrencies) {
        string currencyId = jsonCurrency.at("id").get<std::string>();
        if (currencyId == domesticCurrencyId){
            domesticRate = jsonCurrency.at("InterestRate").get<double>();
            double foreignRate = jsonCurrency.at("InterestRate").get<double>();
            currencies.push_back(new Currency(domesticRate, foreignRate, pnl_vect_create_from_zero(correlation->m)));
            mapping[currencyId] = market;
            market++;
        }
        else{
            double foreignRate = jsonCurrency.at("InterestRate").get<double>();
            pnl_mat_get_col(sigma, correlation, market + assetNb - 1);
            currencies.push_back(new Currency(domesticRate, foreignRate, sigma));
            if (mapping.find(currencyId) == mapping.end()){
                mapping[currencyId] = market;
                market++;
            }
        }
    }
        
    vector<RiskyAsset*> assets;
    std::cout << "-- assets" << std::endl;
    int col = 0;
    auto jsonAssets = jsonParams.at("Assets");
    for (auto jsonAsset : jsonAssets) {
        string id = jsonAsset.at("CurrencyId").get<std::string>();
        pnl_mat_get_col(sigma, correlation, col);
        assets.push_back(new RiskyAsset(currencies[mapping[id]], sigma));

    }

    currencies.erase(currencies.begin());


    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    int maturity = jsonParams.at("Option").at("MaturityInDays").get<int>();
    std::string label = jsonParams.at("Option").at("Type").get<std::string>();
    Option *opt;
    if(label == "call_currency"){
        double strike = jsonParams.at("Option").at("Strike").get<int>();
        opt = new CallCurrency(maturity, strike, domesticRate, currencies[0]->foreignRate_, numberOfDaysPerYear);
    }else{
        if(label == "call_quanto"){
            double strike = jsonParams.at("Option").at("Strike").get<int>();
            opt = new CallQuanto(maturity, strike, domesticRate, numberOfDaysPerYear);
        }
        else{
            if(label == "foreign_asian"){
                opt = new ForeignAsian(maturity, domesticRate, numberOfDaysPerYear);
            }
            else{
                if(label == "quanto_exchange"){
                    double strike = jsonParams.at("Option").at("Strike").get<int>();
                    opt = new QuantoExchange(maturity, strike, domesticRate, numberOfDaysPerYear);
                }
                else{
                    double strike = jsonParams.at("Option").at("Strike").get<int>();
                    opt = new QuantoExchange(maturity, strike, domesticRate, numberOfDaysPerYear);
                }
            }
        }
    }

    PnlVect * importantDates;
    jsonParams.at("Option").at("FixingDatesInDays").at("DatesInDays").get_to(importantDates);



    int nbSample;
    jsonParams.at("SampleNb").get_to(nbSample);

    Portfolio *portfolio = new Portfolio(assetNb+currNb-1);
    BlackScholesModel *bs = new BlackScholesModel(importantDates, assets, currencies, numberOfDaysPerYear);


    //random Initialisation
    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, (unsigned long)time(NULL));

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

    Hedger *hedger = new Hedger(portfolio, argv[2], mc, rebalancing);
    hedger->RebalanceAll();

    pnl_mat_free(&correlation);
    std::exit(0);
}