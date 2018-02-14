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
            //11 - go to new LP1
            this->nextCondition = 11;
            this->LP1_next = this->BB.unload(this->BB.curX, this->BB.curY, this->E_ini, this->revRatio * this->f2);
            stress = this->LP1_next.getY(strain);
            tangent = this->LP1_next.getE(strain);
        } else {
            //10 - stay on BB
            this->nextCondition = 10;
            stress = this->BB.getY(strain);
            tangent = this->BB.getE(strain);
        }
    } else if (this->LP1.isOnPath) {
        if (this->LP1.isInRange(strain)) {
            if (this->LP1.isLeave(strain)) {
                //21 - go to new LP2
                this->nextCondition = 21;
                this->LP2_next = this->LP1.unload(this->LP1.curX, this->LP1.curY, this->E_ini, this->revRatio * this->f2);
                stress = this->LP2_next.getY(strain);
                tangent = this->LP2_next.getE(strain);
            } else {
                //20 - stay on LP1
                this->nextCondition = 20;
                stress = this->LP1.getY(strain);
                tangent = this->LP1.getE(strain);
            }
        } else {
            //22 - back to BB
            this->nextCondition = 22;
            stress = this->BB.getY(strain);
            tangent = this->BB.getE(strain);
        }

    } else if (this->LP2.isOnPath) {
        if (this->LP2.isInRange(strain)) {
            if (this->LP2.isLeave(strain)) {
                //31 - go to new LP2
                this->nextCondition = 31;
                this->LP2_next = this->LP2.unload(this->LP2.curX, this->LP2.curY, this->E_ini, this->revRatio * this->f2);
                stress = this->LP2_next.getY(strain);
                tangent = this->LP2_next.getE(strain);
            } else {
                //30 - stay on LP2
                this->nextCondition = 30;
                stress = this->LP2.getY(strain);
                tangent = this->LP2.getE(strain);
            }
        } else {
            //32 - back to BB
            this->nextCondition = 32;
            stress = this->BB.getY(strain);
            tangent = this->BB.getE(strain);
        }
    }

    this->strain_next = strain;
    this->stress_next = stress;
    this->E_next = tangent;
}

void matCFST::next()
{
    if (this->nextCondition == 10) {
        //10 - stay on BB
        this->BB.back2Path(this->strain_next);
        this->LP1.isOnPath = false;
        this->LP2.isOnPath = false;
    } else if (this->nextCondition = 11) {
        //11 - go to new LP1
        this->LP1 = this->LP1_next;
        this->LP1.back2Path(this->strain_next);
        this->BB.isOnPath = false;
        this->LP2.isOnPath = false;
    } else if (this->nextCondition = 20) {
        //20 - stay on LP1
        this->LP1.back2Path(this->strain_next);
        this->BB.isOnPath = false;
        this->LP2.isOnPath = false;
    } else if (this->nextCondition = 21) {
        //21 - go to new LP2
        this->LP2 = this->LP2_next;
        this->LP2.back2Path(this->strain_next);
        this->BB.isOnPath = false;
        this->LP1.isOnPath = false;
    } else if (this->nextCondition = 22) {
        //22 - back to BB
        this->BB.back2Path(this->strain_next);
        this->LP1.isOnPath = false;
        this->LP2.isOnPath = false;
    } else if (this->nextCondition = 30) {
        //30 - stay on LP2
        this->LP2.back2Path(this->strain_next);
        this->BB.isOnPath = false;
        this->LP2.isOnPath = false;
    } else if (this->nextCondition = 31) {
        //31 - go to new LP2
        this->LP2 = this->LP2_next;
        this->LP2.back2Path(this->strain_next);
        this->BB.isOnPath = false;
        this->LP1.isOnPath = false;
    } else if (this->nextCondition = 32) {
        //32 - back to BB
        this->BB.back2Path(this->strain_next);
        this->LP1.isOnPath = false;
        this->LP2.isOnPath = false;
    }
    this->getDataFromPath();
}

void matCFST::revertToLast()
{
    this->E_next = this->E;
    this->strain_next = strain;
    this->stress_next = stress;
    this->nextCondition = 0;
}

void matCFST::reset()
{
    this->E = this->E_ini;
    this->E_next = this->E;
    this->strain = 0;
    this->strain_next = 0;
    this->stress = 0;
    this->stress_next = 0;
    this->nextCondition = 0;

    this->initial();
}

void matCFST::initial()
{
    double tempx1, tempx2;
    double tempy1, tempy2;
    double x_ult, y_ult;
    tempx1 = this->f1 / this->E_ini;
    tempy1 = this->f1;
    tempx2 = tempx1 + (this->f2 - this->f1) / (this->b1 * this->E_ini);
    tempy2 = this->f2;
    if (this->b2 < 0)
    {
        x_ult = tempx2 - this->f2 / (this->b2 * this->E_ini);
        y_ult = 0;
    } else {
        x_ult = 1E5;
        y_ult = this->f2 + (x_ult - tempx2) * this->b2 * this->E_ini;
    }
    
    std::vector<double> xdata = {-x_ult, -tempx2, -tempx1, tempx1, tempx2, x_ult};
    std::vector<double> ydata = {-y_ult, -tempy2, -tempy1, tempy1, tempy2, y_ult};
    
    this->BB =TriBackbone(xdata, ydata);
    this->BB.back2Path(this->strain);
    this->getDataFromPath();
}

void matCFST::getDataFromPath()
{
    this->nextCondition = 0;
    if (this->BB.isOnPath) {
        this->strain = this->BB.curX;
        this->stress = this->BB.curY;
        this->E = this->BB.curE();
    } else if (this->LP1.isOnPath) {
        this->strain = this->LP1.curX;
        this->stress = this->LP1.curY;
        this->E = this->LP1.curE();
    } else if (this->LP2.isOnPath) {
        this->strain = this->LP2.curX;
        this->stress = this->LP2.curY;
        this->E = this->LP2.curE();
    }
}
