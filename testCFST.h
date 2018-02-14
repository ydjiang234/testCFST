#ifndef testCFST_h
#define testCFST_h


#include <UniaxialMaterial.h>
#include "matCFST.h"

class testCFST : public UniaxialMaterial
{
public:
    testCFST(int tag, double E, double f1, double f2, double b1, double b2, double revRatio);
    testCFST();
    ~testCFST();
    UniaxialMaterial *getCopy(void);

    int setTrialStrain(double strain, double strainRate = 0);

    double getStrain(void);
    double getStress(void);
    double getTangent(void);
    double getInitialTangent(void);

    int commitState(void);
    int revertToLastCommit(void);
    int revertToStart(void);

    int sendSelf(int commitTag, Channel &theChannel);
    int recvSelf(int commitTag, Channel &theChannel, FEM_ObjectBroker &theBroker);

    void Print(OPS_Stream &s, int flag = 0);



protected:

private:
    matCFST curMat;

};



#endif // !testCFST_h
