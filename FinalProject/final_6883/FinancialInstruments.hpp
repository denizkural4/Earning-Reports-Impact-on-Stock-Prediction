//
//  FinancialInstruments.hpp
//  Final_Project
//
//  Created by Cristobal Gonzalez on 5/1/20.
//  Copyright © 2020 Cristobal Gonzalez. All rights reserved.
//

#ifndef FinancialInstruments_hpp
#define FinancialInstruments_hpp

#include <stdio.h>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;




// ---------------------- FinInst DECLARATION ----------------------

class FinInst
{
protected:
    string Ticker;
    string Name;
    vector <string> PriceDates;
    vector <double> PriceValues;
    vector <double> Returns;
    string StartDate;
    string EndDate;
    
public:
     
    int InitLibCurl();
    string cookies;
    
    string crumb;
    int callYahoo(string StartDate, string SetDate);
    int N;
    void SetTicker(string Ticker_);
    void SetName(string Name_);
    int CreateReturns();
    string GetTicker();//Lincoln's Edit get ticker symbol
    vector <string> GetPriceDates();
    vector <double> GetPriceValues();
    vector <double> GetReturns();
   FinInst operator =(const FinInst& a);
   FinInst (const FinInst& a);
    FinInst() { }
    FinInst(string Ticker_){Ticker=Ticker_;};
    FinInst(string Ticker_, string Name_){Ticker=Ticker_;Name=Name_;};
};



// ---------------------- ETFs DECLARATION ----------------------

class ETFs : public FinInst
{


public:
    ETFs() { }
    ETFs(string Ticker_);
    ETFs(string Ticker_, string Name_);
    vector<double> FindReturns(string AnnoucementDate,int N);
    //do not need callYahoo here, call base class method
    void SetBenchDates(string StartDate_, string EndDate_);
//    vector <string> ETFGetPriceDates();
    

    
};


// ---------------------- Stock DECLARATION ----------------------

class Stock : public FinInst
{
protected:
    string AnnouncementDate;
    string PeriodDate;
    double SurprisePerc;
    FinInst * Benchmark;
    vector <double> AbReturns;
    //int N;
    string StartDate; // moved these to base class, else i have to write libcurl function twice
    string EndDate; // must be in format "2020-04-01T16:00:00"
    
public:
    Stock() { }
    Stock(string Ticker_);
    Stock(string Ticker_, string Name_);
    //Lincoln's Edit Stock Constructor with 5 parameters
    
    //Lincoln's Edit Stock copy constructor
    Stock(string Ticker_,string Name_,string AnnouncementDate_,string PeriodDate_,double SurprisePerc_);
    int callYahoo(vector<string> etfdates);
    void SetBenchmark(ETFs * Benchmark_);
    int CreateAbReturns(ETFs * ETFdates);
    void SetN(int N_);
    vector <double> GetAbReturns();
    double GetSurprisePerc();
    void GetStockInfo();
    string GetAnnouncementDate();
    Stock operator =(const Stock& a);
    Stock(const Stock& a);
};

vector<double> static operator-(const vector<double>& a, const vector<double>& b)
{
    vector<double> result;
    if (a.size() == b.size())
    {
        result.resize(a.size());

        for (int i = 0; i < a.size(); i++)
        {
            result[i] = a[i] - b[i];
        }
    }
    else
    {
        cout << "vector sizes do not match" << endl;
        exit(1);
    }

    return result;
}


#endif /* FinancialInstruments_hpp */
