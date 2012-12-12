#include "stdafx.h"
#include "CP_PointVector.h"

#define null 0;
class Type
{
public:
	static const int point_start = 0;
    static const int point_end = 1;
    static const int point_regular = 2;
    static const int point_split = 3;
    static const int point_merge = 4;

	static const int line_insert = 11;
    static const int line_input = 22;//the egde the polygon itself or insert
    static const int line_undefined = 33;
};

class Point
{
public:
		CP_Vector2D point;
		int type;
		int index;
};

class Line
{
public:
    int index;
    Point *points[2];
    int type;//insert or input
    int helperIndex;
    Line():index(0),type(Type::line_undefined),helperIndex(0)
    {
        points[0] = null;
        points[1] = null;
    }
    Line(int p_index,int p_type, Point *p0, Point *p1, int p_helpIndex):index(p_index),type(p_type),helperIndex(p_helpIndex)
    {
        points[0] = p0;
        points[1] = p1;
    }
};