#include "stdafx.h"
#include "CP_PointVector.h"

class Type
{
public:
	static enum pointType{start, end, regular, split, merge};
	static enum egdeType{insert, input};//the egde the polygon itself or insert
};

class Point
{
	public:
		CP_Vector2D point;
		Type type;
		int index;
};