#include "matCFST.h"
#include "tribackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include <iostream>
#include <vector>

matCFST::matCFST(double E, double f1, double f2, double b1, double b2, double revRatio)
{
    this->E_ini = E;
    this->f1 = f1;
    this->f2 = f2;
    this->b1 = b1;
    this->b2 = b2;
    this->revRatio = revRatio;
    
    this->E = this->E_ini;
    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    this->nextCondition = 0;
    
    this->initial();
}

matCFST::matCFST() {}

matCFST::~matCFST() {}

void matCFST::nextStress(double strain)
{
    double stress, tangent;
    if (this->BB.isOnPath) {
        if (this->BB.isLeave(strain)) {
            //update LP
            this->LP_next = this->BB.unload(this->BB.curX, this->BB.curY, this->E_ini, this->f1 * this->f2);
            stress = this->LP_next.getY(strain);
            tangent = this->LP_next.getE(strain);
            this->nextCondition = 3;
        } else {
            //stay on BB
            stress = this->BB.getY(strain);
            tangent = this->BB.getE(strain);
            this->nextCondition = 0;
        }
    } else if (this->LP.isOnPath) {
        if (this->LP.isInRange(strain)) {
            if (this->LP.isLeave(strain)) {
                //update LP
                this->LP_next = this->LP.unload(this->LP.curX, this->LP.curY, this->E_ini, this->f1 * this->f2);
                stress = this->LP_next.getY(strain);
                tangent = this->LP_next.getE(strain);
                this->nextCondition = 3;
            } else {
                //stay on the LP
                stress = this->LP.getY(strain);
                tangent = this->LP.getE(strain);
                this->nextCondition = 1;
            }
        } else {
            //Back to the BB
            stress = this->BB.getY(strain);
            tangent = this->BB.getE(strain);
            this->nextCondition = 2;
        }
    }

    std::cout<<this->nextCondition<<std::endl;
    this->strain_next = strain;
    this->stress_next = stress;
    this->E_next = tangent;
}

void matCFST::next()
{
    if (this->nextCondition == 0) {
        //stay on BB
        this->BB.back2Path(this->strain_next);
    } else if (this->nextCondition == 1) {
        //stay on LP
        this->LP.back2Path(this->strain_next);
    } else if (this->nextCondition == 2) {
        //back to BB
        this->BB.back2Path(this->strain_next);
        this->LP.isOnPath = false;
    } else if (this->nextCondition == 3) {
        //update LP
        this->LP = this->LP_next;
        this->LP.back2Path(this->strain_next);
        this->BB.isOnPath = false;
    }
    this->getDataFromPath();
}

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
    this->BB.back2Path(this->strain);
    this->getDataFromPath();
}

void matCFST::getDataFromPath()
{
    if (this->BB.isOnPath) {
        this->strain = this->BB.curX;
        this->stress = this->BB.curY;
        this->E = this->BB.curE();
    } else if (this->LP.isOnPath) {
        this->strain = this->LP.curX;
        this->stress = this->LP.curY;
        this->E = this->LP.curE();
    }
}
