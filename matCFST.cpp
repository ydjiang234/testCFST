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
    this->nextCondition = -1;
    
    this->initial();
}

matCFST::matCFST() {}

matCFST::~matCFST() {}

void matCFST::nextStress(double strain)
{
    generalPath *curLP, *nextLP;
    curLP = this->getCurLP();

    this->nextCondition = curLP->isLeave(strain);
    if (this->nextCondition == 0) {
        //0 - stay on path
        nextLP = curLP;
    } else if (this->nextCondition == 1) {
        //1 - new LP1
        this->LP1_next = this->BB.unload(curLP->curX, curLP->curY, this->E_ini, this->f1 * this->revRatio);
        nextLP = &this->LP1_next;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        this->LP2_next = this->LP1.unload(curLP->curX, curLP->curY, this->E_ini, this->f1 * this->revRatio);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        this->LP2_next = this->LP2.unload(curLP->curX, curLP->curY, this->E_ini, this->f1 * this->revRatio);
        nextLP = &this->LP2_next;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        nextLP = &this->BB;
    }

    this->strain_next = strain;
    this->stress_next = nextLP->getY(strain);
    this->E_next = nextLP->getE(strain);
    //std::cout<<this->nextCondition<<std::endl;
    //std::cout<<this->strain_next<<std::endl;
}

void matCFST::next()
{
    generalPath *curLP;
    if (this->BB.isOnPath) {
        curLP = &this->BB;
    } else if (this->LP1.isOnPath) {
        curLP = &this->LP1;
    } else if (this->LP2.isOnPath) {
        curLP = &this->LP2;
    }

    if (this->nextCondition == 1) {
        //1 - new LP1
        this->LP1 = this->LP1_next;
        curLP = &this->LP1;
    } else if (this->nextCondition == 21) {
        //21 - new LP2 from LP1
        this->LP2 = this->LP2_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 22) {
        //22 - new LP2 from LP2
        this->LP2 = this->LP2_next;
        curLP = &this->LP2;
    } else if (this->nextCondition == 3) {
        //3 - back to BB
        curLP = &this->BB;
    }

    this->BB.isOnPath = false;
    this->LP1.isOnPath = false;
    this->LP2.isOnPath = false;
    curLP->back2Path(this->strain_next);

    this->getDataFromPath();
}

void matCFST::revertToLast()
{
    this->E_next = this->E;
    this->strain_next = this->strain;
    this->stress_next = this->stress;
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
    this->LP1 = unLoadPath1();
    this->LP2 = unLoadPath2();
    this->BB = TriBackbone();
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
    generalPath *curLP = this->getCurLP();
    this->strain = curLP->curX;
    this->stress = curLP->curY;
    this->E = curLP->curE();
}

generalPath* matCFST::getCurLP()
{
    generalPath *curLP;
    if (this->BB.isOnPath) {
        curLP = &this->BB;
    } else if (this->LP1.isOnPath) {
        curLP = &this->LP1;
    } else if (this->LP2.isOnPath) {
        curLP = &this->LP2;
    }
    return curLP;
}
