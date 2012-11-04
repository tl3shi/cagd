#include "CP_PointVector.h"
#include <vector>
using namespace std;

class DelaunayEdge
{
public:
	CP_Point2D p0, p1;
	DelaunayEdge(CP_Point2D &p0, CP_Point2D &p1);
	DelaunayEdge();
	~DelaunayEdge(){};
	
	/*
	//用set集合时候，重写此方法，以判断重复的set元素
	friend bool operator < (DelaunayEdge const &a,DelaunayEdge const &b)
	{
		bool res = (a.p0.m_x == b.p0.m_x &&
					a.p0.m_y == b.p0.m_y &&
					a.p1.m_x == b.p1.m_x &&
					a.p1.m_y == b.p1.m_y);
		if (res) return false;
		return true;
	}
	*/
};

class DelaunayTriangle
{
public:
	CP_Point2D p0, p1, p2;
	//外接圆圆心
	CP_Point2D center;
	//外接圆半径
	double radius_squared,radius;
	
	DelaunayTriangle();
	DelaunayTriangle(CP_Point2D &p0, CP_Point2D &p1, CP_Point2D &p2);
	void draw();
	//某点是否在其外接圆内
	bool includingPoint(CP_Point2D &p);
	
	~DelaunayTriangle(){};
protected:
	void calRadius();
};

//传入点，返回剖分后的三角形
vector<DelaunayTriangle> doDelaunayTriangulate(vector<CP_Point2D> points);
//构造最外层的大三角形
DelaunayTriangle createBigTriangle(vector<CP_Point2D>);
