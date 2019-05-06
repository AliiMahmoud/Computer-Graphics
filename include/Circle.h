#ifndef CIRCLE_H
#define CIRCLE_H
#include<windows.h>
class Circle
{
public:

    Circle();
    static void drawCirclePolar(HDC hdc, int xc, int yc, int r, COLORREF c);
    static void drawCircleIterativePolar(HDC hdc, int xc, int yc, int r, COLORREF c);
    static void drawCircleIterativeCartesian(HDC hdc, int xc, int yc, int r, COLORREF c);
    static void drawCircleMidpoint(HDC hdc, int xc, int yc, int r, COLORREF c);
    static void drawEightPoints(HDC hdc, int xc, int yc, int x, int y, COLORREF c);
};

#endif // CIRCLE_H
