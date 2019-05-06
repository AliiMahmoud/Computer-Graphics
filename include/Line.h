#ifndef LINE_H
#define LINE_H
#include <windows.h>
class Line
{
private:
    int x1, y1, x2, y2;
public:
    Line();
    void drawLineParametric(HDC hdc, int xs, int ys, int xe, int ye, COLORREF c);
    void drawLineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
    void drawLineMidPoint(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c);
    void drawStoredData(HDC, COLORREF);
    void print();
};

#endif // LINE_H
