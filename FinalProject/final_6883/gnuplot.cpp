#include "gnuplot.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "gnuplot.hpp"

//This is now an individual function
int plotCAAR(const vector<double>& beat, const vector<double>& meet, const vector<double>& miss) {
    
    if (beat.size() == meet.size() && beat.size() == miss.size())
    {
        int dataSize = beat.size();
        FILE* gnuplotPipe, * tempDataFile;
        
        const char* tempDataFileName1 = "Beat";
        const char* tempDataFileName2 = "Meet";
        const char* tempDataFileName3 = "Miss";
        
        double x, y, x2, y2, x3, y3;
        int i = 0;;
        
        gnuplotPipe = popen("/opt/local/bin/gnuplot", "w"); //this should be the one for MAC
        //gnuplotPipe = _popen("C:\\FRE6883\\gnuplot\\bin\\gnuplot.exe", "w");
        
        if (gnuplotPipe) {
            fprintf(gnuplotPipe, "set term wxt size 800, 600\n"); // size of gnuplot window (console)
            fprintf(gnuplotPipe, "set xlabel \"%s\"\n", "Days from Anouncement");
            fprintf(gnuplotPipe, "set ylabel \"%s\"\n", "CAAR");
            fprintf(gnuplotPipe, "set xtics 10\n"); // step size of xticks
            fprintf(gnuplotPipe, "set ytics 0.1\n"); //step size of yticks
            fprintf(gnuplotPipe, "set xrange [\"%d\":\"%d\"]\n", -(dataSize / 2), dataSize / 2);
            fprintf(gnuplotPipe, "set title 'CAAR of 3 Groups (\%s\=\%d\)\n", "N", dataSize / 2);
            fprintf(gnuplotPipe, "set grid\n"); // gird added
            fprintf(gnuplotPipe, "plot \"%s\" with lines linewidth 1.3"
                ", \"%s\" with lines linewidth 1.3"
                ", \"%s\" with lines linewidth 1.3\n"
                , tempDataFileName1, tempDataFileName2, tempDataFileName3);
            
            fflush(gnuplotPipe);
            
            tempDataFile = fopen(tempDataFileName1, "w");
            for (i = 0; i < dataSize; i++) {
                x = i - dataSize / 2 + 1;
                y = beat[i];
                fprintf(tempDataFile, "%lf %lf\n", x, y);
            }
            fclose(tempDataFile);
            
            tempDataFile = fopen(tempDataFileName2, "w");
            for (i = 0; i < dataSize; i++) {
                x2 = i - dataSize / 2 + 1;
                y2 = meet[i];
                fprintf(tempDataFile, "%lf %lf\n", x2, y2);
            }
            fclose(tempDataFile);
            
            tempDataFile = fopen(tempDataFileName3, "w");
            for (i = 0; i < dataSize; i++) {
                x3 = i - dataSize / 2 + 1;
                y3 = miss[i];
                fprintf(tempDataFile, "%lf %lf\n", x3, y3);
            }
            fclose(tempDataFile);
            
            printf("Please enter 'p' to continue...");
            char a = getchar();
            while (a != 'p')
            {
                a = getchar();
            }
            
            remove(tempDataFileName1);
            remove(tempDataFileName2);
            remove(tempDataFileName3);
            
            fprintf(gnuplotPipe, "exit \n");
            //_pclose(gnuplotPipe);
            pclose(gnuplotPipe); //this may be necessary for MAC
        }
        else {
            printf("gnuplot not found...");
            
            return 1;
        }
    }
    else {
        cout << "CAAR vector sizes are not equal" << endl;
        
        return 1;
    }
    
    return 0;
}
