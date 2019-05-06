#ifndef CURVE_H
#define CURVE_H
#include"Point.h"
#include<windows.h>
#include<vector>
#include<math.h>
using namespace std;
class Curve
{
public:
    int u1, v1, u2, v2;
    vector<Point> curvePoints;
    int type; // 1 for quadratic 2 for Bezier ...
    Curve();
    void drawQuadraticCurve(HDC hdc, vector<Point> points, COLORREF); ///  3 points
    void drawCurveBezier(HDC hdc, vector<Point> points, COLORREF);    /// 2 points and 2 guides
    void drawCurveHermite(HDC hdc, Point p1, Point p2, int u1, int v1, int u2, int v2, COLORREF); /// passes through 2 points
    void copyValues(vector<Point> points);
    void drawStoredData(HDC, COLORREF);

};

#endif // CURVE_H
