#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <algorithm>
#include <iomanip>
#include "curl/curl.h"
#include "FinancialInstruments.hpp"

// ---------------------- Local Functions for Libcurl ----------------------
struct MemoryStruct
{
    char* memory;
    size_t size;
};
int write_data(void* ptr, int size, int nmemb, FILE* stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

void* myrealloc(void* ptr, size_t size)
{
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}
int write_data2(void* ptr, size_t size, size_t nmemb, void* data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)data;
    mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

string getTimeinSeconds(string Time)
{
    std::tm t = { 0 };
    std::istringstream ssTime(Time);
    char time[100];
    memset(time, 0, 100);
    if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
    {
        //cout << std::put_time(&t, "%c %Z") << "\n"
        //          << std::mktime(&t) << "\n"; outputs time in seconds
        
        sprintf(time, "%lld", mktime(&t));
        return string(time);
    }
    else
    {
        
        cout << "Parse failed\n";
        return "";
    }
    
}


//iterates through ETF dates and find announcement date
//go +/-N from announcement date to check if N fits in the range.
//Then, find start and end date and return for each stock.

std::pair<string,string> plusNTradingDays(vector<string> days, string day0,int N,string Ticker)
{
    //
    ////iterates through ETF dates and find announcement date

    std::vector<string>::iterator it =(std::find(days.begin(), days.end(), day0)) ;

    //std::cout<<days(at.(std::distance(days.begin(), it))<<endl;
    if (it != days.end()){
      
            
        
        // Get index of element from iterator
        int index = std::distance(days.begin(), it);

        
        //go +/-N from an gbnnouncement date to check if N fits in the range.


        while(index-N-1<0 || (index+N)>days.size()){
            cout<<"Your N is too large/small!"<<endl;
            if((index+N)>days.size())
                cout << "Set new N for "<< Ticker << " announcement date that should be smaller than\t" << (days.size()-index)  << endl;
            else
                cout << "Set new N for "<< Ticker << " announcement date that should be larger than\t"  << index << endl;

            
            cin >> N;
        }
            
            //Then, find start and end date and return for each stock.

            string minusNday=days.at(index-N-1);
            string plusNday=days.at(index+N);
            return std::make_pair(minusNday,plusNday);
        }

        
    
    
    else{
        std::cout << "Element Not Found,try again." << std::endl;
        return std::make_pair("","");
        
    }
    
 
}


// ---------------------- FININST DEFINITIONS ----------------------


string FinInst::GetTicker()
{
    return Ticker;
}

void FinInst::SetTicker(string Ticker_)
{
    Ticker = Ticker_;
}

void FinInst::SetName(string Name_)
{
    Name = Name_;
}

int FinInst::CreateReturns()
{
    int SizeValues = (int) PriceValues.size();
if (SizeValues==0)
{
    cout<<Ticker<<" has size 0 data and rerunning libcurl to retrieve data."<<endl;
    return 0;
}

    Returns.clear();
    Returns.resize(SizeValues-1,0);
    
    for (int i = 1; i < SizeValues; i++)
    {
        Returns[i-1] = PriceValues[i] / PriceValues[i-1] - 1;

    }
    return 1;
}


vector <string> FinInst::GetPriceDates()
{
    return PriceDates;
}

vector <double> FinInst::GetPriceValues()
{
    return PriceValues;
}

vector <double> FinInst::GetReturns()
{
    return Returns;
}

int FinInst::InitLibCurl(){
    
        struct MemoryStruct data;
        data.memory = NULL;
        data.size = 0;

        // file pointer to create file that store the data
        FILE* fp;

        // name of files
    //    const char outfilename[FILENAME_MAX] = "Output.txt";
    //    const char resultfilename[FILENAME_MAX] = "Results.txt"; //remove

        // declaration of an object CURL
        CURL* handle;

        CURLcode result;

        // set up the program environment that libcurl needs
        curl_global_init(CURL_GLOBAL_ALL);

        // curl_easy_init() returns a CURL easy handle
        handle = curl_easy_init();

        // if everything's all right with the easy handle...
        if (handle)
        {
            string sCookies, sCrumb;
            if (sCookies.length() == 0 || sCrumb.length() == 0)
            {
               // fp = fopen(outfilename, "w");
                //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
                curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
                curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
                curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
                curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

    //            curl_easy_setopt(handle, CURLOPT_ENCODING, "");
    //            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
    //            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
    //            result = curl_easy_perform(handle);
                //fclose(fp);

    //            if (result != CURLE_OK)
    //            {
    //                // if errors have occurred, tell us what is wrong with result
    //                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
    //                return 1;
    //            }

                curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
                curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

                // perform, then store the expected code in result
                result = curl_easy_perform(handle);

                if (result != CURLE_OK)
                {
                    // if errors have occured, tell us what is wrong with result
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                    return 1;
                }

                char cKey[] = "CrumbStore\":{\"crumb\":\"";
                char* ptr1 = strstr(data.memory, cKey);
                char* ptr2 = ptr1 + strlen(cKey);
                char* ptr3 = strstr(ptr2, "\"}");
                if (ptr3 != NULL)
                    *ptr3 = NULL;

                sCrumb = ptr2;
                
                
                fp = fopen("cookies.txt", "r");
                char cCookies[100];
                if (fp) {
                    while (fscanf(fp, "%s", cCookies) != EOF);
                    fclose(fp);
                }
                else
                    cerr << "cookies.txt does not exists" << endl;

                sCookies = cCookies;
                
                cookies=sCookies;
                crumb=sCrumb;
                free(data.memory);
                data.memory = NULL;
                data.size = 0;
            }

        }
    return 0;
}
            
    


int FinInst::callYahoo(string StartDate,string EndDate)
{
    //Create tail info. for start and end date for correct input to getTimeinSecons
        PriceDates.clear();
    
       PriceValues.clear();
    string Tail= "T16:00:00";
    
    StartDate=StartDate + Tail;
    
    string startTime = getTimeinSeconds(StartDate);

    
    EndDate= EndDate + Tail;
    
    string endTime = getTimeinSeconds(EndDate);
    
   
    // file pointer to create file that store the data
    FILE* fp;

    // name of files
//    const char outfilename[FILENAME_MAX] = "Output.txt";
//    const char resultfilename[FILENAME_MAX] = "Results.txt"; //remove

    // declaration of an object CURL
    CURL* handle;

    CURLcode result;

    // set up the program environment that libcurl needs
    //curl_global_init(CURL_GLOBAL_ALL);

    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();

    // if everything's all right with the easy handle...
    if (handle)
    {
//        string sCookies, sCrumb;
//        if (sCookies.length() == 0 || sCrumb.length() == 0)
//        {
//           // fp = fopen(outfilename, "w");
//            //curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history?p=AMZN");
//            curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/AMZN/history");
//            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
//            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
//            curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
//            curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");
//
////            curl_easy_setopt(handle, CURLOPT_ENCODING, "");
////            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
////            curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
////            result = curl_easy_perform(handle);
//            //fclose(fp);
//
////            if (result != CURLE_OK)
////            {
////                // if errors have occurred, tell us what is wrong with result
////                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
////                return 1;
////            }
//
//            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
//            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
//
//            // perform, then store the expected code in result
//            result = curl_easy_perform(handle);
//
//            if (result != CURLE_OK)
//            {
//                // if errors have occured, tell us what is wrong with result
//                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//                return 1;
//            }
//
//            char cKey[] = "CrumbStore\":{\"crumb\":\"";
//            char* ptr1 = strstr(data.memory, cKey);
//            char* ptr2 = ptr1 + strlen(cKey);
//            char* ptr3 = strstr(ptr2, "\"}");
//            if (ptr3 != NULL)
//                *ptr3 = NULL;
//
//            sCrumb = ptr2;
//
//            fp = fopen("cookies.txt", "r");
//            char cCookies[100];
//            if (fp) {
//                while (fscanf(fp, "%s", cCookies) != EOF);
//                fclose(fp);
//            }
//            else
//                cerr << "cookies.txt does not exists" << endl;
//
//            sCookies = cCookies;
//            free(data.memory);
//            data.memory = NULL;
//            data.size = 0;
//        }

        
        
        
//FROM HERE CREATE ANOTHER CODE FILE FOR SPEED
        
        
        string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
        string urlB = "?period1=";
        string urlC = "&period2=";
        string urlD = "&interval=1d&events=history&crumb=";
        string url = urlA + Ticker + urlB + startTime + urlC + endTime + urlD + crumb;
        
        const char* cURL = url.c_str();
        const char* cookies2 = cookies.c_str();
        curl_easy_setopt(handle, CURLOPT_COOKIE, cookies2);   // Only needed for 1st stock
        curl_easy_setopt(handle, CURLOPT_URL, cURL);
        
        
        stringstream sData;
        struct MemoryStruct dataptr;
        dataptr.memory = NULL;
        dataptr.size = 0;

//        fp = fopen(resultfilename, "ab"); //not necessary to write to file
//        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
//        curl_easy_setopt(handle, CURLOPT_WRITEDATA, fp);
//        result = curl_easy_perform(handle);
//        fclose(fp); //remove

        // Check for errors */
//        if (result != CURLE_OK)
//        {
//            // if errors have occurred, tell us what is wrong with 'result'
//            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//            return 1;
//        }
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&dataptr);
        result = curl_easy_perform(handle);

        if (result != CURLE_OK)
        {
            // if errors have occurred, tell us what is wrong with result
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
            return 1;
        }

        sData.str(dataptr.memory);
        string sValue, sDate;
        double dValue = 0;
        string line;

        getline(sData, line);
        
        while (getline(sData, line)) {
            sDate = line.substr(0, line.find_first_of(','));
            line.erase(line.find_last_of(','));
            sValue = line.substr(line.find_last_of(',') + 1);
            dValue = strtod(sValue.c_str(), NULL);
            PriceDates.push_back(sDate);
            PriceValues.push_back(dValue);

        
        }
        
        free(dataptr.memory);
        dataptr.size = 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
        return 1;
    }
    
    /*
    if (Ticker != "SPY")
    {
        PriceDates.pop_back();
        PriceValues.pop_back();
    }
    */
     
    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);

    // release resources acquired by curl_global_init()
    curl_global_cleanup();

    return 0;
}


// ---------------------- ETFs DEFINITIONS ----------------------

ETFs::ETFs(string Ticker_):FinInst(Ticker_)
{
    Ticker = Ticker_;
}

ETFs::ETFs(string Ticker_, string Name_):FinInst(Ticker_,Name_)
{
    Ticker = Ticker_; Name = Name_;
}


void ETFs::SetBenchDates(string StartDate_, string EndDate_)
{
    StartDate = StartDate_; EndDate = EndDate_;
}
 vector <double> ETFs::FindReturns(string AnnoucementDate,int N){
     vector<double> returns;
     vector<string> ::iterator it=find(PriceDates.begin(),PriceDates.end(),AnnoucementDate);
     int index= distance(PriceDates.begin(),it);
     for(int i=index-N+1;i<index+N+1;i++){
        
         returns.push_back(Returns.at(i));
     }
   
     return returns;
}




// ---------------------- Stock DEFINITIONS ----------------------

Stock::Stock(string Ticker_):FinInst(Ticker_)
{
    Ticker = Ticker_;
}

Stock::Stock(string Ticker_, string Name_):FinInst(Ticker_,Name_)
{
    Ticker = Ticker_; Name = Name_;
}


int Stock::callYahoo(vector<string> etfdates)
{
    {

        //Call plusNTradingDays
            
        //vector <string> etfdates=ETFdates->GetPriceDates();
        std::pair<string, string>dates  = plusNTradingDays(etfdates,AnnouncementDate, N,Ticker);
        string StartDate=dates.first;
        string EndDate=dates.second;

        //EndDate = plusNTradingDays(PeriodDate, N);
        return FinInst::callYahoo(StartDate,EndDate);
    }
}


void Stock::SetBenchmark(ETFs * Benchmark_)
{
    Benchmark = Benchmark_;
}


int Stock::CreateAbReturns(ETFs * ETFdates)
{
    AbReturns.clear();
    if(CreateReturns()==0){
        return 0;
    }
    vector<double> etfreturns=ETFdates->FindReturns(AnnouncementDate, N);

    
 AbReturns = Returns - etfreturns;
    if(AbReturns.size()==0){
        cout<<Ticker<<" has size 0 abnormal return"<<endl;
    }
    return 1;
}


void Stock::SetN(int N_)
{
    N = N_;
}

vector <double> Stock::GetAbReturns()
{
    return AbReturns;
}

double Stock::GetSurprisePerc()
{
    return SurprisePerc;
}


string Stock::GetAnnouncementDate()
{
    return AnnouncementDate;
}
     
               
Stock:: Stock(string Ticker_,string Name_,string AnnouncementDate_,string PeriodDate_,double SurprisePerc_):FinInst(Ticker_,Name_){
    Ticker=Ticker_;
    Name=Name_;
    PeriodDate=PeriodDate_;
    SurprisePerc=SurprisePerc_;

    vector<string> result;
       stringstream ss (AnnouncementDate_);
       string item;

       while (getline (ss, item, '-')) {
           result.push_back (item);
       }
    string year,month,day;
    year="20";
    year.append(result.at(2));
    if(result.at(0).size()==1){
        day=to_string(0)+result.at(0);
        
    }else{
         day=result.at(0);
    }
    
   string months[] = {"Jan", "Feb", "Mar", "Apr", "May",
    "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    if(result.at(1)=="Jan"){
        month=to_string(0)+to_string(1);
    }else if(result.at(1)=="Feb"){
       month=to_string(0)+to_string(2);
    }else if(result.at(1)=="Mar"){
       month=to_string(0)+to_string(3);
    }else if(result.at(1)=="Apr"){
       month=to_string(0)+to_string(4);
    }else if(result.at(1)=="May"){
       month=to_string(0)+to_string(5);
    }else if(result.at(1)=="Jun"){
       month=to_string(0)+to_string(6);
    }else if(result.at(1)=="Jul"){
       month=to_string(0)+to_string(7);
    }else if(result.at(1)=="Aug"){
       month=to_string(0)+to_string(8);
    }else if(result.at(1)=="Sep"){
       month=to_string(0)+to_string(9);
    }else if(result.at(1)=="Oct"){
       month=to_string(10);
    }else if(result.at(1)=="Nov"){
       month=to_string(11);
    }else if(result.at(1)=="Dec"){
       month=to_string(12);
    }
    AnnouncementDate=year+"-"+month+"-"+day;
  
    
}






//Lincoln's Edit Stock copy Constructor
Stock::Stock(const Stock& a):FinInst(a){

    Ticker=a.Ticker;
    Name=a.Name;
    AnnouncementDate=a.AnnouncementDate;
    PeriodDate=a.PeriodDate;
    SurprisePerc=a.SurprisePerc;
  
    AbReturns=a.AbReturns;
    Ticker=a.Ticker;
    Name=a.Name;
    PriceDates=a.PriceDates;
    PriceValues=a.PriceValues;
    Returns=a.Returns;
    N=a.N;
}
Stock Stock:: operator =(const Stock& a){
      Ticker=a.Ticker;
      Name=a.Name;
      AnnouncementDate=a.AnnouncementDate;
      PeriodDate=a.PeriodDate;
      SurprisePerc=a.SurprisePerc;
    
      AbReturns=a.AbReturns;
      Ticker=a.Ticker;
      Name=a.Name;
      PriceDates=a.PriceDates;
      PriceValues=a.PriceValues;
      Returns=a.Returns;
      N=a.N;
    return a;
}
FinInst FinInst:: operator =(const FinInst& a){
       Ticker=a.Ticker;
       Name=a.Name;
       PriceDates=a.PriceDates;
       PriceValues=a.PriceValues;
       Returns=a.Returns;
       N=a.N;
        return a;
    
}
FinInst::FinInst (const FinInst& a){
    
       Ticker=a.Ticker;
       Name=a.Name;
       PriceDates=a.PriceDates;
       PriceValues=a.PriceValues;
       Returns=a.Returns;
       N=a.N;

}
void Stock::GetStockInfo(){

    cout<<Name<<" has announced their 2019 Q3 earnings on "<<AnnouncementDate<<" for quarter ended in "+PeriodDate<<" and the surprise is at "<<SurprisePerc<<"%"<<endl;

}
