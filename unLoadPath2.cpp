#include <iostream>
#include <vector>
#include "generalPath.h"
#include "unLoadPath2.h"
#include <float.h>


unLoadPath2::unLoadPath2(std::vector<double> xdata, std::vector<double> ydata): generalPath(xdata, ydata)
{
    this->initial();
}

unLoadPath2::unLoadPath2(): generalPath() {}

unLoadPath2 unLoadPath2::unload(double x, double y, double curE, double curRev)
{
    double Rev, tempx1;
    std::vector<double> tempxdata, tempydata;
    
    if (this->isLinear(x) == false) {
        if (x > this->xdata[2]) {
            Rev = -1 * curRev;
        } else {
            Rev =  curRev;
        }
        tempx1 = x - (y - Rev) / curE;
        
        if (x > this->xdata[2]) {
            tempxdata = {this->xdata[0], tempx1, x, this->xdata.back()};
            tempydata = {this->ydata[0], Rev, y, this->ydata.back()};
        } else {
            tempxdata = {this->xdata[0], x, tempx1, this->xdata.back()};
            tempydata = {this->ydata[0], y, Rev, this->ydata.back()};
        }
    }
    return unLoadPath2(tempxdata, tempydata);
}

bool unLoadPath2::isLeave(double nextX)
{
    bool out;
    if (this->isInRange(nextX)) {
        if (this->isLinear(this->curX)) {
            return false;
        } else {
            if (this->curE() == this->E1) {
                if ((nextX - this->curX) < 0) {out = false;} else {out = true;}
            } else if (this->curE() == this->E2) {
                if ((nextX - this->curX) > 0) {out = false;} else {out = true;}
            }
        }
    } else {
        out = false;
    }
    return out;
}

double unLoadPath2::getE(double x)
{
    double out;
    if (this->isInRange(x)) {
        if (x <= this->xdata[1]) {
            out = this->E1;
        } else if (x <= this->xdata[2]) {
            out = this->E;
        } else if (x <= this->xdata[3]) {
            out = this->E2;
        }
    } else {
        out = DBL_EPSILON;
    }
    return out;
}

unLoadPath2::~unLoadPath2() {}

void unLoadPath2::initial()
{
        this->linearRange.push_back(this->xdata[1]);
        this->linearRange.push_back(this->xdata[2]);
        this->E = (this->ydata[2] - this->ydata[1]) / (this->xdata[2] - this->xdata[1]);
        this->E1 = (this->ydata[1] - this->ydata[0]) / (this->xdata[1] - this->xdata[0]);
        this->E2 = (this->ydata[3] - this->ydata[2]) / (this->xdata[3] - this->xdata[2]);
}
