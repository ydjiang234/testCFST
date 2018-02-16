#ifndef MATCFSTANA_H
#define MATCFSTANA_H

#include "matCFST.h"
#include <iostream>
#include <vector>


class matCFSTana : public matCFST
{
public:
    matCFSTana(double E, double f1, double f2, double b1, double b2, double revRatio);
    matCFSTana();
    ~matCFSTana();

    void analysis(std::vector<double> loadingProtocol, double interval);

    std::vector<double> Xresults, Yresults;
    std::vector<double> loadingProtocol;

private:
    void analysisOneStep(double target, double interval);
    void reset();

};

#endif // MATCFSTANA_H
