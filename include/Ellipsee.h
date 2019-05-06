#ifndef ELLIPSEE_H
#define ELLIPSEE_H
#include<windows.h>
class Ellipsee
{
private:
    int xc, yc, a, b;
public:
    Ellipsee();
    void drawEllipseDirectly(HDC hdc, int xc, int yc, int a, int b, COLORREF c);
    void drawEllipsePolar(HDC hdc, int xc, int yc, int a, int b, COLORREF c);
    void drawEllipseMidpoint(HDC hdc, int xc, int yc, int a, int b, COLORREF c);
    void drawFourPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
    void drawEllipseDefault(HDC hdc, int xc, int yc, int a, int b, COLORREF c);

    void drawStoredData(HDC, COLORREF);
};
#endif // ELLIPSEE_H

