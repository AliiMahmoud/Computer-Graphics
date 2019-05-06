#include "Ellipsee.h"
#include<algorithm>
using namespace std;
Ellipsee::Ellipsee()
{
    //ctor
}

void Ellipsee::drawEllipseDefault(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{

    this-> a = a;
    this-> b = b;
    this-> xc = xc;
    this-> yc = yc;

    double y = 0;
    double x = a;
    drawFourPoints(hdc, xc, yc, a, 0, c);
    while(x >= 0)
    {
        x-=0.1;
        y = abs(b * sqrt(1 - ((x/a) * (x/a))));
        drawFourPoints(hdc, xc, yc, round(x), round(y), WHITE_BRUSH);
    }
}


void Ellipsee::drawFourPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc,xc + x, yc + y,c);
    SetPixel(hdc,xc - x, yc + y,c);
    SetPixel(hdc,xc + x, yc - y,c);
    SetPixel(hdc,xc - x, yc - y,c);
}



void Ellipsee::drawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{
    this-> a = a;
    this-> b = b;
    this-> xc = xc;
    this-> yc = yc;
    int x, y, x1, y1;
    double theta;
    float p = 3.14159/180; /// convert to degree so that every increment is equivalent to angle
    for(theta = 0; theta <= 90; theta += .25)
    {
        x1 = a * cos(theta * p);
        y1 = b * sin(theta * p);
        x = round(x1);
        y = round(y1);
        drawFourPoints(hdc, xc, yc, x, y, c);
    }
}

void Ellipsee::drawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{

    this-> a = a;
    this-> b = b;
    this-> xc = xc;
    this-> yc = yc;

    /// region 1
    int x = 0, y = b;
    drawFourPoints(hdc, xc, yc, x, y, c);
    int a2 = a * a;
    int b2 = b * b;
    int d = (b2) - (a2 * b) + round(a2 / 4.0);
    int d1 = (3 * b2) - (2 * b * a2) + (2 * a2);
    int d2 = (3 * b2);
    int delta1 = 2 * b2 + 2 * a2;
    int delta2 = 2 * b2;
    while(d1 < a * a + b * b)
    {
        if(d > 0)
            y--, d += d1, d1 += delta1, d2 += delta2;
        else
            d += d2, d1 += delta2, d2 += delta2;
        x++;
        drawFourPoints(hdc, xc, yc, x, y, c);
    }
    ///region 2
    d1 = -2 * a2 * y + 3 * a2;
    d2 = 2 * b2 * x + 2 + d1;
    delta1 = 2 * a2;
    delta2 = 2 * (b2 + a2);
    while(y >= 0)
    {
        if(d > 0)
            d += d1, d1 += delta1, d2 += delta1;
        else
            x++, d += d2, d1 += delta1, d2 += delta2;
        y--;
        drawFourPoints(hdc, xc, yc, x, y, c);
    }
}



void Ellipsee::drawEllipseDirectly(HDC hdc, int xc, int yc, int a, int b, COLORREF c)
{
    this-> a = a;
    this-> b = b;
    this-> xc = xc;
    this-> yc = yc;
    Ellipse(hdc, xc, yc, a, b);
}

void Ellipsee::drawStoredData(HDC hdc, COLORREF c)
{
    drawEllipseMidpoint(hdc, this->xc, this->yc, this->a, this->b, c);
}
