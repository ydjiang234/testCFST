#ifndef MATCFST_H
#define MATCFST_H
#include <iostream>
#include <vector>
#include "tribackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"

class matCFST
{
public:
    matCFST(double E, double f1, double f2, double b1, double b2);
    matCFST();
    ~matCFST();
    double E, f1, f2, b1, b2;
    double E_ini, strain, stress;
    double E_next, strain_next, stress_next;
    TriBackbone BB;
    unLoadPath1 LP1();
    unLoadPath2 LP2();
    
private:
    void initial();
    
};

#endif