#include "generalPath.h"
#include <iostream>
#include <vector>
#include <float.h>

generalPath::generalPath(std::vector<double> xdata, std::vector<double> ydata)
{
    this->xdata = xdata;
    this->ydata = ydata;
}

generalPath::generalPath() {};

generalPath::~generalPath() {};

double generalPath::getY(double x)
{
    double out;
     int curId;
    if (this->isInRange(x))
    {
        std::vector<double>::iterator low;
        low=std::lower_bound (this->xdata.begin(), this->xdata.end(), x);
        curId = low - this->xdata.begin() - 1;
        if (curId < 0)
            curId = 0;
        out = this->interp(x, this->xdata[curId], this->xdata[curId+1], this->ydata[curId], this->ydata[curId+1]);
    } else {
        out = DBL_EPSILON;
    }
    
    return out;
}

double generalPath::interp(double x, double x1, double x2, double y1, double y2)
{
    double k = (y2 - y1) / (x2 - x1);
    double out = y1 + (x - x1) * k;
    return out;
}

bool generalPath::isLinear(double x)
{
    double low, up;
    low = this->linearRange[0];
    up = this->linearRange[1];
    if ((x <= up) && (x>=low))
        return true;
    else
        return false;
}

bool generalPath::isInRange(double x)
{
    return this->inRange(x, this->xdata[0], this->xdata.back());
}

bool generalPath::inRange(double x, double low, double high)
{
    if (x < low)
        return false;
    else if (x > high)
        return false;
    else
        return true;
}