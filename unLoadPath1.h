#ifndef UNLOADPATH1_H
#define UNLOADPATH1_H

#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath2.h"

class unLoadPath1 : public generalPath
{
public:
    
    //Three Points
    unLoadPath1(std::vector<double> xdata, std::vector<double> ydata, unsigned int direction);
    unLoadPath1();
    ~unLoadPath1();
    
    unLoadPath2 unload(double x, double y, double curE, double curRev);
    double getE(double x);
    
    double E, E1;
    
private:
    virtual void initial();
    unsigned int direction;//0 - low to high; 1 - high to low
    
};


#endif