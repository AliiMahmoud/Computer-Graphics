#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include"Curve.h"
#include"Line.h"
#include"Circle.h"
#include"Ellipsee.h"
#include"Clipper.h"

#define LINE_DDA                    3
#define LINE_MID                    5
#define LINE_PARAMETRIC             7
#define ELLIPSE_MID                 11
#define ELLIPSE_POLAR               13
#define ELLIPSE_DIRECT              17
#define CURVE_HERMITE               23
#define CURVE_BEZIER                29
#define SPLINE                      31
#define CLIPPING                    37
#define SAVE                        40
#define LOAD                        50

using namespace std;


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void saveData();
void loadData(HDC);


/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("DRAWING AND CLIPPING");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the window class */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);
    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;


    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("DRAWING AND CLIPPING"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               600,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );


    /* Make the window visible on the screen */
    AddMenus(hwnd);
    ShowWindow (hwnd, nCmdShow);
//
//    /* Run the message loop. It will run until GetMessage() returns 0 */
//
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);

    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

int selected = 0;
Point point1;
Point point2;

Line line;

Ellipsee ellipse;

Clipper clipper;
vector<Point> polygonPoints;

Curve curve;
vector<Point> curvePoints;

// to store it
vector<Curve> curves;
vector<Line> lines;
vector<Ellipsee> ellipses;


bool startFlag = false, endFlag = false;
Point temp;
LONG WINAPI WindowProcedure(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    switch(mcode)
    {
        case WM_COMMAND:
            switch(LOWORD(wp))
            {
            case LINE_DDA:
                selected = LINE_DDA;
                break;
            case LINE_MID:
                selected = LINE_MID;
                break;
            case LINE_PARAMETRIC:
                selected = LINE_PARAMETRIC;
                break;
            case ELLIPSE_DIRECT:
                selected = ELLIPSE_DIRECT;
                break;
            case ELLIPSE_MID:
                selected = ELLIPSE_MID;
                break;
            case ELLIPSE_POLAR:
                selected = ELLIPSE_POLAR;
                break;
            case CURVE_BEZIER:
                selected = CURVE_BEZIER;
                break;
            case CURVE_HERMITE:
                selected = CURVE_HERMITE;
                break;
            case SPLINE:
                selected = SPLINE;
                break;
            case CLIPPING:
                selected = CLIPPING;
                break;
            case SAVE:
                selected = SAVE;
                saveData();
                break;
            case LOAD:
                selected = LOAD;
                hdc = GetDC(hwnd);
                loadData(hdc);
                break;
            default:
                selected = 0;
            }
        break;
        case WM_LBUTTONDOWN:
            point1.x = LOWORD(lp);
            point1.y = HIWORD(lp);
            if((selected == LINE_DDA || selected == LINE_MID || selected == LINE_PARAMETRIC) && !startFlag)
            {
                temp.x = point1.x;
                temp.y = point1.y;
                startFlag = true;
            }
            if(selected == CURVE_BEZIER)
                curvePoints.push_back(point1);
            if(selected == CURVE_HERMITE)
                curvePoints.push_back(point1);
            if(selected == SPLINE)
                curvePoints.push_back(point1);
            if(selected == CLIPPING)
                polygonPoints.push_back(point1);
            break;
        case WM_LBUTTONDBLCLK:
            hdc = GetDC(hwnd);
            if(selected == LINE_DDA)
                line.drawLineDDA(hdc, temp.x, temp.y, point1.x, point1.y, WHITE_BRUSH), lines.push_back(line);
            if(selected == LINE_PARAMETRIC)
                line.drawLineParametric(hdc, temp.x, temp.y, point1.x, point1.y, WHITE_BRUSH), lines.push_back(line);
            if(selected == LINE_MID)
                line.drawLineMidPoint(hdc, temp.x, temp.y, point1.x, point1.y, WHITE_BRUSH), lines.push_back(line);
            if(selected == CURVE_BEZIER)
                curve.drawCurveBezier(hdc, curvePoints, WHITE_BRUSH), curves.push_back(curve);
            if(selected == CURVE_HERMITE)
            {
                int u1 = 100, v1 = -200, u2 = 200, v2 = -100;
                curve.drawCurveHermite(hdc, curvePoints[0], curvePoints[1], u1, v1, u2, v2, WHITE_BRUSH), curves.push_back(curve);
            }
            if(selected == SPLINE)
                curve.drawQuadraticCurve(hdc, curvePoints, WHITE_BRUSH), curves.push_back(curve);
            if(selected == CLIPPING)
                clipper.clipPolygonByLines(hdc, polygonPoints, WHITE_BRUSH);
            curvePoints.clear();
            polygonPoints.clear();
            startFlag = false;
            ReleaseDC(hwnd, hdc);
            break;
        case WM_RBUTTONDOWN:
            point1.x = LOWORD(lp);
            point1.y = HIWORD(lp);
            break;
        case WM_RBUTTONUP:
        {
            point2.x = LOWORD(lp);
            point2.y = HIWORD(lp);
            hdc = GetDC(hwnd);
            int xc = (point1.x + point2.x) / 2;
            int yc = (point1.y + point2.y) / 2;
            int a = abs(point1.x  - point2.x) / 2;
            int b = abs(point1.y - point2.y) / 2;
            if(selected == ELLIPSE_MID)
            {
                cout << "midpoint ELLIPSE" << endl;
                ellipse.drawEllipseMidpoint(hdc, xc, yc, a, b, WHITE_BRUSH);
                ellipses.push_back(ellipse);
            }
            if(selected == ELLIPSE_DIRECT)
            {
                cout << "DIRECT ELLIPSE" << endl;
                ellipse.drawEllipseDefault(hdc, xc, yc, a, b, WHITE_BRUSH);
                ellipses.push_back(ellipse);
            }
            if(selected == ELLIPSE_POLAR)
            {
                cout << "POLAR ELLIPSE" << endl;
                ellipse.drawEllipsePolar(hdc, xc, yc, a, b, WHITE_BRUSH);
                ellipses.push_back(ellipse);
            }
            if(selected == CLIPPING)
            {
                cout << "CLIPPING" << endl;
                clipper.drawClipper(hdc, point1.x, point1.y, point2.x, point2.y, WHITE_BRUSH);
            }
            ReleaseDC(hwnd, hdc);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}

void AddMenus(HWND hwnd)
{

    HMENU hMenubar = CreateMenu();
    HMENU hMenu = CreateMenu();
    HMENU data = CreateMenu();
    HMENU lineSubMenu = CreatePopupMenu();
    HMENU ellipseSubMenu = CreatePopupMenu();
    HMENU curveSubMenu = CreatePopupMenu();

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) lineSubMenu, L"&Line");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_DDA, L"&DDA");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_MID, L"&Midpoint");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_PARAMETRIC, L"&Parametric");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) ellipseSubMenu, L"&Ellipse");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_POLAR, L"&Polar");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_MID, L"&Midpoint");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_DIRECT, L"&Direct");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) curveSubMenu, L"&Curve");
    AppendMenuW(curveSubMenu, MF_STRING, CURVE_HERMITE, L"&Hermite");
    AppendMenuW(curveSubMenu, MF_STRING, CURVE_BEZIER, L"&Bezier");
    AppendMenuW(curveSubMenu, MF_STRING, SPLINE, L"&Quadratic");

    AppendMenuW(hMenu, MF_STRING, CLIPPING, L"&Clipping");


    AppendMenuW(data, MF_STRING, LOAD, L"&Load");
    AppendMenuW(data, MF_STRING, SAVE, L"&save");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenu, L"&draw");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) data, L"&data");

    SetMenu(hwnd, hMenubar);
}

void saveData()
{
    fstream data("data.txt", ios::out | ios::trunc| ios::binary);
    int linesSize = lines.size();
    data.write((char*)&linesSize, sizeof(linesSize));
    for(int i = 0; i < linesSize; ++i)
        data.write((char*)&lines[i], sizeof(lines[i]));

    int ellipsesSize = ellipses.size();
    data.write((char*)&ellipsesSize, sizeof(ellipsesSize));
    for(int i = 0; i < ellipsesSize; ++i)
        data.write((char*)&ellipses[i], sizeof(ellipses[i]));

    int curvesSize = curves.size();
    data.write((char*)&curvesSize, sizeof(curvesSize));
    for(int i = 0; i < curvesSize; ++i)
    {
        data.write((char*)&curves[i].type, sizeof(curves[i].type));
        data.write((char*)&curves[i].u1, sizeof(curves[i].u1));
        data.write((char*)&curves[i].v1, sizeof(curves[i].v1));
        data.write((char*)&curves[i].u2, sizeof(curves[i].u2));
        data.write((char*)&curves[i].v2, sizeof(curves[i].v2));
        int elementSize = curves[i].curvePoints.size();
        data.write((char*)&elementSize, sizeof(elementSize));
        for(int j = 0; j < elementSize; ++j)
            data.write((char*)&curves[i].curvePoints[j], sizeof(curves[i].curvePoints[j]));
    }

    data.write((char*)&clipper.xLeft, sizeof(clipper.xLeft));
    data.write((char*)&clipper.xRight, sizeof(clipper.xRight));
    data.write((char*)&clipper.yTop, sizeof(clipper.yTop));
    data.write((char*)&clipper.yBottom, sizeof(clipper.yBottom));
    int size;
    size = clipper.polygonPoints.size();
    data.write((char*)&size, sizeof(size));
    for(int j = 0; j < size; ++j)
        data.write((char*)&clipper.polygonPoints[j], sizeof(clipper.polygonPoints[j]));
    data.close();
}

void loadData(HDC hdc)
{
    fstream data("data.txt", ios::in | ios::binary);

    int linesSize;
    data.read((char*)&linesSize, sizeof(linesSize));
    lines.resize(linesSize);
    for(int i = 0; i < linesSize; ++i)
        data.read((char*)&lines[i], sizeof(lines[i]));

    int ellipsesSize;
    data.read((char*)&ellipsesSize, sizeof(ellipsesSize));
    ellipses.resize(ellipsesSize);
    for(int i = 0; i < ellipsesSize; ++i)
        data.read((char*)&ellipses[i], sizeof(ellipses[i]));

    int curvesSize;
    data.read((char*)&curvesSize, sizeof(curvesSize));
    curves.resize(curvesSize);
    for(int i = 0; i < curvesSize; ++i)
    {

        data.read((char*)&curves[i].type, sizeof(curves[i].type));
        data.read((char*)&curves[i].u1, sizeof(curves[i].u1));
        data.read((char*)&curves[i].v1, sizeof(curves[i].v1));
        data.read((char*)&curves[i].u2, sizeof(curves[i].u2));
        data.read((char*)&curves[i].v2, sizeof(curves[i].v2));
        int size;
        data.read((char*)&size, sizeof(size));
        curves[i].curvePoints.resize(size);
        for(int j = 0; j < size; ++j)
            data.read((char*)&curves[i].curvePoints[j], sizeof(curves[i].curvePoints[j]));
    }


    data.read((char*)&clipper.xLeft, sizeof(clipper.xLeft));
    data.read((char*)&clipper.xRight, sizeof(clipper.xRight));
    data.read((char*)&clipper.yTop, sizeof(clipper.yTop));
    data.read((char*)&clipper.yBottom, sizeof(clipper.yBottom));
    int size;
    data.read((char*)&size, sizeof(size));
    clipper.polygonPoints.resize(size);
    for(int j = 0; j < size; ++j)
        data.read((char*)&clipper.polygonPoints[j], sizeof(clipper.polygonPoints[j]));


    data.close();
    for(Line l : lines)
        l.drawStoredData(hdc, WHITE_BRUSH);
    for(Ellipsee e : ellipses)
        e.drawStoredData(hdc, WHITE_BRUSH);
    for(Curve c : curves)
        c.drawStoredData(hdc, WHITE_BRUSH);
    clipper.drawStoredData(hdc, WHITE_BRUSH);
}
