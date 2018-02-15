#include "generalPath.h"
#include <iostream>
#include <vector>
#include <float.h>
#include <algorithm>

generalPath::generalPath(std::vector<double> xdata, std::vector<double> ydata)
{
    this->xdata = xdata;
    this->ydata = ydata;
    this->isOnPath = false;
}

generalPath::generalPath() {};

generalPath::~generalPath() {};

void generalPath::back2Path(double x)
{
    this->curX = x;
    this->curY = this->getY(x);
    this->isOnPath = true;
}

double generalPath::getY(double x)
{
    double out;
     int curId;
    if (this->isInRange(x))
    {
        std::vector<double>::iterator low;
        low=std::lower_bound(this->xdata.begin(), this->xdata.end(), x);
        curId = low - this->xdata.begin() - 1;
        if (curId < 0)
            curId = 0;
        out = this->interp(x, this->xdata[curId], this->xdata[curId+1], this->ydata[curId], this->ydata[curId+1]);
    } else {
        out = DBL_EPSILON;
    }
    
    return out;
}

bool generalPath::isLeave(double nextX)
{
    return false;
}

double generalPath::getE(double x)
{
    return 0;
}

double generalPath::curE()
{
    return this->getE(this->curX);
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
    if (this->inRange(x, low, up))
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
    if (x < (low + DBL_EPSILON))
        return false;
    else if (x > (high - DBL_EPSILON))
        return false;
    else
        return true;
}
