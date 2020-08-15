//
//  Data.hpp
//  Final_Project
//
//  Created by Cristobal Gonzalez on 5/1/20.
//  Copyright © 2020 Cristobal Gonzalez. All rights reserved.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

#include <algorithm>
#include "FinancialInstruments.hpp"

using namespace std;


class Data
{
private:
    vector <string> StockTickers;
    string BenchTicker;
    map <string, Stock> StockMap;
    map <string, ETFs> BenchMap;
    vector <string> beat;
    vector <string> meet;
    vector <string> miss;
    vector <string> WorkingTickers;
public:
    Data(string& filename, string BenchTicker_);
    void SetStockN(int N,ETFs * ETFdates);
    void SetYahoo(ETFs * ETFdates); // create returns too
  
    map <string, Stock> GetStockMap();
    map <string, ETFs> GetBenchMap();
    void Group(); //Lincoln's Edit
    vector <string> GetBeat(){return beat;};
    vector <string> GetMeet(){return meet;};
    vector <string> GetMiss(){return miss;};
    string maxdate();
    string mindate();
    void printstockinfo ();
    vector <string> GetWorkingTickers(){return WorkingTickers;};
};

#endif /* Data_hpp */
