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
#include <map>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

int main(int argc, char **argv) {
    // if (argc > 0) {
    //      std::cerr << "Wrong number of arguments. Exactly one argument is required" << std::endl;
    //      std::exit(0);
    // }
    std::cout << argv[1] << std::endl;
    std::ifstream ifs(argv[1]);
    nlohmann::json jsonParams = nlohmann::json::parse(ifs);
    std::cout << argv[1] << std::endl;

    int assetNb = jsonParams.at("Assets").size();
    int currNb = jsonParams.at("Currencies").size();
    PnlMat *correlation = pnl_mat_create_from_zero(assetNb + currNb - 1, assetNb + currNb - 1);
    
    // jsonParams.at("Correlations").get_to(correlation);

    map<string,Currency*> currencies;
    std::cout << "-- currencies" << std::endl;

    std::string domesticCurrencyId;
    jsonParams.at("DomesticCurrencyId").get_to(domesticCurrencyId);
    double domesticRate = 0;

    PnlVect* sigmaat = pnl_vect_create_from_zero(1);
    pnl_vect_print(sigmaat);
    auto jsonCurrencies = jsonParams.at("Currencies");
    for (auto jsonCurrency : jsonCurrencies) {
        std::string currencyId = jsonCurrency.at("id").get<std::string>();
        if (currencyId == domesticCurrencyId){
            domesticRate = jsonCurrency.at("InterestRate").get<double>();
        }
        else{
            double foreignRate = jsonCurrency.at("InterestRate").get<double>();
            Currency *currency = new Currency(domesticRate, foreignRate, sigmaat);
            // currencies.insert(std::make_pair(currencyId, Currency);
        }
    }
    
    std::cout << "Number of assets " << assetNb << std::endl;

    
    vector<RiskyAsset*> assets;
    std::cout << "-- assets" << std::endl;
    auto jsonAssets = jsonParams.at("Assets");
    for (auto jsonAsset : jsonAssets) {
        string id = jsonAsset.at("CurrencyId").get<std::string>();
        Currency *curr = currencies[id];
        // RiskyAsset *asset = new RiskyAsset(curr, sigmaat); 
        // assets.push_back(asset);
    }

    int numberOfDaysPerYear = jsonParams.at("NumberOfDaysInOneYear").get<int>();
    double maturity = jsonParams.at("Option").at("MaturityInDays").get<int>() / double (numberOfDaysPerYear);
    std::string label = jsonParams.at("Option").at("Type").get<std::string>();

    if(label == "call_currency"){
        double strike = jsonParams.at("Option").at("Strike").get<int>();
        // CallCurrency *opt = new CallCurrency(maturity, strike, domesticRate, currencies.first)
    }

    // PnlVect * importantDates;


    // int nbSample;
    // jsonParams.at("SampleNb").get_to(nbSample);

    // Portfolio portfolio = new Portfolio(assetNb);
    // BlackScholesModel *bs = new BlackScholesModel();


    // //random Initialisation
    // PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    // pnl_rng_sseed(rng, (unsigned long)time(NULL));


    // MonteCarlo *mc = new MonteCarlo(bs, opt, rng,nbSample);
    



    // Hedger hedger = new Hedger(portfolio, argv[2], mc);
    // hedger.RebalanceAll();

    // pnl_mat_free(&correlation);
    std::exit(0);
}