#include "matCFST.h"
#include "tribackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include <iostream>
#include <vector>

matCFST::matCFST(double E, double f1, double f2, double b1, double b2)
{
    this->E = E;
    this->f1 = f1;
    this->f2 = f2;
    this->b1 = b1;
    this->b2 = b2;
    
    this->E_ini = this->E;
    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    
    this->initial();
}

matCFST::matCFST() {}

matCFST::~matCFST() {}

void matCFST::initial()
{
    double tempx1, tempx2;
    double tempy1, tempy2;
    double x_ult, y_ult;
    tempx1 = this->f1 / this->E;
    tempy1 = this->f1;
    tempx2 = tempx1 + (this->f2 - this->f1) / (this->b1 * this->E);
    tempy2 = this->f2;
    if (this->b2 < 0)
    {
        x_ult = tempx2 - this->f2 / (this->b2 * this->E);
        y_ult = 0;
    } else {
        x_ult = 1E5;
        y_ult = this->f2 + (x_ult - tempx2) * this->b2 * this->E;
    }
    
    std::vector<double> xdata = {-x_ult, -tempx2, -tempx1, tempx1, tempx2, x_ult};
    std::vector<double> ydata = {-y_ult, -tempy2, -tempy1, tempy1, tempy2, y_ult};
    
    this->BB =TriBackbone(xdata, ydata);
}