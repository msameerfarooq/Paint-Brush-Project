/*
* CSE/ENGR 142 GP142_Demo
*
* 1/25/95
* Author(s):         Larry Ruzzo
* Email Address(s):  ruzzo@cs.washington.edu
*
* This program is a demo of some of the features of
* GP142 --- the CSE/ENGR 142 Graphics Package.
* It includes simple drawing commands, keyboard and mouse
* interaction, and a simple animation.
*
* Revision history:
* 11/18/97 Dan Boren  - Removed calls to obsolete function GP142_undo()
*
*/
//#include "Shape.h"
#include "GP142.h"
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cstring>
#include <fstream>
#include <conio.h>
#include <fstream>
#include <string>
#include <string.h>
using namespace std;
#ifndef DRIVER_CPP_
#define DRIVER_CPP_
#define PI 3.1415926535897932384
#define ANIMATE_MSG "Select \"Animate\"\nOn the \"Run\" Menu"
#define TEXT_SIZE 20

#define First_Line_TL_x -482
#define First_Line_BR_x -458

#define Second_Line_TL_x -452
#define Second_Line_BR_x -428

#define Line_TL_y 350
#define Line_BR_y 331

#define Circle_Eraser_TL_y 211
#define Circle_Eraser_BR_y 188

#define Angle_TL_y 314
#define Angle_BR_y 293

#define Text_Polygon_TL_y 278
#define Text_Polygon_BR_y 254

#define Curve_TL_y 240
#define Curve_BR_y 220

#define Bucket_TL_y 181
#define Bucket_BR_y 157

// First Row of Colour
#define First_Row_C_TL_y 80
#define First_Row_C_BR_y 70

#define Second_Row_C_TL_y 60
#define Second_Row_C_BR_y 50

#define Third_Row_C_TL_y 40
#define Third_Row_C_BR_y 30

#define Fourth_Row_C_TL_y 20
#define Fourth_Row_C_BR_y 10

#define Fifth_Row_C_TL_y 0
#define Fifth_Row_C_BR_y -10

#define Sixth_Row_C_TL_y -20
#define Sixth_Row_C_BR_y -30

// LS---> Load/Save Menu
#define LS_TL_x 453
#define LS_BR_x 487

#define Load_TL_y 350
#define Load_BR_y 334

#define Save_TL_y 330
#define Save_BR_y 314

vector<GP142_point *> T;
vector<GP142_point> Bucket_Points;
vector<int> Clr_No_for_Bucket_Points;
int clr = 0, toolno = 0, LS_no = 0;
inline double distance_between_points(GP142_point P1, GP142_point P2)
{
    return sqrt(pow(P1.y - P2.y, 2) + pow(P1.x - P2.x, 2));
}
GP142_point point(int x, int y)
{
    GP142_point a;
    a.x = x;
    a.y = y;
    return a;
}
vector<GP142_point *> Allocate_Points()
{
    T.resize(3);
    for (int i = 0; i < 3; i++)
    {
        T[i] = new GP142_point[4];
        for (int j = 0; j < 4; j++)
        {
            T[i][j].x = 0;
            T[i][j].y = 0;
        }
    }
    T[0][0].x = T[0][3].x = -488;
    T[0][0].y = T[0][1].y = 355;
    T[0][1].x = T[0][2].x = -420;
    T[0][2].y = T[0][3].y = 130;

    T[1][0].x = T[1][3].x = -488;
    T[1][0].y = T[1][1].y = 110;
    T[1][1].x = T[1][2].x = -420;
    T[1][2].y = T[1][3].y = -50;

    T[2][0].x = T[2][3].x = 450;
    T[2][0].y = T[2][1].y = 355;
    T[2][1].x = T[2][2].x = 490;
    T[2][2].y = T[2][3].y = 309;
    return T;
}
GP142_point *GetPoints(GP142_point temp1, GP142_point temp2)
{
    GP142_point *arr = new GP142_point[9];

    //Mid-Point

    arr[4].x = temp1.x + temp2.x;
    arr[4].x /= 2;
    arr[4].y = temp1.y + temp2.y;
    arr[4].y /= 2;

    //First Half

    arr[2].x = temp1.x + arr[4].x;
    arr[2].x /= 2;
    arr[2].y = temp1.y + arr[4].y;
    arr[2].y /= 2;

    arr[3].x = arr[2].x + arr[4].x;
    arr[3].x /= 2;
    arr[3].y = arr[2].y + arr[4].y;
    arr[3].y /= 2;

    arr[1].x = temp1.x + arr[2].x;
    arr[1].x /= 2;
    arr[1].y = temp1.y + arr[2].y;
    arr[1].y /= 2;

    arr[0].x = temp1.x;
    arr[0].y = temp1.y;

    //Second Half

    arr[6].x = temp2.x + arr[4].x;
    arr[6].x /= 2;
    arr[6].y = temp2.y + arr[4].y;
    arr[6].y /= 2;

    arr[5].x = arr[6].x + arr[4].x;
    arr[5].x /= 2;
    arr[5].y = arr[6].y + arr[4].y;
    arr[5].y /= 2;

    arr[7].x = temp2.x + arr[6].x;
    arr[7].x /= 2;
    arr[7].y = temp2.y + arr[6].y;
    arr[7].y /= 2;

    arr[8].x = temp2.x;
    arr[8].y = temp2.y;

    return arr;
}

bool Point_Within_Box(GP142_point Top_Left_Corner, GP142_point Bottom_Right_Corner, int x, int y)
{
    if ((Top_Left_Corner.x < x) && (Top_Left_Corner.y > y) && (Bottom_Right_Corner.x > x) && (Bottom_Right_Corner.y < y))
        return 1;
    return 0;
}
bool Point_On_Line(GP142_point P1, GP142_point P2, GP142_point P3)
{
    bool flag = false;
    flag = distance_between_points(P1, P2) == distance_between_points(P1, P3) + distance_between_points(P2, P3);
    double y2_y1 = 0.0, x2_x1 = 0.0;
    y2_y1 = (P3.y - P2.y);
    x2_x1 = (P3.x - P2.x);
    if (x2_x1 == 0)
    {
        if (P1.x == P3.x)
            flag = true;
    }
    else if (y2_y1 == 0)
    {
        if (P1.y == P3.y)
            flag = true;
    }
    else
    {
        if (abs((P1.y - P3.y) * (x2_x1) - (y2_y1) * (P1.x - P3.x)) < 0.001)
            flag = true;
    }
    return flag;
}
//double y2_y1 = 0.0,x2_x1 = 0.0;
//y2_y1 = (P2.y-P3.y);
//x2_x1 = (P2.x-P3.x);

//for (int i = 0; i<4; i++)
//{
//	for (int j = 0; j<4; j++)
//	{
//		b = (P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j);
//		b = (P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j);
//		b = (P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j);
//		b = (P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j);
//		b = (P1.y-P3.y+i)*(x2_x1) +(y2_y1) *(P1.x-P3.x+j);
//		b = (P1.y-P3.y+i)*(x2_x1) +(y2_y1) *(P1.x-P3.x-j);
//		b = (P1.y-P3.y-i)*(x2_x1) +(y2_y1) *(P1.x-P3.x+j);
//		b = (P1.y-P3.y-i)*(x2_x1) +(y2_y1) *(P1.x-P3.x-j);
//		a += abs ((P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j))<1;
//		a += abs ((P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j))<1;
//		a += abs ((P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j))<1;
//		a += abs ((P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j))<1;
//		a += abs ((P1.y-P3.y+i)*(x2_x1) +(y2_y1) *(P1.x-P3.x+j))<1;
//		a += abs ((P1.y-P3.y+i)*(x2_x1) +(y2_y1) *(P1.x-P3.x-j))<1;
//		a += abs ((P1.y-P3.y-i)*(x2_x1) +(y2_y1) *(P1.x-P3.x+j))<1;
//		a += abs ((P1.y-P3.y-i)*(x2_x1) +(y2_y1) *(P1.x-P3.x-j))<1;
//		/*if (abs ((P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j))<1)
//			return true;
//			if (abs ((P1.y-P3.y+i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j))<1)
//			return true;
//			if (abs ((P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x+j))<1)
//			return true;
//			if (abs ((P1.y-P3.y-i)*(x2_x1) -(y2_y1) *(P1.x-P3.x-j))<1)
//			return true;*/
/*
		}
		}
		if (a>0)
		return true;*/
/*if (x2_x1==0)
{
if (P1.x==P3.x)
return 1;
}
else if (y2_y1==0)
{10152
if (P1.y==P3.y)
return 1;
}
else
{
if ((P1.y-P3.y)*(x2_x1)==(y2_y1) *(P1.x-P3.x))
return 1;
}*/
//double m1 = (P1.y-P2.y)/(P1.x-P2.x),m2 = (P1.y-P3.y)/(P1.x-P3.x);
//	return 0;

bool Point_On_Line(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return Point_On_Line(point(x1, y1), point(x2, y2), point(x3, y3));
}
bool Click_Within_Boxes(vector<GP142_point *> T, int x, int y)
{
    for (int i = 0; i < 3; i++)
    {
        if (Point_Within_Box(T[i][0], T[i][2], x, y))
            return true;
    }
    return false;
}
void Print_Tool_Box()
{
    for (int j = 355; j > 130; j--)
        GP142_lineXY(7, -488, j, -420, j, 3);

    // for Line
    for (int j = 331; j <= 350; j++)
        GP142_lineXY(14, -458, j, -482, j, 3); // Background colour

    GP142_lineXY(5, -480, 348, -460, 333, 2);

    // for dotted line
    for (int j = 331; j <= 350; j++)
        GP142_lineXY(14, -452, j, -428, j, 3); // Background colour

    GP142_lineXY(5, -450, 348, -430, 333, 2); // draw dotted line

    for (int i = -451; i < -429; i = i + 2)
        GP142_lineXY(14, i, 348, i, 333, 1);

    // for Triangle
    //GP142_triangleXY (1,-470,310,-460,295,-480,295,1);
    for (int j = 293; j <= 314; j++)
        GP142_lineXY(14, -458, j, -482, j, 3); // Background colour

    GP142_lineXY(5, -470, 310, -460, 295, 2);
    GP142_lineXY(5, -470, 310, -480, 295, 2);
    GP142_lineXY(5, -460, 295, -480, 295, 2);

    // for Rectangle
    // GP142_rectangleXY (1,-450,295,-430,310,1);
    for (int j = 293; j <= 314; j++)
        GP142_lineXY(14, -452, j, -428, j, 3); // Background colour

    GP142_lineXY(5, -450, 310, -430, 310, 2);
    GP142_lineXY(5, -450, 310, -450, 295, 2);
    GP142_lineXY(5, -450, 295, -430, 295, 2);
    GP142_lineXY(5, -430, 295, -430, 310, 2);

    // for a Text (A)
    for (int j = 254; j <= 278; j++)
        GP142_lineXY(14, -458, j, -482, j, 3); // Background colour

    GP142_lineXY(5, -462, 255, -470, 275, 4);
    GP142_lineXY(5, -478, 255, -470, 275, 4);
    GP142_lineXY(5, -476, 262, -465, 262, 4);

    // for a Polygon
    //
    //GP142_triangleXY (5,-440,276,-430,270,-450,270,2);
    //GP142_rectangleXY (5,-430,270,-450,262,2);
    //GP142_triangleXY (5,-440,256,-430,262,-450,262,2);
    for (int j = 254; j <= 278; j++)
        GP142_lineXY(14, -428, j, -452, j, 3); // Background colour

    GP142_lineXY(5, -440, 276, -430, 270, 2);
    GP142_lineXY(5, -440, 276, -450, 270, 2);
    GP142_lineXY(5, -430, 263, -430, 269, 2);
    GP142_lineXY(5, -450, 262, -450, 270, 2);
    GP142_lineXY(5, -440, 256, -430, 262, 2);
    GP142_lineXY(5, -440, 256, -450, 262, 2);

    // for a curve
    for (int j = 220; j <= 240; j++)
        GP142_lineXY(14, -458, j, -482, j, 3); // Background colour

    GP142_textXY(5, -477, 213, 26, "O");
    for (int i = -463; i < -459; i++)
        GP142_lineXY(14, i, 238, i, 220, 3); // hide the right part of O

    for (int j = 220; j <= 224; j++)
        GP142_lineXY(14, -463, j, -474, j, 2); // hide the lower part of O

    // for a dotted curve
    for (int j = 220; j <= 240; j++)
        GP142_lineXY(14, -428, j, -452, j, 3); // Background colour

    GP142_textXY(5, -447, 213, 26, "O");
    for (int i = -433; i < -429; i++)
        GP142_lineXY(14, i, 238, i, 220, 3); // hide the right part of O

    for (int j = 220; j <= 224; j++)
        GP142_lineXY(14, -433, j, -444, j, 2); // hide the lower part of O

    for (int j = 220; j <= 240; j = j + 2)
        GP142_lineXY(14, -430, j, -450, j, 1); // dotted horizontaly

    for (int i = -439; i < -430; i = i + 2)
        GP142_lineXY(14, i, 240, i, 220, 1); // dotted vertically

    // for a Circle
    for (int j = 188; j <= 211; j++)
        GP142_lineXY(14, -458, j, -482, j, 3); // Background colour

    GP142_circleXY(5, -470, 200, 10);

    // for an Eraser
    //	GP142_rectangleXY (5,-450,208,-430,192,1);
    for (int j = 188; j <= 211; j++)
        GP142_lineXY(14, -428, j, -452, j, 3); // Background colour

    GP142_lineXY(5, -450, 202, -445, 192, 1);
    GP142_lineXY(5, -434, 208, -430, 197, 4);
    GP142_lineXY(5, -445, 192, -430, 197, 3);
    GP142_lineXY(5, -450, 202, -435, 208, 1);

    // for Bucket
    for (int i = -482; i <= -428; i++)
        GP142_lineXY(14, i, 157, i, 181, 1); // dotted vertically

    GP142_textXY(5, -481, 160, 17, "Bucket");
    GP142_textXY(8, -485, 133, 17, "Tool Box");
}
void Colour_Box()
{
    for (int j = 110; j > -50; j--)
        GP142_lineXY(7, -488, j, -420, j, 3);

    GP142_textXY(8, -481, 90, 17, "Palette");

    for (int j = 80; j >= 70; j--)
    {
        for (int k = 0; k < 6; k++)
            GP142_lineXY(k, -482, j - 20 * k, -458, j - 20 * k, 2);
    }
    for (int j = 80; j >= 70; j--)
    {
        for (int k = 0, l = 0; k < 6; l++)
        {
            if (l == 1 || (l > 2 && l < 11) && l != 7) // if (l==1||l==3||l==4||l==5||l==6||l==8||l==9||l==10)
                continue;
            GP142_lineXY(l + 6, -452, j - 20 * k, -428, j - 20 * k, 2);
            k++;
        }
    }
}
void Print_Load_Save()
{
    for (int i = 450; i < 490; i++)
        GP142_lineXY(7, i, 355, i, 309, 2);
    for (int i = 453; i < 487; i++)
    {
        GP142_lineXY(14, i, 350, i, 334, 2);
        GP142_lineXY(14, i, 330, i, 314, 2);
    }
    GP142_textXY(5, 453, 334, 15, "Load");
    GP142_textXY(5, 453, 314, 15, "Save");
}
void initialize_0(GP142_point &P)
{
    P.x = P.y = 0;
}
double angle(GP142_point P1, GP142_point P2, GP142_point P3)
{
    int a[2] = {P1.x - P2.x, P1.y - P2.y};
    int b[2] = {P1.x - P3.x, P1.y - P3.y};
    double temp1 = a[0] * b[0] + a[1] * b[1];
    double temp2 = sqrt(pow(a[0], 2) + pow(a[1], 2));
    double temp3 = sqrt(pow(b[0], 2) + pow(b[1], 2));
    return acos(temp1 / (temp2 * temp3)); //(P122+P132-P232)/(2*P12 * P13))
                                          //return atan2 (P1.y-P2.y,P1.x-P2.x);
}
void Hollow_Rectangle(GP142_point P1, GP142_point P2)
{
    GP142_lineXY(7, P1.x, P1.y, P1.x, P2.y, 3);
    GP142_lineXY(7, P2.x, P2.y, P2.x, P1.y, 3);
    GP142_lineXY(7, P2.x, P1.y, P1.x, P1.y, 3);
    GP142_lineXY(7, P1.x, P2.y, P2.x, P2.y, 3);
}
void draw_line(int x1, int y1, int x2, int y2, int color = WHITE, int width = 1)
{
    GP142_lineXY(0, x1, y1, x2, y2, width);
}
void draw_dotted_line(GP142_point P1, GP142_point P2, int color, int thickness)
{
    GP142_point *arr1, *arr2, *arr3, *arr4;
    GP142_point temp1, temp2, temp3, temp4, temp5;

    temp1 = P1;
    temp5 = P2;
    temp3.x = temp1.x + temp5.x;
    temp3.x = temp3.x / 2;
    temp3.y = temp1.y + temp5.y;
    temp3.y = temp3.y / 2;
    temp2.x = temp1.x + temp3.x;
    temp2.x = temp2.x / 2;
    temp2.y = temp1.y + temp3.y;
    temp2.y = temp2.y / 2;
    temp4.x = temp3.x + temp5.x;
    temp4.x = temp4.x / 2;
    temp4.y = temp3.y + temp5.y;
    temp4.y = temp4.y / 2;
    arr1 = GetPoints(temp1, temp2);
    arr2 = GetPoints(temp2, temp3);
    arr3 = GetPoints(temp3, temp4);
    arr4 = GetPoints(temp4, temp5);

    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            GP142_lineXY(color, arr1[i].x, arr1[i].y, arr1[i + 1].x, arr1[i + 1].y, 1);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            GP142_lineXY(color, arr2[i].x, arr2[i].y, arr2[i + 1].x, arr2[i + 1].y, 1);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            GP142_lineXY(color, arr3[i].x, arr3[i].y, arr3[i + 1].x, arr3[i + 1].y, 1);
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (i % 2 == 0)
        {
            GP142_lineXY(color, arr4[i].x, arr4[i].y, arr4[i + 1].x, arr4[i + 1].y, 1);
        }
    }
}
void draw_dotted_line(int x1, int y1, int x2, int y2, int color, int thickness)
{
    GP142_point a, b;
    a.x = x1;
    a.y = y1;
    b.x = x2;
    b.y = y2;
    draw_dotted_line(a, b, color, thickness);
}
void draw_circle(int x, int y, int radius, int color = WHITE)
{
    GP142_circleXY(0, x, y, radius);
}
void handle_mouse(int x, int y)
{
    //GP142_printfXY (YELLOW,x,y,9,"Mouse at (%d,%d)",x,y);
}
void handle_kbd(char c)
{
    //put the character anywhere you want to.
    //I'm putting it on the origin
    //GP142_printfXY (WHITE,0,0,20,"%c",c);
}
void Colour_Pressed()
{
    if (clr == 0)
        Hollow_Rectangle(point(First_Line_TL_x, First_Row_C_TL_y), point(First_Line_BR_x, First_Row_C_BR_y));
    else if (clr == 1)
        Hollow_Rectangle(point(First_Line_TL_x, Second_Row_C_TL_y), point(First_Line_BR_x, Second_Row_C_BR_y));
    else if (clr == 2)
        Hollow_Rectangle(point(First_Line_TL_x, Third_Row_C_TL_y), point(First_Line_BR_x, Third_Row_C_BR_y));
    else if (clr == 3)
        Hollow_Rectangle(point(First_Line_TL_x, Fourth_Row_C_TL_y), point(First_Line_BR_x, Fourth_Row_C_BR_y));
    else if (clr == 4)
        Hollow_Rectangle(point(First_Line_TL_x, Fifth_Row_C_TL_y), point(First_Line_BR_x, Fifth_Row_C_BR_y));
    else if (clr == 5)
        Hollow_Rectangle(point(First_Line_TL_x, Sixth_Row_C_TL_y), point(First_Line_BR_x, Sixth_Row_C_BR_y));
    else if (clr == 6)
        Hollow_Rectangle(point(Second_Line_TL_x, First_Row_C_TL_y), point(Second_Line_BR_x, First_Row_C_BR_y));
    else if (clr == 8)
        Hollow_Rectangle(point(Second_Line_TL_x, Second_Row_C_TL_y), point(Second_Line_BR_x, Second_Row_C_BR_y));
    else if (clr == 13)
        Hollow_Rectangle(point(Second_Line_TL_x, Third_Row_C_TL_y), point(Second_Line_BR_x, Third_Row_C_BR_y));
    else if (clr == 17)
        Hollow_Rectangle(point(Second_Line_TL_x, Fourth_Row_C_TL_y), point(Second_Line_BR_x, Fourth_Row_C_BR_y));
    else if (clr == 18)
        Hollow_Rectangle(point(Second_Line_TL_x, Fifth_Row_C_TL_y), point(Second_Line_BR_x, Fifth_Row_C_BR_y));
    else if (clr == 19)
        Hollow_Rectangle(point(Second_Line_TL_x, Sixth_Row_C_TL_y), point(Second_Line_BR_x, Sixth_Row_C_BR_y));
}
bool Palette(int x, int y, int &Colour)
{
    Colour_Box();
    int n = -1;
    if (Point_Within_Box(point(First_Line_TL_x, First_Row_C_TL_y), point(First_Line_BR_x, First_Row_C_BR_y), x, y)) // for a line
        Colour = clr = n = 0;
    else if (Point_Within_Box(point(First_Line_TL_x, Second_Row_C_TL_y), point(First_Line_BR_x, Second_Row_C_BR_y), x, y)) // for a Triangle
        Colour = clr = n = 1;
    else if (Point_Within_Box(point(First_Line_TL_x, Third_Row_C_TL_y), point(First_Line_BR_x, Third_Row_C_BR_y), x, y)) // for a Text
        Colour = clr = n = 2;
    else if (Point_Within_Box(point(First_Line_TL_x, Fourth_Row_C_TL_y), point(First_Line_BR_x, Fourth_Row_C_BR_y), x, y)) // for a Curve
        Colour = clr = n = 3;
    else if (Point_Within_Box(point(First_Line_TL_x, Fifth_Row_C_TL_y), point(First_Line_BR_x, Fifth_Row_C_BR_y), x, y)) // for a Circle
        Colour = clr = n = 4;
    else if (Point_Within_Box(point(First_Line_TL_x, Sixth_Row_C_TL_y), point(First_Line_BR_x, Sixth_Row_C_BR_y), x, y)) // for a Circle
        Colour = clr = n = 5;
    else if (Point_Within_Box(point(Second_Line_TL_x, First_Row_C_TL_y), point(Second_Line_BR_x, First_Row_C_BR_y), x, y)) // for a dotted line
        Colour = clr = n = 6;
    else if (Point_Within_Box(point(Second_Line_TL_x, Second_Row_C_TL_y), point(Second_Line_BR_x, Second_Row_C_BR_y), x, y)) // for a Rectangle
        Colour = clr = n = 8;
    else if (Point_Within_Box(point(Second_Line_TL_x, Third_Row_C_TL_y), point(Second_Line_BR_x, Third_Row_C_BR_y), x, y)) // for a Polygon
        Colour = clr = n = 13;
    else if (Point_Within_Box(point(Second_Line_TL_x, Fourth_Row_C_TL_y), point(Second_Line_BR_x, Fourth_Row_C_BR_y), x, y)) // for a dotted curve
        Colour = clr = n = 17;
    else if (Point_Within_Box(point(Second_Line_TL_x, Fifth_Row_C_TL_y), point(Second_Line_BR_x, Fifth_Row_C_BR_y), x, y)) // for an Eraser
        Colour = clr = n = 18;
    else if (Point_Within_Box(point(Second_Line_TL_x, Sixth_Row_C_TL_y), point(Second_Line_BR_x, Sixth_Row_C_BR_y), x, y)) // for an Eraser
        Colour = clr = n = 19;
    Colour_Pressed();
    if (n != -1)
        return 1;
    return 0;
}
void LS_Pressed()
{
    if (LS_no == 0)
        Hollow_Rectangle(point(LS_TL_x, Load_TL_y), point(LS_BR_x, Load_BR_y));
    else if (LS_no == 1)
        Hollow_Rectangle(point(LS_TL_x, Save_TL_y), point(LS_BR_x, Save_BR_y));
}
void putpixel(int x, int y, int Colour)
{
    GP142_lineXY(Colour, x, y, x, y, 2);
}
int temporary = 0;
void BackGround()
{
    for (int i = -500; i < 500; i++)
        GP142_lineXY(1, i, -500, i, 500, 1);
}
void Bucket_fill(int x, int y, int Old, int fill)
{
    int current = Get_Pixel(x, y);
    if (Old == current && Old != fill)
    {
        //GP142_pixelP (fill,point (x,y));
        putpixel(x, y, fill);
        Bucket_fill(x + 1, y, Old, fill);
        Bucket_fill(x - 1, y, Old, fill);
        Bucket_fill(x, y + 1, Old, fill);
        Bucket_fill(x, y - 1, Old, fill);
    }
}

class Shape
{
protected:
    vector<GP142_point> Points;
    int Colour;
    int n;
    char *ch;

public:
    Shape()
    {
        Points.resize(0);
        Colour = 0;
    }
    Shape(vector<GP142_point> points, int colour)
    {
        Points.resize(points.size());
        for (int i = 0; i < static_cast<int>(Points.size()); i++)
        {
            Points[i] = points[i];
        }
        Colour = colour;
    }
    char *get_Word()
    {
        return ch;
    }
    Shape &operator=(const Shape &Obj)
    {
        Points.resize(Obj.Points.size());
        for (int i = 0; i < static_cast<int>(Points.size()); i++)
        {
            Points[i] = Obj.Points[i];
        }
        Colour = Obj.Colour;
        n = Obj.n;
        return *this;
    }
    vector<GP142_point> operator=(vector<GP142_point> obj)
    {
        vector<GP142_point> temp;
        temp.resize(obj.size());
        for (int i = 0; i < temp.size(); i++)
        {
            temp[i].x = obj[i].x;
            temp[i].y = obj[i].y;
        }
        return temp;
    }
    virtual void draw() = 0;
    virtual void Draw() = 0;
    virtual int ShapeNumber() = 0;
    vector<GP142_point> get_Points()
    {
        return Points;
    }
    int get_Colour()
    {
        return Colour;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    virtual bool contain(GP142_point) = 0;
    ~Shape()
    {
        Points.resize(0);
        Colour = 0;
    }
    friend class Load_Save_;
};

class Polygon_ : public Shape
{
private:
public:
    Polygon_()
    {
        n = 5;
    }
    Polygon_(vector<GP142_point> Points_, int Colour_) : Shape(Points_, Colour_)
    {
        n = 5;
    }
    void draw()
    {
        Points.resize(1);
        //	initialize_0 (pt [0]); initialize_0 (pt [1]); initialize_0 (pt [2]); initialize_0 (pt [3]);
        char c = '\0';
        for (int i = 0;; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (!Click_Within_Boxes(T, Points[i].x, Points[i].y) && c == '\0')
            {
                if (i != 0)
                {
                    if ((Points[i].x == Points[i - 1].x) && (Points[i].y == Points[i - 1].y))
                    {
                        GP142_lineP(Colour, Points[0], Points[i], 2);
                        break;
                    }
                    else
                    {
                        GP142_lineP(Colour, Points[i - 1], Points[i], 2);
                    }
                }
                Points.push_back(point(0, 0));
            }
            else
            {
                Palette(Points[i].x, Points[i].y, Colour);
                i--;
            }
        }
    }
    void Draw()
    {
        for (int i = 0, a = 1; i < Points.size(); i++, a++)
        {
            if (a == static_cast<int>(Points.size()))
                a = 0;
            GP142_lineP(Colour, Points[i], Points[a], 2);
        }
    }
    int ShapeNumber()
    {
        return n;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    bool contain(GP142_point P)
    {
        double a = 0;
        for (int i = 0; i < static_cast<int>(Points.size()) - 1; i++)
        {
            a += angle(P, Points[i], Points[i + 1]); // if sum of angle is equal to 2(pi) or -2(pi) then it's inside the polygon
        }
        a += angle(P, Points[static_cast<int>(Points.size()) - 1], Points[0]);
        if (abs(a - 2 * PI) < 0.00001)
            return 1;
        return 0;
    }
    ~Polygon_()
    {
    }
};
class fill_ : public Shape
{
private:
public:
    fill_()
    {
        n = 10;
    }
    fill_(vector<GP142_point> Points_, int Colour_) : Shape(Points_, Colour_)
    {
        n = 10;
    }
    bool contain(GP142_point P)
    {
        return 0;
    }
    void Draw()
    {
        for (int i = 0; i < Points.size(); i++)
        {
            Bucket_fill(Points[0].x, Points[0].y, Get_Pixel(Points[0].x, Points[0].y), Colour);
        }
    }
    int ShapeNumber()
    {
        return n;
    }
    void draw()
    {
        int x = 0, y = 0;
        char ch = '\0';
        GP142_await_event(&x, &y, &ch);
        while (1)
        {
            if (Palette(x, y, clr) || Click_Within_Boxes(T, x, y))
            {
                GP142_await_event(&x, &y, &ch);
            }
            else
                break;
        }
        Bucket_fill(x, y, Get_Pixel(x, y), clr);
        Points.push_back(point(x, y));
    }
    ~fill_()
    {
    }
};
class OpenShape : public Shape
{
protected:
    bool style;

public:
    OpenShape()
    {
        style = 0;
    }
    OpenShape(vector<GP142_point> points_, int Colour_) : Shape(points_, Colour_)
    {
        style = 0;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    int ShapeNumber()
    {
        return n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    ~OpenShape()
    {
        style = 0;
    }
};
class Line : public OpenShape
{
public:
    Line()
    {
        style = 0;
        n = 0;
    }
    Line(int Style)
    {
        style = Style;
        n = 0;
    }
    int ShapeNumber()
    {
        return n;
    }
    Line(vector<GP142_point> points_, int Colour_, bool Style) : OpenShape(points_, Colour_)
    {
        n = 0;
        style = Style;
    }
    void Draw()
    {
        if (style == 0)
            GP142_lineP(Colour, Points[0], Points[1], 2);
        else
            draw_dotted_line(Points[0], Points[1], Colour, 2);
    }
    void draw()
    {
        Points.resize(2);
        initialize_0(Points[0]);
        initialize_0(Points[1]);
        char c = '\0';
        for (int i = 0; i < 2; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (Click_Within_Boxes(T, Points[i].x, Points[i].y))
            {
                Palette(Points[i].x, Points[i].y, Colour);
                --i;
            }
        }
        Draw();
    }
    bool contain(GP142_point P)
    {
        bool a = Point_On_Line(Points[0], Points[1], P);
        return a;
        //return Point_On_Line (Points [0],Points [1],P);
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    ~Line()
    {
        Points.resize(0);
    }
};
class Curve : public OpenShape
{
public:
    Curve()
    {
        style = 0;
        n = 6;
    }
    Curve(int Style)
    {
        style = Style;
        n = 6;
    }
    int ShapeNumber()
    {
        return n;
    }
    Curve(vector<GP142_point> Points_, int Color_, bool Style) : OpenShape(Points_, Color_)
    {
        style = Style;
        n = 6;
    }
    ~Curve()
    {
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    bool contain(GP142_point P)
    {
        for (int i = 0; i < static_cast<int>(Points.size()) - 1; i++)
        {
            if (Point_On_Line(Points[i], Points[i + 1], P))
                return 1;
        }
        return 0;
    }
    void draw()
    {
        Points.resize(1);
        char c = '\0';
        for (int i = 0;; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (!Click_Within_Boxes(T, Points[i].x, Points[i].y))
            {
                if (i != 0)
                {
                    if ((Points[i].x == Points[i - 1].x) && (Points[i].y == Points[i - 1].y))
                        break;
                    else
                    {
                        if (style == 0)
                            GP142_lineP(Colour, Points[i - 1], Points[i], 2);
                        else
                            draw_dotted_line(Points[i], Points[i - 1], Colour, 2);
                    }
                }
                Points.push_back(point(0, 0));
            }
            else
            {
                Palette(Points[i].x, Points[i].y, Colour);
                i--;
            }
        }
    }
    void Draw()
    {
        for (int i = 1; i < Points.size(); i++)
        {
            if (style == 0)
                GP142_lineP(Colour, Points[i - 1], Points[i], 2);
            else
                draw_dotted_line(Points[i], Points[i - 1], Colour, 2);
        }
    }
};
class Rectangle_ : public Polygon_
{
private:
public:
    Rectangle_()
    {
        n = 3;
    }
    Rectangle_(vector<GP142_point> Points_, int Colour_) : Polygon_(Points_, Colour_)
    {
        n = 3;
    }
    void Draw()
    {
        GP142_lineXY(Colour, Points[0].x, Points[0].y, Points[0].x, Points[1].y, 2);
        GP142_lineXY(Colour, Points[1].x, Points[1].y, Points[1].x, Points[0].y, 2);
        GP142_lineXY(Colour, Points[1].x, Points[0].y, Points[0].x, Points[0].y, 2);
        GP142_lineXY(Colour, Points[0].x, Points[1].y, Points[1].x, Points[1].y, 2);
    }
    void draw()
    {
        Points.resize(2);
        char c = '\0';
        for (int i = 0; i < 2; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (Click_Within_Boxes(T, Points[i].x, Points[i].y))
            {
                Palette(Points[i].x, Points[i].y, Colour);
                --i;
            }
        }
        Draw();
    }
    int ShapeNumber()
    {
        return n;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    bool contain(GP142_point P)
    {
        if (Point_On_Line(Points[0].x, Points[0].y, Points[0].x, Points[1].y, P.x, P.y) ||
            Point_On_Line(Points[1].x, Points[1].y, Points[1].x, Points[0].y, P.x, P.y) ||
            Point_On_Line(Points[1].x, Points[0].y, Points[0].x, Points[0].y, P.x, P.y) ||
            Point_On_Line(Points[0].x, Points[1].y, Points[1].x, Points[1].y, P.x, P.y))
            return 1;
        return Point_Within_Box(Points[0], Points[1], P.x, P.y);
    }
    ~Rectangle_()
    {
    }
};
class Circle : public Shape
{
private:
    int rad;

public:
    Circle()
    {
        n = 8;
    }
    bool contain(GP142_point P)
    {
        double a = P.x - Points[0].x, b = P.y - Points[0].y, c = rad * rad;
        if (a * a + b * b > c)
            return 0;
        return 1;
    }
    Circle(vector<GP142_point> Points_, int Color) : Shape(Points_, Color)
    {
        n = 8;
    }
    void Draw()
    {
        GP142_circleP(Colour, Points[0], rad);
    }
    void draw()
    {
        Points.resize(2);
        char c = '\0';
        for (int i = 0; i < 2; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (Click_Within_Boxes(T, Points[i].x, Points[i].y))
            {
                Palette(Points[i].x, Points[i].y, Colour);
                --i;
            }
        }
        rad = distance_between_points(Points[0], Points[1]);
        Draw();
    }
    int ShapeNumber()
    {
        return n;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    ~Circle()
    {
    }
};
class Triangle : public Polygon_
{
private:
public:
    Triangle()
    {
        n = 2;
    }
    void Draw()
    {
        GP142_lineP(Colour, Points[0], Points[1], 2);
        GP142_lineP(Colour, Points[0], Points[2], 2);
        GP142_lineP(Colour, Points[1], Points[2], 2);
    }
    void draw()
    {
        Points.resize(3);
        char c = '\0';
        for (int i = 0; i < 3; i++)
        {
            GP142_await_event(&Points[i].x, &Points[i].y, &c);
            if (!Click_Within_Boxes(T, Points[i].x, Points[i].y))
            {
                if (i != 0)
                {
                    GP142_lineP(Colour, Points[i - 1], Points[i], 2);
                }
            }
            else
            {
                Palette(Points[i].x, Points[i].y, Colour);
                i--;
            }
        }
        GP142_lineP(Colour, Points[0], Points[2], 2);
    }
    Triangle(vector<GP142_point> Points_, int Color) : Polygon_(Points_, Color)
    {
        n = 2;
    }
    bool contain(GP142_point P)
    {
        for (int i = 0; i < 2; i++)
        {
            if (Point_On_Line(Points[i], Points[i + 1], P))
                return 1;
        }
        bool flag = false;
        flag = Polygon_::contain(P);
        return Point_On_Line(Points[0], Points[2], P) || flag;
    }
    int ShapeNumber()
    {
        return n;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    ~Triangle()
    {
    }
};
class Text : public Shape
{
private:
public:
    Text()
    {
        n = 4;
    }
    Text(vector<GP142_point> Points_, int Colour_, char *cha) : Shape(Points_, Colour_)
    {
        int a = strlen(cha);
        ch = new char[a + 1];
        strcpy_s(ch, strlen(cha) + 1, cha);
        n = 4;
    }
    void Draw()
    {
        GP142_textP(Colour, Points[0], 20, ch);
    }
    char *get_Word()
    {
        return ch;
    }
    void draw()
    {
        Points.push_back(point(0, 0));
        int mouse_x, mouse_y, i = 0;
        char key_pressed;
        char temp[120];
        bool flag = false;
        while (1)
        {
            GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
            if (key_pressed == '\0')
            {
                if (!Click_Within_Boxes(T, mouse_x, mouse_y))
                {
                    Points[0].x = mouse_x;
                    Points[0].y = mouse_y;
                }
                else
                {
                    Palette(mouse_x, mouse_y, Colour);
                }
            }
            else if (mouse_x == 0 && mouse_y == 0 && key_pressed != '\0' && key_pressed != '\r')
            {
                temp[i++] = key_pressed;
            }
            else if (key_pressed == '\r')
                break;
        }
        temp[i++] = '\0';
        ch = new char[i];
        for (int j = 0; j < i; j++)
            ch[j] = temp[j];
        Draw();
    }
    int ShapeNumber()
    {
        return n;
    }
    void SetColour(int n)
    {
        Colour = clr = n;
    }
    void ChangeColour(int n)
    {
        SetColour(n);
        Draw();
    }
    bool contain(GP142_point P)
    {
        return Point_Within_Box(point(Points[0].x, Points[0].y + 4), point(Points[0].x + 10 * (strlen(ch)), Points[0].y), P.x, P.y);
    }
    ~Text()
    {
    }
};

vector<Shape *> S;
class Load_Save_
{
private:
    vector<GP142_point> pt;
    int ColourNo;

public:
    Load_Save_()
    {
        pt.resize(0);
        ColourNo = 0;
    }
    void serialize(string fname)
    {
        ofstream write(fname);
        for (int i = 0; i < S.size(); i++)
        {
            pt = S[i]->get_Points();
            ColourNo = S[i]->get_Colour();

            write << S[i]->ShapeNumber() << "\t" << S[i]->get_Colour() << "\t";

            if (S[i]->ShapeNumber() == 4)
                write << S[i]->get_Word() << "  ";
            for (int j = 0; j < pt.size(); j++)
            {
                write << pt[j].x << "\t" << pt[j].y << "\t";
            }
            write << 1000 << "\t" << 1000;
            if ((i + 1) != S.size())
                write << endl;
        }
        write.close();
    }
    void unserialize(string fname)
    {
        string str;
        char temp[120];
        int shapeNo = 0, temp1 = 0, temp2 = 0;
        char *c;
        ifstream read(fname);
        for (int i = 0; !read.eof(); i++)
        {
            pt.resize(0);
            read >> shapeNo;
            read >> ColourNo;
            if (shapeNo == 4)
            {
                read >> str;
                int l = 0;
                str.shrink_to_fit();
                c = new char[l = str.size() + 1];
                for (int j = 0; j < l; j++)
                    c[j] = str[j];
                c[l] = '\0';
            }
            for (;;)
            {
                read >> temp1;
                read >> temp2;
                if ((temp1 == 1000) || (temp2 == 1000))
                    break;
                else
                    pt.push_back(point(temp1, temp2));
            }

            if (shapeNo == 0)
                S.push_back(new Line(pt, ColourNo, 0));
            else if (shapeNo == 1)
                S.push_back(new Line(pt, ColourNo, 1));
            else if (shapeNo == 2)
                S.push_back(new Triangle(pt, ColourNo));
            else if (shapeNo == 3)
                S.push_back(new Rectangle_(pt, ColourNo));
            else if (shapeNo == 4)
                S.push_back(new Text(pt, ColourNo, c));
            else if (shapeNo == 5)
                S.push_back(new Polygon_(pt, ColourNo));
            else if (shapeNo == 6)
                S.push_back(new Curve(pt, ColourNo, 0));
            else if (shapeNo == 7)
                S.push_back(new Curve(pt, ColourNo, 1));
            else if (shapeNo == 8)
                S.push_back(new Circle(pt, ColourNo));
            else if (shapeNo == 10)
                S.push_back(new fill_(pt, ColourNo));
            S[i]->Draw();
        }
        read.close();
    }
    ~Load_Save_()
    {
        pt.resize(0);
    }
};
Load_Save_ LS;
void Canvas()
{
    BackGround();
    for (int i = 0; i < S.size(); i++)
        S[i]->Draw();
    Print_Tool_Box();
    Colour_Box();
    Colour_Pressed();
    Print_Load_Save();
}
bool Load_Save(int x, int y)
{
    int n = -1;
    if (Point_Within_Box(point(LS_TL_x, Load_TL_y), point(LS_BR_x, Load_BR_y), x, y)) // for a Load
    {
        LS_no = n = 0;
        LS_Pressed();
    }
    else if (Point_Within_Box(point(LS_TL_x, Save_TL_y), point(LS_BR_x, Save_BR_y), x, y)) // for a Save
    {
        LS_no = n = 1;
        LS_Pressed();
    }
    if (n == -1)
        return 0;

    int mouse_x, mouse_y, i = 0;
    char key_pressed;
    char temp[120];
    while (1)
    {
        GP142_await_event(&mouse_x, &mouse_y, &key_pressed);
        if (mouse_x == 0 && mouse_y == 0 && key_pressed != '\0' && key_pressed != '\r')
        {
            temp[i++] = key_pressed;
        }
        else if (mouse_x != 0 || mouse_y != 0)
        {
            return 0;
        }
        else if (key_pressed == '\r')
            break;
    }
    temp[i++] = '\0';

    if (n == 0)
        LS.unserialize(temp);
    else if (n == 1)
        LS.serialize(temp);
    Print_Load_Save();
    return 1;
}
void Eraser(int x, int y)
{
    char ch = '\0';
    bool flag = false;
    GP142_await_event(&x, &y, &ch);
    for (int i = S.size() - 1; i >= 0; i--)
    {
        if (S[i]->contain(point(x, y)))
        {
            flag = true;
            S.erase(S.begin() + i);
            S.shrink_to_fit();
            break;
        }
    }
    if (flag)
    {
        GP142_clear();
        Canvas();
    }
}
void Tool_Pressed()
{
    if (toolno == 0)
        Hollow_Rectangle(point(First_Line_TL_x, Line_TL_y), point(First_Line_BR_x, Line_BR_y));
    else if (toolno == 1)
        Hollow_Rectangle(point(Second_Line_TL_x, Line_TL_y), point(Second_Line_BR_x, Line_BR_y));
    else if (toolno == 2)
        Hollow_Rectangle(point(First_Line_TL_x, Angle_TL_y), point(First_Line_BR_x, Angle_BR_y));
    else if (toolno == 3)
        Hollow_Rectangle(point(Second_Line_TL_x, Angle_TL_y), point(Second_Line_BR_x, Angle_BR_y));
    else if (toolno == 4)
        Hollow_Rectangle(point(First_Line_TL_x, Text_Polygon_TL_y), point(First_Line_BR_x, Text_Polygon_BR_y));
    else if (toolno == 5)
        Hollow_Rectangle(point(Second_Line_TL_x, Text_Polygon_TL_y), point(Second_Line_BR_x, Text_Polygon_BR_y));
    else if (toolno == 6)
        Hollow_Rectangle(point(First_Line_TL_x, Curve_TL_y), point(First_Line_BR_x, Curve_BR_y));
    else if (toolno == 7)
        Hollow_Rectangle(point(Second_Line_TL_x, Curve_TL_y), point(Second_Line_BR_x, Curve_BR_y));
    else if (toolno == 8)
        Hollow_Rectangle(point(First_Line_TL_x, Circle_Eraser_TL_y), point(First_Line_BR_x, Circle_Eraser_BR_y));
    else if (toolno == 9)
        Hollow_Rectangle(point(Second_Line_TL_x, Circle_Eraser_TL_y), point(Second_Line_BR_x, Circle_Eraser_BR_y));
    else if (toolno == 10)
        Hollow_Rectangle(point(First_Line_TL_x, Bucket_TL_y), point(Second_Line_BR_x, Bucket_BR_y));
}

bool Tool_Box(int x, int y, int &n)
{
    Tool_Pressed();
    n = -1;
    if (Point_Within_Box(point(First_Line_TL_x, Line_TL_y), point(First_Line_BR_x, Line_BR_y), x, y)) // for a line
    {
        S.push_back(new Line(0));
        toolno = n = 0;
    }
    else if (Point_Within_Box(point(Second_Line_TL_x, Line_TL_y), point(Second_Line_BR_x, Line_BR_y), x, y)) // for a dotted line
    {
        S.push_back(new Line(1));
        toolno = n = 1;
    }
    else if (Point_Within_Box(point(First_Line_TL_x, Angle_TL_y), point(First_Line_BR_x, Angle_BR_y), x, y)) // for a Triangle
    {
        S.push_back(new Triangle);
        toolno = n = 2;
    }
    else if (Point_Within_Box(point(Second_Line_TL_x, Angle_TL_y), point(Second_Line_BR_x, Angle_BR_y), x, y)) // for a Rectangle
    {
        S.push_back(new Rectangle_);
        toolno = n = 3;
    }
    else if (Point_Within_Box(point(First_Line_TL_x, Text_Polygon_TL_y), point(First_Line_BR_x, Text_Polygon_BR_y), x, y)) // for a Text
    {
        S.push_back(new Text);
        toolno = n = 4;
    }
    else if (Point_Within_Box(point(Second_Line_TL_x, Text_Polygon_TL_y), point(Second_Line_BR_x, Text_Polygon_BR_y), x, y)) // for a Polygon
    {
        S.push_back(new Polygon_);
        toolno = n = 5;
    }
    else if (Point_Within_Box(point(First_Line_TL_x, Curve_TL_y), point(First_Line_BR_x, Curve_BR_y), x, y)) // for a Curve
    {
        S.push_back(new Curve(0));
        toolno = n = 6;
    }
    else if (Point_Within_Box(point(Second_Line_TL_x, Curve_TL_y), point(Second_Line_BR_x, Curve_BR_y), x, y)) // for a dotted curve
    {
        S.push_back(new Curve(1));
        toolno = n = 7;
    }
    else if (Point_Within_Box(point(First_Line_TL_x, Circle_Eraser_TL_y), point(First_Line_BR_x, Circle_Eraser_BR_y), x, y)) // for a Circle
    {
        S.push_back(new Circle);
        toolno = n = 8;
    }
    else if (Point_Within_Box(point(Second_Line_TL_x, Circle_Eraser_TL_y), point(Second_Line_BR_x, Circle_Eraser_BR_y), x, y)) // for an Eraser
    {
        Eraser(x, y);
        toolno = n = 9;
    }
    else if (Point_Within_Box(point(First_Line_TL_x, Bucket_TL_y), point(Second_Line_BR_x, Bucket_BR_y), x, y)) // for a Bucket
    {
        S.push_back(new fill_);
        toolno = n = 10;
    }
    Print_Tool_Box();
    Colour_Box();
    Colour_Pressed();
    Tool_Pressed();
    Print_Load_Save();
    if (n != 9 && n != -1)
    {
        S[S.size() - 1]->SetColour(clr);
        S[S.size() - 1]->draw();
    }
    Print_Tool_Box();
    if (n != -1)
        return 1;
    return 0;
}
int main(void)
{
    GP142_open(); /* Open and initialize the GP142 Graphics Window    */
    bool flag = false;
    T = Allocate_Points();
    int quit = 0; /* has user signaled quit yet?                      */
    Load_Save_ a;
    int mouse_x, mouse_y;
    char key_pressed;
    int temp = 0;
    Canvas();
    //a.unserialize ("abc.txt");
    for (int i = 0; i < S.size(); i++)
        S[i]->Draw();
    /*int size = 0;
	ifstream read ("Size.txt");
	read>>size;
	for (int i = 0; i<size; i++)
	{
	S [i]->unserialize ("Save.txt");
	S [i]->Draw ();
	}*/
    //The Event Loop
    while (!quit)
    {
        switch (GP142_await_event(&mouse_x, &mouse_y, &key_pressed))
        {
            /*Print_Tool_Box ();
			Colour_Box ();
			Colour_Pressed ();
			Print_Load_Save ();*/
            flag = false;
        case GP142_QUIT:
            quit = 1; /* set flag to terminate loop                */
            break;

        case GP142_MOUSE: // Mouse Handler
            if (!Tool_Box(mouse_x, mouse_y, temp))
            {
                if (!Palette(mouse_x, mouse_y, clr))
                {
                    if (!Load_Save(mouse_x, mouse_y))
                        flag = true;
                }
            }
            if (flag)
            {
                for (int i = S.size() - 1; i >= 0; i--)
                {
                    if (S[i]->contain(point(mouse_x, mouse_y)))
                    {
                        S[i]->ChangeColour(clr);
                        break;
                    }
                }
            }
            break;

        case GP142_KBD:
            //Keyboard handler
            handle_kbd(key_pressed);
            break;

        default:
            break;
        }
    }
    //a.serialize ("abc.txt");
    /*for (int i = 0; i<S.size (); i++)
		S [i]->serialize ("Save.txt");
		ofstream a ("Size.txt");
		a<<S.size ();
		a.close ();*/
    GP142_close(); /* Clean up and close graphics window           */
    return 0;
}
#endif