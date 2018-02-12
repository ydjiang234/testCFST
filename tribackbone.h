#ifndef TRIBACKBONE_H
#define TRIBACKBONE_H


class TriBackbone
{
public:
    TriBackbone(double E, double f1, double f2, double b1, double b2);
    ~TriBackbone();
    double getY(double x);
    bool isLinear(double x);
    bool isInRange(double x);

private:
    //to get the points on the backbone
    void initial();
    bool inRange(double x, double low, double high);


    double E, f1, f2, b1, b2;
    double x1, x2, x_ult;
    double y1, y2;

};

#endif // TRIBACKBONE_H
