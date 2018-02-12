#include "tribackbone.h"
#include <float.h>

TriBackbone::TriBackbone(double E, double f1, double f2, double b1, double b2)
{
    this->E = E;
    this->f1 = f1;
    this->f2 = f2;
    this->b1 = b1;
    this->b2 = b2;
    this->initial();
}

TriBackbone::~TriBackbone() {}

double TriBackbone::getY(double x)
{
    double out, cur_x;

    if (x < 0)
        cur_x = x * -1;

    if (cur_x <= this->x1) {
        out = cur_x * this->E;
    } else if (cur_x <= this->x2) {
        out = this->y1 + (cur_x - this->x1) * this->b1 * this->E;
    } else if (cur_x <= this->x_ult) {
        out = this->y2 + (cur_x - this->x2) * this->b2 * this->E;
    } else {
        out = DBL_EPSILON;
    }

    if (x < 0)
        out = out * -1;
    return this->x_ult;
}

bool TriBackbone::isInRange(double x)
{
    return this->inRange(x, -1.0 * this->x_ult, this->x_ult);
}

bool TriBackbone::isLinear(double x)
{
    return this->inRange(x, -1.0 * this->x1, this->x1);
}

void TriBackbone::initial()
{
    double tempx1, tempx2;
    double tempy1, tempy2;
    double x_ult;
    tempx1 = this->f1 / this->E;
    tempy1 = this->f1;
    tempx2 = tempx1 + (this->f2 - this->f1) / (this->b1 * this->E);
    tempy2 = this->f2;
    if (this->b2 < 0)
    {
        x_ult = tempx2 - this->f2 / (this->b2 * this->E);
    } else {
        x_ult = 1E5;
    }
    this->x_ult = x_ult;
    this->x1 = tempx1;
    this->x2 = tempx2;
    this->y1 = tempy1;
    this->y2 = tempy2;
}

bool TriBackbone::inRange(double x, double low, double high)
{
    if (x < low)
        return false;
    else if (x > high)
        return false;
    else
        return true;
}
