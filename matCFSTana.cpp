#include "matCFSTana.h"

#include "matCFST.h"
#include <iostream>
#include <vector>

matCFSTana::matCFSTana(double E, double f1, double f2, double b1, double b2, double revRatio) : matCFST(E, f1, f2, b1, b2, revRatio) {}

matCFSTana::matCFSTana() {}

matCFSTana::~matCFSTana() {}

void matCFSTana::analysis(std::vector<double> loadingProtocol, double interval)
{
    this->reset();
    this->loadingProtocol = loadingProtocol;
    for (int i=0; i<this->loadingProtocol.size(); ++i) {
        this->analysisOneStep(this->loadingProtocol[i], interval);
    }
}

void matCFSTana::analysisOneStep(double target, double interval)
{
    if (this->strain <= target) {
        while (this->strain < target) {
            this->nextStress(this->strain + interval);
            this->next();
            this->Xresults.push_back(this->strain);
            this->Yresults.push_back(this->stress);
        }
    } else {
        while (this->strain > target) {
            this->nextStress(this->strain - interval);
            this->next();
            this->Xresults.push_back(this->strain);
            this->Yresults.push_back(this->stress);
        }
    }
}

void matCFSTana::reset()
{
    this->Xresults.clear();
    this->Yresults.clear();
    this->loadingProtocol.clear();
}
