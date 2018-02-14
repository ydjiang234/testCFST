#include <elementAPI.h>
#include "testCFST.h"
#include <Vector.h>
#include <Channel.h>
#include <math.h>
#include <float.h>
#include "matCFST.h"

//#ifdef _USRDLL
#define OPS_Export extern "C" _declspec(dllexport)
//#elif _MACOSX
//#define OPS_Export extern "C" __attribute__((visibility("default")))
//#else
//#define OPS_Export extern "C"
//#endif


static int numtestCFST = 0;

OPS_Export void *OPS_testCFST()
{
    // print out some KUDO's
    if (numtestCFST == 0) {
        opserr << "testCFST unaxial material - Written by Yadong Jiang\n";
        numtestCFST = 1;
    }

    // Pointer to a uniaxial material that will be returned
    UniaxialMaterial *theMaterial = 0;

    //
    // parse the input line for the material parameters
    //

    int    iData[1];
    double dData[6];
    int numData;
    numData = 1;
    if (OPS_GetIntInput(&numData, iData) != 0) {
        opserr << "WARNING invalid uniaxialMaterial testCFST tag" << endln;
        return 0;
    }

    numData = 6;
    if (OPS_GetDoubleInput(&numData, dData) != 0) {
        opserr << "WARNING invalid E & f1 & f2 & b1 & b2 & revRatio\n";
        return 0;
    }

    //
    // create a new material
    //

    theMaterial = new testCFST(iData[0], dData[0], dData[1], dData[2], dData[3], dData[4], dData[5]);

    if (theMaterial == 0) {
        opserr << "WARNING could not create uniaxialMaterial of type testCFST\n";
        return 0;
    }

    // return the material
    return theMaterial;
}


testCFST::testCFST(int tag, double E, double f1, double f2, double b1, double b2, double revRatio):UniaxialMaterial(tag, 0)
{
    this->curMat = matCFST(E, f1, f2, b1, b2, revRatio);
}


testCFST::testCFST() :UniaxialMaterial(0, 0)
{
}

testCFST::~testCFST()
{
    //do nothing here
}

int testCFST::setTrialStrain(double strain, double strainRate)
{
    if (fabs(strain - this->curMat.strain) < DBL_EPSILON)
    {
        return 0;
    } else {
        this->curMat.nextStress(strain);
        return 0;
    }
}

double testCFST::getStrain(void)
{
    return this->curMat.strain_next;
}

double testCFST::getStress(void)
{
    return this->curMat.stress_next;
}

double testCFST::getTangent(void)
{
    return this->curMat.E_next;
}

double testCFST::getInitialTangent(void)
{
    return this->curMat.E_ini;
}

int testCFST::commitState(void)
{
    this->curMat.next();
    return 0;
}

int testCFST::revertToLastCommit(void)
{

    this->curMat.revertToLast();
    return 0;
}

int testCFST::revertToStart(void)
{
    this->curMat.reset();
    return 0;
}

UniaxialMaterial *testCFST::getCopy(void)
{
    testCFST *theCopy = new testCFST(this->getTag(), this->curMat.E_ini, this->curMat.f1, this->curMat.f2, this->curMat.b1, this->curMat.b2, this->curMat.revRatio);
    theCopy->curMat = this->curMat;
    //opserr << this->i << endln;
    return theCopy;
}

int testCFST::sendSelf(int commitTag, Channel &theChannel)
{
    int res = 0;
    /*
    static Vector data(2);
    data(0) = this->getTag();
    data(1) = this->curMat;
    res = theChannel.sendVector(this->getDbTag(), commitTag, data);
    if (res < 0)
        opserr << "testCFST::sendSelf() - failed to send data\n";
    */
    return res;
}
int testCFST::recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker)
{
    int res = 0;
    /*
    static Vector data(2);
    res = theChannel.recvVector(this->getDbTag(), commitTag, data);
    if (res < 0) {
        opserr << "testCFST::recvSelf() - failed to receive data\n";
    }
    else {
        this->setTag(data(0));
        this->curMat = data(1);
    }
    */
    return res;
}

void testCFST::Print(OPS_Stream &s, int flag)
{
    s << "testCFST tag: " << this->getTag() << endln;
}



