#ifndef CLIPPER_H
#define CLIPPER_H
#include <windows.h>
#include <vector>
#include"Point.h"
using namespace std;
class Clipper
{
private:
public:
    int xLeft, xRight, yTop, yBottom;
    vector<Point> polygonPoints;
    Clipper();
    Clipper(int, int, int, int);
/// draw the window
    void drawClipper(HDC hdc, COLORREF c);
    void drawClipper(HDC hdc, int, int, int, int, COLORREF c);
/// for clipping then drawing
    int code(double x, double y);
    void clipLine(HDC hdc, double x1, double y1, double x2, double y2, COLORREF);
    void clipPolygonByLines(HDC hdc, vector<Point> points, COLORREF);
	
	vector<point> clipLeft(vector<point> polygon, int xLeft);
	vector<point> clipRight(vector<point> polygon, int xRight);
	vector<point> clipTop(vector<point> polygon, int yTop);
	vector<point> clipBottom(vector<point> polygon, int yBottom);/// clip side by side
	void clipPolygonBySides(HDC hdc, vector<point> polygon);

	
    void drawStoredData(HDC, COLORREF);
};
#endif // CLIPPER_H



