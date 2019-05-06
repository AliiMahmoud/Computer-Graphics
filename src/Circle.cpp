#include "Circle.h"
#include<math.h>
using namespace std;
Circle::Circle()
{

}

void Circle::drawCirclePolar(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    int x, y;
    for(double theta = 0; theta <= 6.28; theta += .007)
    {
        x = round(xc + r * cos(theta));
        y = round(yc + r * sin(theta));
        SetPixel(hdc, x, y, c);
    }
}

void Circle::drawCircleIterativePolar(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    double theta = 1.0 / r;
    double cosTheta = cos(theta);
    double sinTheta = sin(theta);
    double x = r, y = 0;
    drawEightPoints(hdc, xc, yc, r, 0, c);
    while(x > y)
    {
        double x1 = x * cosTheta - y * sinTheta;
        y = x * sinTheta + y * cosTheta;
        x = x1;
        drawEightPoints(hdc, xc, yc, round(x), round(y), c);
    }
}

void Circle::drawCircleIterativeCartesian(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    /// |slope| > 1
    int x = r, y = 0;
    drawEightPoints(hdc, xc, yc, r, 0, c);
    while(x > y)
    {
        y++;
        x = round(sqrt(r * r - y * y));
        drawEightPoints(hdc, xc, yc, x, y, c);
    }
}

void Circle::drawCircleMidpoint(HDC hdc, int xc, int yc, int r, COLORREF c)
{
    int x = 0;
    int y = r;
    int d = 1 - r;
    int d1 = 2 * (x - y) + 5;
    int d2 = 2 * (x) + 3;
    drawEightPoints(hdc, xc, yc, x, y, c);
    while(x < y)
    {
        if(d > 0)
            x++, y--, d += d1, d1 += 4, d2 += 2;
        else
            x++, d += d2, d1 += 2, d2 += 2;
        drawEightPoints(hdc, xc, yc, x, y, c);
    }
}

void Circle::drawEightPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc,xc + x, yc + y,c);
    SetPixel(hdc,xc - x, yc + y,c);
    SetPixel(hdc,xc + x, yc - y,c);
    SetPixel(hdc,xc - x, yc - y,c);
    SetPixel(hdc,xc + y, yc + x,c);
    SetPixel(hdc,xc - y, yc + x,c);
    SetPixel(hdc,xc + y, yc - x,c);
    SetPixel(hdc,xc - y, yc - x,c);
}
