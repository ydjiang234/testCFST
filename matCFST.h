#ifndef MATCFST_H
#define MATCFST_H
#include <iostream>
#include <vector>
#include "tribackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include "generalPath.h"

class matCFST
{
public:
    matCFST(double E, double f1, double f2, double b1, double b2, double revRatio);
    matCFST();
    ~matCFST();

    void nextStress(double strain_next);
    void next();

    double E_ini, f1, f2, b1, b2, revRatio;
    double E, strain, stress;
    double E_next, strain_next, stress_next;
    unsigned int nextCondition;//0 - stay on BB, 1 - stay on LP, 2 - back to BB, 3 - update LP
    TriBackbone BB;
    unLoadPath1 LP1;
    unLoadPath2 LP2;
    
private:
    void initial();
    void getDataFromPath();
};

#endif
