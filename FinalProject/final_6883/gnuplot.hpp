#ifndef gnuplot_h
#define gnuplot_h
#include <vector>
using namespace std;

//Change the path to gnuplot.exe in gnuplot.cpp (search for "popen" to find the line)
//for MAC, the line# 21 with popen instead of _popen should be used.

int plotCAAR(const vector<double>& beat, const vector<double>& meet, const vector<double>& miss);

#endif // gnuplot_h



