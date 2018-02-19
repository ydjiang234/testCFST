#ifndef TRIBACKBONE_H
#define TRIBACKBONE_H

#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath1.h"


class TriBackbone : public generalPath
{
public:
    TriBackbone(std::vector<double> xdata, std::vector<double> ydata);
    TriBackbone();
    ~TriBackbone();
    
    unLoadPath1 unload(double x, double y, double curE, double curRev);
    double getE(double x);
    unsigned int isLeave(double nextX);
    

private:
    //to get the points on the backbone
    void initial();
    
    

};

#endif // TRIBACKBONE_H
