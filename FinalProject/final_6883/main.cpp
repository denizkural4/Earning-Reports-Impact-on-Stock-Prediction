#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept> // std::runtime_error
#include <sstream>
#include "FinancialInstruments.hpp"
#include "Data.hpp"
#include "Group.hpp"
#include "gnuplot.hpp"
using namespace std;


void option1(Data&b,ETFs *SPY,Group& meet,Group& beat,Group& miss){
    int N=0;
       cout<<endl;
    cout<<"Enter your desired N: ";
       cout<<endl;
    cin>>N;

       cout<<endl;
    b.SetStockN(N,SPY);
    b.SetYahoo( SPY);
    b.Group();
    meet.SetTickers(b.GetMeet());
    meet.Refresh();
    cout<<"Bootstrapping Completed for Group Meet."<<endl;
     miss.SetTickers(b.GetMiss());
    miss.Refresh();
    cout<<"Bootstrapping Completed for Group Miss."<<endl;
    beat.SetTickers(b.GetBeat());
    beat.Refresh();
    cout<<"Bootstrapping Completed for Group Beat."<<endl;

    
}


void option2(Data& b,Group& meet,Group& beat,Group& miss){
     cout<<"Enter the stock ticker symbol to find its group and other information: ";
    string ticker="";
    cin>>ticker;
 
    if(meet.IsInGroup(ticker)==true){
           cout<<endl;
        cout<<ticker<<" is in group meet."<<endl;
           cout<<endl;
        b.GetStockMap().at(ticker).GetStockInfo();
           cout<<endl;
    }
    else if(beat.IsInGroup(ticker)==true){
           cout<<endl;
         cout<<ticker<<" is in group beat."<<endl;
           cout<<endl;
         b.GetStockMap().at(ticker).GetStockInfo();
           cout<<endl;

    }else if (miss.IsInGroup(ticker)==true){
           cout<<endl;
         cout<<ticker<<" is in group miss."<<endl;
           cout<<endl;
         b.GetStockMap().at(ticker).GetStockInfo();
           cout<<endl;
    
    }else{
           cout<<endl;
        cout<<ticker<<" is not in any of the three Group."<<endl;
           cout<<endl;
    }
    
 
    
}



void option3(Group& meet,Group& beat,Group& miss){
    int group_num=0;
    cout<<"Group 1: Beat"<<endl<<"Group 2: Meet"<<endl<<"Group 3: Miss"<<endl;
    cout<<"Enter the group number to see bootstrap results"<<endl;
    cin>>group_num;
    
    switch(group_num){
        case 1:
            for(int i=0; i<beat.GetAverageAAR().size();i++){
                cout<<endl;
                cout<<"Day "<<i+1<<" average AAR is "<<beat.GetAverageAAR().at(i)<<", average CAAR is "<<beat.GetAverageCAAR().at(i)<<", and the standard deviations are respectively "<<beat.GetStdDevAAR().at(i)<<" and "<<beat.GetStdDevCAAR().at(i)<<endl;
                   cout<<endl;
            }
            break;
        case 2:
            for(int i=0; i<meet.GetAverageAAR().size();i++){
                   cout<<endl;
                       cout<<"Day "<<i+1<<" average AAR is "<<meet.GetAverageAAR().at(i)<<", average CAAR is "<<meet.GetAverageCAAR().at(i)<<", and the standard deviations are respectively "<<meet.GetStdDevAAR().at(i)<<" and "<<meet.GetStdDevCAAR().at(i)<<endl;
                   cout<<endl;
                   }
                   break;
        case 3:
            for(int i=0; i<miss.GetAverageAAR().size();i++){
                   cout<<endl;
                cout<<"Day "<<i+1<<" average AAR is "<<miss.GetAverageAAR().at(i)<<", average CAAR is "<<miss.GetAverageCAAR().at(i)<<", and the standard deviations are respectively "<<miss.GetStdDevAAR().at(i)<<" and "<<miss.GetStdDevCAAR().at(i)<<endl;
                   cout<<endl;
            }
            break;
            
    }
}

void option4(Group& meet, Group& beat, Group& miss){
       cout<<endl;
    cout<<"Displaying CAAR graphs now......"<<endl;
       cout<<endl;
    plotCAAR(beat.GetAverageCAAR(), meet.GetAverageCAAR(), miss.GetAverageCAAR());
    
}
void option5(ETFs *newETF){
    
       cout<<endl;
    
    cout<<"Enter the ETF ticker ";
       cout<<endl;
    string ticker="";
    cin>>ticker;
    
    newETF->SetTicker(ticker);
    
       cout<<endl;
    string newStartDate;
    cout<<"Enter your start date for ETF : " ;
       cout<<endl;
    cin>>newStartDate;
    cout<<endl;
    
    string newEndDate;
       cout<<endl;
    cout<<"Enter your end date for ETF : " ;
    
    cin>>newEndDate;
    cout<<endl;
    
    cout<<newETF->GetTicker()<<endl;
    
    newETF->SetBenchDates(newStartDate,newEndDate);
    newETF->callYahoo(newStartDate,newEndDate);
    
}




int main() {
    string a="S&P_EPS_V6.csv";
    Data b(a," ");
    
    b.Group();
    
    Group meet(&b,b.GetMeet());
    Group miss(&b,b.GetMiss());
    Group beat(&b,b.GetBeat());

    string StartDate="2019-04-04T16:00:00";
    string EndDate="2020-04-04T16:00:00";
    ETFs SPY("SPY");
    SPY.InitLibCurl();
    SPY.SetBenchDates(StartDate,EndDate);
    SPY.callYahoo(StartDate,EndDate);
    SPY.CreateReturns();

    int option=0;
   
    option1(b,&SPY,meet,beat,miss);
    

    do{
        cout<<"Menu Options: "<<endl<<endl;
        cout<<"       1: Enter N to retrieve historical price data for all stocks"<<endl;
        cout<<"       2: Pull information for one stock from one group."<<endl;
        cout<<"       3: Show AAR, AAR-SD, CAAR and CAAR-STD for one group"<<endl;
        cout<<"       4: Show the Excel or gnuplot graph with CAAR for all 3 Group."<<endl;
        cout<<"       5: Enter Ticker, Start and End dates for ETF"<<endl;
        cout<<"       6: Exit program."<<endl;
        cout<<endl<<"Enter a number to choose an option to proceed: "<<endl;
        cin>>option;

        switch(option){
            case 1:
               option1(b,&SPY,meet,beat,miss);
              break;
            case 2:
               option2(b,meet,beat,miss);
              break;
            case 3:
                option3(meet,beat,miss);
                break;
            case 4:
               option4(meet,beat,miss);
               break;
            case 5:
                option5(&SPY);
                break;
            case 6:
                cout<<"Program terminated at your request"<<endl;
                break;
            default:
                cout<<"Please enter an integer from 1 to 6."<<endl;
                option=7;
                break;

        }

    }
    while(option!=6);
}
