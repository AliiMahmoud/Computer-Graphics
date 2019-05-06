#include "Curve.h"
#include<bits/stdc++.h>
using namespace std;
Curve::Curve()
{
    u1 = v1 = u2 = v2 = 0;
}

void Curve::copyValues(vector<Point> points)
{
    curvePoints.clear();
    for(Point p : points)
        curvePoints.push_back(p);
}

void Curve::drawCurveBezier(HDC hdc, vector<Point> points, COLORREF c)
{

//    * information to be stored *
    copyValues(points);
    type = 2;

    double x, y;
    for(double t = 0; t <= 1; t += .0001)
    {
//               x = pow(1 - t, 3) * points[0].x + 3 * t * pow(1 - t, 2) * points[1].x + 3 * pow(t, 2) * (1 - t) * points[2].x + pow(t, 3) * points[3].x;
        x = points[0].x
            + 3 * (points[1].x - points[0].x) * t
            + (3 * points[0].x - 6 * points[1].x + 3 * points[2].x) * pow(t, 2)
            + (-1 * points[0].x + 3 * points[1].x - 3 * points[2].x + points[3].x)* pow(t, 3);

        y = points[0].y
            + 3 * (points[1].y - points[0].y) * t
            + (3 * points[0].y - 6 * points[1].y + 3 * points[2].y) * pow(t, 2)
            + (-1 * points[0].y + 3 * points[1].y - 3 * points[2].y + points[3].y)* pow(t, 3);

        SetPixel(hdc, round(x), round(y), c);
    }
}
void Curve::drawCurveHermite(HDC hdc, Point p1, Point p2, int u1, int v1, int u2, int v2, COLORREF c)
{
//    * information to be stored *
    curvePoints.clear();
    curvePoints.push_back(p1);
    curvePoints.push_back(p2);
    this->u1 = u1;
    this->v1 = v1;
    this->u2 = u2;
    this->v2 = v2;
    type = 3;

    double x, y;
    for(double t = 0; t <= 1; t += .0001)
    {
        x = p1.x
            + (u1) * t
            + (-3 * p1.x - 2 * u1 + 3 * p2.x - u2) * pow(t, 2)
            + ( 2 * p1.x + 1 * u1 - 2 * p2.x + u2) * pow(t, 3);
        y = p1.y
            + (v1) * t
            + (-3 * p1.y - 2 * v1 + 3 * p2.y - v2) * pow(t, 2)
            + ( 2 * p1.y + 1 * v1 - 2 * p2.y + v2) * pow(t, 3);

        SetPixel(hdc, int(x), int(y), c);
    }
}
void Curve::drawQuadraticCurve(HDC hdc, vector<Point> points, COLORREF c)
{

//    * information to be stored *
    copyValues(points);
    type = 1;

    double x, y;
    for(double t = 0; t <= 1; t += .0001)
    {
        x = points[0].x
            + (-3 * points[0].x + 4 * points[1].x - points[2].x) * t
            + (2 * points[0].x - 4 * points[1].x + 2 * points[2].x) * pow(t, 2);
        y = points[0].y
            + (-3 * points[0].y + 4 * points[1].y - points[2].y) * t
            + (2 * points[0].y - 4 * points[1].y + 2 * points[2].y) * pow(t, 2);
        SetPixel(hdc, round(x), round(y), c);
    }
}

void Curve::drawStoredData(HDC hdc, COLORREF c)
{
    if(type == 1)
    {
        drawQuadraticCurve(hdc, this->curvePoints, c);
    }
    if(type == 2)
    {

        drawCurveBezier(hdc, this->curvePoints, c);
    }
    if(type == 3)
    {
        drawCurveHermite(hdc, this->curvePoints[0], this->curvePoints[1],
                    this-> u1, this-> v1, this-> u2, this-> v2, c);
    }
}

