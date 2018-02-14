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
    void revertToLast();
    void reset();


    double E_ini, f1, f2, b1, b2, revRatio;
    double E, strain, stress;
    double E_next, strain_next, stress_next;


    
private:
    void initial();
    void getDataFromPath();
    unsigned int nextCondition;
    TriBackbone BB;//10 - stay on BB, 11 - go to LP1
    unLoadPath1 LP1, LP1_next; // 20 stay on LP1, 21 - go to LP2, 22 - back to BB
    unLoadPath2 LP2, LP2_next; // 30 stay on LP2, 31 - go to new LP2, 32 - back to BB
};

#endif
