#include "Line.h"
#include<bits/stdc++.h>
using namespace std;
Line::Line()
{
    //ctor
}
void Line::drawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c)
{
    this -> x1 = xs;
    this -> y1 = ys;
    this -> x2 = xe;
    this -> x2 = xe;
    int x, y;
    int dx = (xe - xs);
    int dy = (ye - ys);
    for(double t = 0; t <= 1; t += .001)
    {
        x = round(xs + t * dx);
        y = round(ys + t * dy);
        SetPixel(hdc, x, y, c);
    }
}

void Line::drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    this -> x1 = x1;
    this -> y1 = y1;
    this -> x2 = x2;
    this -> y2 = y2;
    int dx = x2 - x1, dy = y2 - y1;
    double m = (double)(dy) / dx;
    /// slope from -1 to 1
    /// |m| >= 1
    if(abs(dx) >= abs(dy))
    {
        if(x1 > x2)
            swap(x1, x2), swap(y1, y2);
        int x = x1;
        double y = y1;
        while(x < x2)
        {
            SetPixel(hdc, x, round(y), c);
            x++;
            y += m;
        }
    }
    else
    {
        if(y1 > y2)
            swap(x1, x2), swap(y1, y2);
        int y = y1;
        double x = x1;
        double mInverse(1.0 / m);
        while(y < y2)
        {
            SetPixel(hdc, round(x), y, c);
            y++;
            x += mInverse;
        }
    }
}
void Line::drawLineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{

    this -> x1 = x1;
    this -> y1 = y1;
    this -> x2 = x2;
    this -> y2 = y2;
    int dx = (x2 - x1);
    int dy = (y2 - y1);
    if(((double)dy / dx) >= 0 &&((double)dy / dx) <= 1)
    {
        if(x1 > x2)
            swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
        int x = x1, y = y1;
        int d = dx - 2 * dy;
        int d1 = -2 * dy;
        int d2 = 2 * dx - 2 * dy;
        SetPixel(hdc, x, y, c);
        while(x < x2)
        {
            if(d < 0)
                y++, x++, d += d2;
            else
                x++, d += d1;
            SetPixel(hdc, x, y, c);
        }
    }
    else if (((double)dy / dx) <= -1)
    {
        if(y1 > y2)
            swap(x1, x2), swap(y1, y2);
        else
            dx = -1 * dx, dy = -1 * dy;

        int x = x1, y = y1;
        int d = 2 * dx - dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx + 2 * dy;
        SetPixel(hdc, x, y, c);
        while(y < y2)
        {
            if(d < 0)
                y++, d += d1;
            else
                y++, x--, d += d2;
            SetPixel(hdc, x, y, c);
        }
    }
    else if (((double)dy / dx) > 1)
    {
        if(y1 > y2)
            swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
        int x = x1, y = y1;
        int d = dx - 2 * dy;
        int d1 = 2 * dx;
        int d2 = 2 * dx - 2 * dy;
        SetPixel(hdc, x, y, c);
        while(y < y2)
        {
            if(d < 0)
                y++, d += d1;
            else
                x++, y++, d += d2;
            SetPixel(hdc, x, y, c);
        }
    }
    ///   -1 to 0
    else
    {
        if(x1 > x2)
            swap(x1, x2), swap(y1, y2), dx = -1 * dx, dy = -1 * dy;
        int x = x1, y = y1;
        int d = dx + 2 * dy;
        int d1 = 2 * dy;
        int d2 = 2 * dx + 2 * dy;
        SetPixel(hdc, x, y, c);
        while(x < x2)
        {
            if(d > 0)
                x++, d += d1;
            else
                x++, y--, d += d2;
            SetPixel(hdc, x, y, c);
        }
    }
}

void Line::print()
{
    cout << x1 << " " << y1 << "        " << x2 << " " << y2 << endl;
}

void Line::drawStoredData(HDC hdc, COLORREF c)
{
    drawLineMidPoint(hdc, this->x1, this->y1, this->x2, this->y2, c);
}
