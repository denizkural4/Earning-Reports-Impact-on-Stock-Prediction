//
//  Data.cpp
//  Final_Project
//
//  Created by Cristobal Gonzalez on 5/1/20.
//  Copyright © 2020 Cristobal Gonzalez. All rights reserved.
//

#include "Data.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream>
#include <map>
#include <algorithm>





//READ THE DATA FROM .CSV FILE

Data::Data(string& filename, string BenchTicker_)
{
     BenchTicker = BenchTicker_;
    vector<string> ticker_symbol,company_name,annoucement_date,quarter_end;
    vector<double> surprise_percent,estimated_eps,actual_eps,surprise_value;
              ifstream myFile(filename,ios::in);//create file input object
              if (!myFile.is_open()) throw std::runtime_error("Could not open file");//throw unopen file error
              string line = ""; string num = "";
              vector<string> initials;
              if (myFile.good())
              {
    
                  while (std::getline(myFile, line)) {
                      stringstream ss(line);
                      while (getline(ss, num, ',')) {
                          initials.push_back(num);
                         
                      }
    
                  }
                  myFile.close();
    
              }
    
    //GET THE INFO. FROM CSV FILE INTO PARTS FOR CREATING STOCK OBJECT
              double  val = 0.0; string::size_type sz; int val_int = 0;
              const int& z = initials.size();
              for (int i = 8; i < z; i++)
              {
                  switch (i % 8) {
                  case 0:ticker_symbol.push_back(initials.at(i));
                      break;
                  case 1:
                      company_name.push_back(initials.at(i));
                      break;
                  case 2:
                      annoucement_date.push_back(initials.at(i));
                      break;
                  case 3:
                      quarter_end.push_back(initials.at(i));
                      break;
                  case 4:
                     val = stod(initials.at(i), &sz);
                      estimated_eps.push_back(val);
                      break;
                  case 5:
                        val = stod(initials.at(i), &sz);
                     actual_eps.push_back(val);
                      break;
                  case 6:
                          val = stod(initials.at(i), &sz);
                      surprise_value.push_back(val);
                      break;
                case 7:
                          val = stod(initials.at(i), &sz);
                    surprise_percent.push_back(val);
                          break;
                  }
              }
                myFile.close();
    
    //CREATE STOCK OBJECT FOR  EACH LINE & CREATE STOCK MAP
    StockTickers=ticker_symbol;
    for(int i=0;i<StockTickers.size();i++){
        Stock a=Stock(StockTickers.at(i),company_name.at(i),annoucement_date.at(i),quarter_end.at(i),surprise_percent.at(i));
        
        StockMap.insert(pair<string,Stock>(StockTickers.at(i),a));

    }
 
}








void Data::SetStockN(int N,ETFs * ETFdates)
{
    
    // Get Max DAte Min Date
    vector <string> etfdates=ETFdates->GetPriceDates();

    //Control if N is smaller than 30.
    while(N<30){
        cout<< "Give new N that is larger than 30"<<endl;
        cout << "N:\t"; cin>>N;
    }
    
    //get the max and min date from stock list.
    string minimumdate= mindate();
    string maximumdate= maxdate();

    //Find their locations in ETF dates vector.
    std::vector<string>::iterator min_it =(std::find(etfdates.begin(), etfdates.end(), minimumdate)) ;
    
     std::vector<string>::iterator max_it =(std::find(etfdates.begin(), etfdates.end(), maximumdate)) ;
     
    
    //If both days are found, Day will be found in the ETF list, otherwise

    if (min_it != etfdates.end() || max_it != etfdates.end() ){

            
        
        // Get index of element from iterator
        int min_index = distance(etfdates.begin(), min_it);
        int max_index = distance(etfdates.begin(), max_it);

        
        //go +/-N from an gbnnouncement date to check if N fits in the range.

        while(min_index-N<0 || (max_index+N)>etfdates.size()){
            cout<<"Your N is too large!"<<endl;
            if (etfdates.size()-max_index>min_index)
                cout << "Set new N that should be smaller than\t" << min_index-1 << endl;
            else
                cout << "Set new N that should be smaller than\t" << etfdates.size()-max_index-1 << endl;


            
            cin >> N;
        }
        
    }
    else{
        cout<<"Error: The max or min day cannot be found in the ETF date list."<<endl;
        return;
        
    }
        
    
    cout<<"N has been successfully set to "<<N<<" at your request."<<endl;

   
    for(int i=0;i<StockTickers.size();i++){
        StockMap.at(StockTickers.at(i)).SetN(N);
    }

    
}


void Data::SetYahoo(ETFs * ETFdates)
{
    
    //GET ETF PRICE DATE INFO
    vector <string> etfdates=ETFdates->GetPriceDates();
    //cout<< (etfdates.size())<<endl;
    vector<string> absentee;
    //ITERATE THROUGH STOCKMAP TO FİND PRICE AND DATE INFO OF EACH STOCK IN STOCK MAP
    int num=0;
    int perc=0;
    int status=1;
    for(map <string, Stock>::iterator it = StockMap.begin(); it != StockMap.end(); it++) {
        it->second.callYahoo(etfdates);
        
       
        status=it->second.CreateAbReturns(ETFdates);
        if(status==0){
            
            absentee.push_back(it->second.GetTicker());
            cout<<"Cannot get data for "<<it->second.GetTicker()<<endl;
     
        }
        num++;
              if(num%25==0){
                  perc=(int)100*(num+0.0)/(StockTickers.size()+0.0);
                  cout<<perc<<"% 0f Yahoo info has been retrived"<<endl;
                  
              }
    }
  
    cout<<"100% 0f Yahoo info has been retrived"<<endl;
    // we are doing polymorphism in the CallGetData since the parameter is a FinInst pointer
    // and we put arguments from different classes (Stock and ETFs)
    WorkingTickers=StockTickers;
    if(absentee.size()>0)
    {
        for(int i=0;i<absentee.size();i++){
            cout<<absentee.at(i)<<" has been temporarily removed because libcurl cannot get data for this stock"<<endl;
            vector<string> ::iterator it=find(WorkingTickers.begin(),WorkingTickers.end(),absentee.at(i));
            WorkingTickers.erase(it);
        }
    }
}




map <string, Stock> Data::GetStockMap()
{
    return StockMap;
}

map <string, ETFs> Data::GetBenchMap()
{
    return BenchMap;
}

void Data::Group(){
    
    miss.clear();
    meet.clear();
    beat.clear();
    int increment=0;
    increment=(int)WorkingTickers.size()/3;
    vector<string>::iterator it=WorkingTickers.begin();
    vector<string>::iterator lower=WorkingTickers.begin()+increment;
    vector<string>::iterator upper=WorkingTickers.begin()+2*increment;
    for(it;it!=lower;++it){
        miss.push_back(*it);
    
    }
   
    for(lower;lower!=upper;++lower){
        meet.push_back(*lower);
    
    }
  
    for(upper;upper!=WorkingTickers.end();++upper){
        beat.push_back(*upper);
       
    }
}




//In case, we need to look for N at the start and end.

//Find maximum (latest) date from the stock map
string Data::maxdate(){

    string maxdate="0000-00-00";
   


    for(map <string, Stock>::iterator it =  StockMap.begin(); it !=  StockMap.end(); it++) {
        if(it->second.GetAnnouncementDate()>maxdate){
            maxdate=it->second.GetAnnouncementDate();
        }
    }
    return maxdate;

}

//Find minimum (earlier) date from the stock map
string Data::mindate(){

    string mindate="9999-99-99";
    

    for(map <string, Stock>::iterator it =  StockMap.begin(); it !=  StockMap.end(); it++) {
        if(it->second.GetAnnouncementDate()<mindate)
            mindate=it->second.GetAnnouncementDate();



    }
    return mindate;

}


//Print corresponding price and date info. from each Stock in the stock map.
void Data::printstockinfo (){
    
    for(map <string, Stock>::iterator it = StockMap.begin(); it != StockMap.end(); it++) {
          cout << it->second.GetTicker() << endl;
//          cout << it->second.GetPriceDates().size() << endl;
//          cout << it->second.GetPriceValues().size() << endl;
        
          for (int i=0; i<it->second.GetPriceDates().size(); i++)
              cout << it->second.GetPriceDates().at(i) << endl;
          for (int i=0; i<it->second.GetPriceValues().size(); i++)
              cout << it->second.GetPriceValues().at(i) << endl;
               


    }
    
}

