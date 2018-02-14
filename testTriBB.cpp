#include <iostream>
#include <vector>
#include "generalPath.h"
#include "tribackbone.h"
#include "unLoadPath1.h"
#include "unLoadPath2.h"
#include "matCFST.h"

using namespace std;

int main()
{
    /*
    vector<double> xdata = {-2, -1, 1, 2};
    vector<double> ydata = {-1.5, -1, 1, 1.5};
    generalPath *gp = new generalPath(xdata, ydata);
    */

    double E, f1, f2, b1, b2, revRatio;
    E = 1;
    f1 =1;
    f2 = 1.1;
    b1 =0.1;
    b2 =-0.1;
    revRatio = 0.1;
    
    matCFST mat(E, f1, f2, b1, b2, revRatio);
    mat.nextStress(1.5);
    cout<<mat.stress_next<<endl;
    cout<<mat.E_next<<endl;
    mat.next();
    mat.nextStress(-1.0);
    cout<<mat.stress_next<<endl;
    cout<<mat.E_next<<endl;
    mat.next();
    mat.nextStress(1.3);
    cout<<mat.stress_next<<endl;
    cout<<mat.E_next<<endl;
    mat.next();
    mat.nextStress(1.2);
    cout<<mat.stress_next<<endl;
    cout<<mat.E_next<<endl;
    mat.next();
    mat.nextStress(1.3);
    cout<<mat.stress_next<<endl;
    cout<<mat.E_next<<endl;

    cout<<"OK"<<endl;

    return 0;
}
