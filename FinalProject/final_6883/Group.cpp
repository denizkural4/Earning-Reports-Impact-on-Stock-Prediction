//
//  Group.cpp
//  Final_Project
//
//  Created by Cristobal Gonzalez on 5/1/20.
//  Copyright © 2020 Cristobal Gonzalez. All rights reserved.
//

#include "Group.hpp"
#include <random>

//Lincoln's edit
 Group::Group(Data * DataPtr_, vector<string> group_tickers_)
{
    DataPtr = DataPtr_;
    group_tickers=group_tickers_;
    for(int i=0;i<group_tickers.size();i++){
        Stock* a=&DataPtr_->GetStockMap().at(group_tickers.at(i));
        GrpStockMap.insert(pair<string,Stock*>(group_tickers.at(i),a));
     
    }
}

void Group::Bootstrapping(int NumPerGroup, int NumTimes)
{
    AverageAAR.clear();
    AverageCAAR.clear();
    StdDevAAR.clear();
    StdDevCAAR.clear();
    vector<double> temp;
    
    temp=RandomSampling(30);
    
    int length=(int)temp.size();

    AverageAAR.resize(length,0);
    AverageCAAR.resize(length,0);
    StdDevAAR.resize(length,0);
    StdDevCAAR.resize(length,0);
    
    vector <int> aa0;
    vector <double> aa1;
    vector <double> aa2;
    vector <double> bb1;
    vector <double> bb2;
    
    aa0.resize(length,0);
    aa1.resize(length,0);
    aa2.resize(length,0);
    bb1.resize(length,0);
    bb2.resize(length,0);
    
    double temp_double;
    
    for(int i=0;i<NumTimes;i++)
    {
        temp_double = 0.0;
        temp.clear();
        temp=RandomSampling(NumPerGroup);
        for(int j=0;j<length;j++){
            
            AverageAAR.at(j)+=temp.at(j)/(NumTimes+0.0);
            
            temp_double = temp.at(j) + temp_double;
        
            AverageCAAR.at(j) += temp_double/(NumTimes+0.0);
            
            aa0[j] += 1;
            aa1[j] += temp.at(j);
            aa2[j] += temp.at(j) * temp.at(j);
            bb1[j] += temp_double;
            bb2[j] += temp_double * temp_double;
            
        }
    }
    
    double auxAvg;
    double auxStDev;
    double auxAvg2;
    double auxStDev2;
    
    for(int j=0;j<length;j++)
    {
        auxAvg = aa1[j] / aa0[j];
        auxStDev = sqrt((aa2[j] / aa0[j]) - auxAvg * auxAvg);
        StdDevAAR[j] = auxStDev * sqrt(aa0[j]) / sqrt(aa0[j]-1);
        
        auxAvg2 = bb1[j] / aa0[j];
        auxStDev2 = sqrt((bb2[j] / aa0[j]) - auxAvg2 * auxAvg2);
        StdDevCAAR[j] = auxStDev2 * sqrt(aa0[j]) / sqrt(aa0[j]-1);
        
    }
    
}

int Group::IsInGroup(string StockTicker)
{
    std::vector<string>::iterator it = find(group_tickers.begin(), group_tickers.end(), StockTicker);
       if(it!=group_tickers.end())
       {
           return 1;
       }
    else return 0;
}

void Group::GetStockInfo(string StockTicker)
{
    GrpStockMap.at(StockTicker)->GetStockInfo();
}

vector <double> Group::GetAAR()
{
    return AAR;
}
vector<double> Group::GetCAAR()
{
    return CAAR;
}

 vector< double> Group::GetAverageAAR()
{
    return AverageAAR;
}

 vector< double> Group::GetAverageCAAR()
{
    return AverageCAAR;
}

 vector< double> Group::GetStdDevAAR()
{
    return StdDevAAR;
}

 vector< double> Group::GetStdDevCAAR()
{
    return StdDevCAAR;
}


vector<double> Group::RandomSampling(int NumPerGroup ){
    const int range_from  = 0;
    const int range_to    =(int)group_tickers.size()-1;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);

    vector<double> sample_aar;
    vector<double> temp;
    int rand=0;
    int length=(int)((*DataPtr).GetStockMap().at(group_tickers.at(0))).GetAbReturns().size();
    sample_aar.resize(length,0.0);
   

    for(int i=0;i<NumPerGroup;i++){
        temp.clear();
        rand=distr(generator);
        temp=DataPtr->GetStockMap().at(group_tickers.at(rand)).GetAbReturns();
        
        for(int j=0;j<length;j++){
   
            sample_aar.at(j)+=temp.at(j)/(NumPerGroup+0.0);
        }
    }
  
    int num=0;
    for(vector<double> ::iterator it=sample_aar.begin();it!=sample_aar.end();++it){
        num++;
   
    }
    return sample_aar;
    
}
void Group::Refresh(){
    AAR.clear();
    CAAR.clear();
    vector<double> temp;
    int length=(int)((*DataPtr).GetStockMap().at(group_tickers.at(0))).GetAbReturns().size();

    AAR.resize(length,0.0);
    CAAR.resize(length,0.0);

    double temp_double=0.0;
    for(int i=0;i<group_tickers.size();i++){
          temp.clear();
          
          temp=(*DataPtr).GetStockMap().at(group_tickers.at(i)).GetAbReturns();
       
          for(int j=0;j<length;j++){
     
              AAR.at(j)+=temp.at(j)/(group_tickers.size()+0.0);
             
          }
        
      }
    temp_double=0.0;
    
    for(int j=0;j<length;j++){
        temp_double+=AAR.at(j);
        CAAR.at(j)=temp_double;

    }

    Bootstrapping(30, 30);
}
