#ifndef GENERALPATH_h
#define GENERALPATH_h

#include <iostream>
#include <vector>

class generalPath
{
public:
    generalPath(std::vector<double> xdata, std::vector<double> ydata);
    generalPath();
    ~generalPath();
    bool isLinear(double x);
    bool isInRange(double x);
    double getY(double x);
    
    std::vector<double> xdata, ydata;
    std::vector<double> linearRange;
    
    
private:
    bool inRange(double x, double low, double high);
    double interp(double x, double x1, double x2, double y1, double y2);
};

#endif