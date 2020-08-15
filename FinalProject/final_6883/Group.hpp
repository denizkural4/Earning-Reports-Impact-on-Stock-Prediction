//
//  Group.hpp
//  Final_Project
//
//  Created by Cristobal Gonzalez on 5/1/20.
//  Copyright © 2020 Cristobal Gonzalez. All rights reserved.
//

#ifndef Group_hpp
#define Group_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include<iostream> //linc
#include "Data.hpp"
#include "FinancialInstruments.hpp"


using namespace std;

class Group
{
private:
    
    Data * DataPtr;
    map <string, Stock*> GrpStockMap;
    vector<string> group_tickers; //liNCOLN EDIT
    vector <double> AAR;
    vector<double> CAAR;
    vector< double> AverageAAR;
    vector<double> AverageCAAR;
    vector<double> StdDevAAR;
    vector<double> StdDevCAAR;
    
public:
    Group(Data * DataPtr_, vector<string> group_tickers);// Lincoln's Edit changed 3 arguments to 2 with data pointer and group ticker names
    
    //void SelectStock();
    void Bootstrapping(int NumPerGroup, int NumTimes);
    void Refresh();
    int IsInGroup(string StockTicker);//LINCOLN EDIT COMPLETED
    void GetStockInfo(string StockTicker); //lincoln edit completed
    vector<double> RandomSampling(int NumPerGroup );//Lincoln add complete
    vector <double> GetAAR();
    vector<double>GetCAAR();
     vector< double> GetAverageAAR();
     vector< double> GetAverageCAAR();
     vector< double> GetStdDevAAR();
     vector< double> GetStdDevCAAR();
    void SetTickers(vector<string> Tickers_){group_tickers=Tickers_;};
    
};

#endif /* Group_hpp */
