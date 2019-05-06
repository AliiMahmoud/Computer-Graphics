#include "Clipper.h"
#include"Line.h"
Clipper::Clipper()
{
    xLeft = xRight = yTop = yBottom = 0;
}

void Clipper::drawClipper(HDC hdc, COLORREF c)
{
    Line l;
    l.drawLineMidPoint(hdc, xLeft, yTop, xRight, yTop, c);
    l.drawLineMidPoint(hdc, xLeft, yTop, xLeft, yBottom, c);
    l.drawLineMidPoint(hdc, xLeft, yBottom, xRight, yBottom, c);
    l.drawLineMidPoint(hdc, xRight, yTop, xRight, yBottom, c);
}
// draw using 2 points
void Clipper::drawClipper(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    xLeft = min(x1, x2);
    xRight = max(x1, x2);
    yTop = min(y1, y2);
    yBottom = max(y1, y2);
    drawClipper(hdc, c);
}

Clipper::Clipper(int xLeft, int xRight, int yTop, int yBottom)
{
    this-> xLeft = xLeft;
    this-> xRight = xRight;
    this-> yTop = yTop;
    this-> yBottom = yBottom;
}
int Clipper::code(double x, double y)
{
    int c = 0;
    if(x < xLeft)
        c += 1; /// 0001 in binary
    if(x > xRight)
        c += 2; /// 0010 in binary
    if(y < yTop)
        c += 4; /// 0100 in binary
    if(y > yBottom)
        c += 8; /// 1000 in binary
    return c;
}


void Clipper::clipPolygonByLines(HDC hdc, vector<Point> points, COLORREF c)
{
    polygonPoints.clear();
    for(int i = 0; i < (int)points.size(); ++i)
    {
        polygonPoints.push_back(points[i]);
        Point p1 = points[(i) % points.size()];
        Point p2 = points[(i + 1) % points.size()];
        clipLine(hdc, p1.x, p1.y, p2.x, p2.y, c);
    }
}

void Clipper::clipLine(HDC hdc, double x1, double y1, double x2, double y2, COLORREF color)
{
    int c1 = code(x1, y1);
    int c2 = code(x2, y2);
    while(true)
    {
        int out;
        double x, y;
        if(c1 == 0 && c2 == 0) /// inside the window
            break;
        else if(c1 & c2) /// if both in the same side
            return;
        if(c1 != 0) /// to pick on at a time
            out = c1;
        else if(c2 != 0)
            out = c2;
        if(out & 1) /// if exceeded the left -> get the intersection
            x = xLeft, y = ((y2 - y1) / (x2 - x1)) * (xLeft - x2) + y2;
        else if(out & 2)
            x = xRight,  y = ((y2 - y1) / (x2 - x1)) * (xRight - x1) + y1;
        else if(out & 4)
            y = yTop, x = ((x2 - x1) / (y2 - y1)) * (yTop - y1) + x1;
        else
            y = yBottom, x = ((x2 - x1) / (y2 - y1)) * (yBottom - y1) + x1;
        if(out == c1)
        {
            x1 = x;
            y1 = y;
            c1 = code(x1, y1);
        }
        else
        {
            x2 = x;
            y2 = y;
            c2 = code(x2, y2);
        }
    }
    Line l;
    l.drawLineMidPoint(hdc, x1, y1, x2, y2, color);
}

void Clipper::drawStoredData(HDC hdc, COLORREF c)
{
    drawClipper(hdc, c);
    clipPolygonByLines(hdc, polygonPoints, c);
}



vector<point> Clipper::clipLeft(vector<point> polygon, int xLeft)
{
    vector<point> res;
   for(int i = 0; i < (int)polygon.size() - 1; i += 2)
    {
        point p1 = polygon[i];
        point p2 = polygon[i + 1];
        if(p1.x < xLeft && p2.x < xLeft)
            continue;
        if(p1.x > xLeft && p2.x > xLeft)
        {
            res.push_back(p1);
            res.push_back(p2);
            continue;
        }
        if(p1.x > xLeft)
        {
            res.push_back(p1);
            point newOne;
            newOne.x = xleft;
            newOne.y = p1.y + (xleft - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
            res.push_back(newOne);
        }
        if(p2.x > xLeft)
        {
            point newOne;
            newOne.x = xleft;
            newOne.y = p1.y + (xleft - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
            res.push_back(newOne);
            res.push_back(p2);
        }
    }
    return res;
}
vector<point> Clipper::clipRight(vector<point> polygon, int xRight)
{
    vector<point> res;
    for(int i = 0; i < (int)polygon.size() - 1; i += 2)
    {
        point p1 = polygon[i];
        point p2 = polygon[i + 1];
        if(p1.x > xRight && p2.x > xRight)
            continue;
        if(p1.x < xRight && p2.x < xRight)
        {
            res.push_back(p1);
            res.push_back(p2);
            continue;
        }
        if(p1.x < xRight)
        {
            res.push_back(p1);
            point newOne;
            newOne.x = xRight;
            newOne.y = p1.y + (xRight - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
            res.push_back(newOne);
        }
        if(p2.x < xRight)
        {
            point newOne;
            newOne.x = xRight;
            newOne.y = p1.y + (xRight - p1.x) * (p2.y - p1.y) / (p2.x - p1.x);
            res.push_back(newOne);
            res.push_back(p2);
        }
    }
    return res;
}
vector<point> Clipper::clipTop(vector<point> polygon, int yTop)
{
    vector<point> res;
    for(int i = 0; i < (int)polygon.size() - 1; i += 2)
    {
        point p1 = polygon[i];
        point p2 = polygon[i + 1];
        if(p1.y < yTop && p2.y < yTop)
            continue;
        if(p1.y > yTop && p2.y > yTop)
        {
            res.push_back(p1);
            res.push_back(p2);
            continue;
        }
        if(p1.y > yTop)
        {
            res.push_back(p1);
            point newOne;
            newOne.y = yTop;
            newOne.x = p1.x + (yTop - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            res.push_back(newOne);
        }
        if(p2.y > yTop)
        {
            point newOne;
            newOne.y = yTop;
            newOne.x = p1.x + (yTop - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            res.push_back(newOne);
            res.push_back(p2);
        }
    }
    return res;
}
vector<point> Clipper::clipBottom(vector<point> polygon, int yBottom)
{
    vector<point> res;
    for(int i = 0; i < (int)polygon.size() - 1; i += 2)
    {
        point p1 = polygon[i];
        point p2 = polygon[i + 1];
        if(p1.y > yBottom && p2.y > yBottom)
            continue;
        if(p1.y < yBottom && p2.y < yBottom)
        {
            res.push_back(p1);
            res.push_back(p2);
            continue;
        }
        if(p1.y < yBottom)
        {
            res.push_back(p1);
            point newOne;
            newOne.y = yBottom;
            newOne.x = p1.x + (yBottom - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            res.push_back(newOne);
        }
        if(p2.y < yBottom)
        {
            point newOne;
            newOne.y = yBottom;
            newOne.x = p1.x + (yBottom - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            res.push_back(newOne);
            res.push_back(p2);
        }
    }
    return res;
}
void Clipper::clipPolygonBySides(HDC hdc, vector<point> polygon)
{
    vector<point> poly;
    if(polygon.size() <= 2)
        poly = polygon;
    else
    {
        for(int i = 0; i < polygon.size(); ++i)
        {
            poly.push_back(polygon[i]);
            poly.push_back(polygon[(i + 1) % polygon.size()]);
        }
    }
    vector<point> left = clipLeft(poly, xleft);
    vector<point> right = clipRight(left, xright);
    vector<point> top = clipTop(right, ytop);
    vector<point> res = clipBottom(top, ybottom);
    for(int i = 0; i < (int)res.size() - 1; i += 2)
    {
        lineMidPoint(hdc, res[i].x,  res[i].y, res[i + 1].x, res[i + 1].y, WHITE_BRUSH);
    }
}
