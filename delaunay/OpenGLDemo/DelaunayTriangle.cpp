#include "StdAfx.h"
#include "DelaunayTriangle.h"
#include "GL/GLU.H" // 已经包含GL/GL.h
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

const double ZERODIS = 1.0e-6;
using namespace std;

DelaunayEdge::DelaunayEdge(CP_Point2D &p0, CP_Point2D &p1)
{
	this->p0 = p0;
	this->p1 = p1;
}

DelaunayTriangle::DelaunayTriangle(CP_Point2D &p0, CP_Point2D &p1, CP_Point2D &p2)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->calRadius();
}

void DelaunayTriangle::draw()
{
	
	//glBegin(GL_TRIANGLES);
	glBegin(GL_LINE_LOOP);
	glVertex2d(this->p0.m_x, this->p0.m_y);
	glVertex2d(this->p1.m_x, this->p1.m_y);
	glVertex2d(this->p2.m_x, this->p2.m_y);
	glEnd();
}
/*
Let the three given points be a, b, c. Use _0 and _1 to represent x and y coordinates. The coordinates of the center p=(p_0,p_1) of the circle determined by a, b, and c are:
A = b_0 - a_0;
B = b_1 - a_1;
C = c_0 - a_0;
D = c_1 - a_1;
E = A*(a_0 + b_0) + B*(a_1 + b_1);
F = C*(a_0 + c_0) + D*(a_1 + c_1);
G = 2.0*(A*(c_1 - b_1)-B*(c_0 - b_0));
p_0 = (D*E - B*F) / G;
p_1 = (A*F - C*E) / G;
If G is zero then the three points are collinear and no finite-radius circle through them exists. Otherwise, the radius of the circle is:

r^2 = (a_0 - p_0)^2 + (a_1 - p_1)^2
reference on:http://www.exaflop.org/docs/cgafaq/cga1.html
*/
void DelaunayTriangle::calRadius()
{
	double a = (this->p1 - this->p0).m_x;
	double b = (this->p1 - this->p0).m_y;
	double c = (this->p2 - this->p0).m_x;
	double d = (this->p2 - this->p0).m_y;
	double e = a * (this->p0.m_x + this->p1.m_x) + b *(this->p0.m_y + this->p1.m_y);
	double f = c * (this->p0.m_x + this->p2.m_x) + d *(this->p0.m_y + this->p2.m_y);
	double g = 2.0 * (a * (this->p2.m_y - this->p1.m_y) - b * (this->p2.m_x + this->p1.m_x));
	
	double center_x, center_y;
	double dx, dy;

	if ( fabs(g) < ZERODIS )
	{
		//// Collinear - find extremes and use the midpoint
		double max_x = max(max(this->p0.m_x,this->p1.m_x),this->p2.m_x);
		double max_y = max(max(this->p0.m_y,this->p1.m_y),this->p2.m_y);
		double min_x = min(min(this->p0.m_x,this->p1.m_x),this->p2.m_x);
		double min_y = min(min(this->p0.m_y,this->p1.m_y),this->p2.m_y);
		
		center_x = (max_x - min_x) / 2.0;
		center_y = (max_y - min_y) / 2.0;

		dx = center_x - min_x;
		dy = center_y - min_y;
	}else{
		center_x = (d*e - b*f) / g;
		center_y = (a*f - c*e) / g;

		dx = center_x - this->p0.m_x;
		dy = center_y - this->p0.m_y;
	}

	this->center = CP_Point2D(center_x, center_y);
	this->radius_squared = dx * dx + dy * dy;
	this->radius = sqrt(this->radius_squared);

}

	
vector<DelaunayEdge> removeDoubleEgdes(vector<DelaunayEdge> egdes)
{
	vector<DelaunayEdge> newEgdes;
	for (int i = 0; i < egdes.size(); i++)
	{
		DelaunayEdge egde = egdes[i];
		bool egdeEqual = false;
		//remove double ones 
		//detail:http://www.codeguru.com/cpp/cpp/algorithms/general/article.php/c8901/Delaunay-Triangles.htm
		for (int j = 0; j < egdes.size(); j++)
		{
			if(i != j)
			{
				DelaunayEdge egde2 = egdes[j];
				if ((egde2.p0 == egde.p0 && egde2.p1 == egde.p1) ||
					(egde2.p0 == egde.p1 && egde2.p1 == egde.p0))
				{
					egdeEqual = true;
					break;
				}
			}
		}
		if(! egdeEqual)
		{
			newEgdes.push_back(egde);
		}
	}
	return newEgdes;
}

bool DelaunayTriangle::includingPoint(CP_Point2D &p)
{
	double dx = p.m_x - this->center.m_x;
	double dy = p.m_y - this->center.m_y;

	return ((dx * dx + dy * dy) <= this->radius_squared );
}

void addPoint(CP_Point2D p , vector<DelaunayTriangle> &triangles)
{
	//set<DelaunayEdge> egdes;
	vector<DelaunayEdge> egdesList;
	vector<DelaunayTriangle>::iterator it;
	for (it = triangles.begin(); it != triangles.end(); )
	{
		DelaunayTriangle triange = *it;
		bool flag = true;
		if (triange.includingPoint(p))
		{
			//删除这些三角形，把边记录下来
			DelaunayEdge egde0 = DelaunayEdge(triange.p0, triange.p1);
			DelaunayEdge egde1 = DelaunayEdge(triange.p1, triange.p2);
			DelaunayEdge egde2 = DelaunayEdge(triange.p2, triange.p0);
			//egdes.insert(egde0);
			//egdes.insert(egde1);
			//egdes.insert(egde2);
			egdesList.push_back(egde0);
			egdesList.push_back(egde1);
			egdesList.push_back(egde2);
			
			flag = false;
			it = triangles.erase(it); //erase 后，指针后移一个
		}
		if(flag){
			it++;
		}
	}
	egdesList = removeDoubleEgdes(egdesList);
	/*
	set<DelaunayEdge>::iterator it;
	//再把这个点于 边连接得到新的三角形
	for (it = egdes.begin(); it != egdes.end(); it++)
	{
		DelaunayEdge egde = *it;
		DelaunayTriangle triange = DelaunayTriangle(egde.p0, egde.p1, p);
		triangles.push_back(triange);
	}*/
	for(int i = 0; i < egdesList.size(); i++)
	{
		DelaunayEdge egde = egdesList[i];
		DelaunayTriangle triangle = DelaunayTriangle(egde.p0, egde.p1, p);
		triangles.push_back(triangle);
	}
	
}
//不想改变CP_Point的原始实现，重写==
bool pointEquals(CP_Point2D a, CP_Point2D b)
{
	return (a.m_x == b.m_x) && (a.m_y == b.m_y);
}

//Bowyer-Watson算法
vector<DelaunayTriangle> doDelaunayTriangulate(vector<CP_Point2D> points)
{
	vector<DelaunayTriangle> triangles;

	DelaunayTriangle bigTriangle = createBigTriangle(points);
	for (int i = 0; i<points.size(); i++)
	{
		if(! bigTriangle.includingPoint(points[i])){
			exit(-1);
		}
	}

	glColor3d(0.0,0.0,1.0);
	bigTriangle.draw();
	triangles.push_back(bigTriangle);

	for (int i = 0; i < points.size(); i++)
	{
		CP_Point2D p = points[i];
		
		addPoint(p, triangles);
	}
	/*
	vector<DelaunayTriangle> result;
	//删除外围的大三角形及其相连接的边
	for (int i = 0; i < triangles.size(); i++)
	{
		DelaunayTriangle triangle = triangles[i];
		//glColor3d(0.0,0.0,0.0);
		//triangle.draw();
		//类似的操作	
		//set<CP_Point2D> pSets = set(bigTriangle.p0, bigTriangle.p1, bigTriangle.p2);
		//if (triangle.p0 in pSets || triangle.p1 in pSets || triangle.p2 in pSets)
		if(pointEquals(triangle.p0, bigTriangle.p0) || pointEquals(triangle.p0, bigTriangle.p1) || pointEquals(triangle.p0, bigTriangle.p2)
			|| pointEquals(triangle.p1, bigTriangle.p0) || pointEquals(triangle.p1, bigTriangle.p1) || pointEquals(triangle.p1, bigTriangle.p2) 
			|| pointEquals(triangle.p2, bigTriangle.p0) || pointEquals(triangle.p2, bigTriangle.p1) || pointEquals(triangle.p2, bigTriangle.p2)) 
		{
			//attention return iterator.erase后 triangle有变
			//triangles.erase(triangles.begin()+ i);
		}else{
			result.push_back(triangle);
		}
	}
	return result;
	*/
	vector<DelaunayTriangle>::iterator it;
	for (it = triangles.begin(); it != triangles.end();)
	{
		DelaunayTriangle triangle = *it;
		bool flag = true;
		if ( (triangle.p0 == bigTriangle.p0) || (triangle.p0 == bigTriangle.p1) || (triangle.p0 == bigTriangle.p2)||
			(triangle.p1 == bigTriangle.p0) || (triangle.p1 == bigTriangle.p1) || (triangle.p1 == bigTriangle.p2)||
			(triangle.p2 == bigTriangle.p0) || (triangle.p2 == bigTriangle.p1) || (triangle.p2 == bigTriangle.p2))
		{
			it = triangles.erase(it);
			flag = false;
		}
		if(flag)
			it++;
	}
	return triangles;
}

DelaunayTriangle createBigTriangle(vector<CP_Point2D> points)
{
	double max_x=0.0, max_y=0.0, min_x=0.0, min_y=0.0;
	for(int i = 0; i < points.size(); i++)
	{
		CP_Point2D p = points[i];
		max_x = p.m_x > max_x ? p.m_x : max_x;
		max_y = p.m_y > max_y ? p.m_y : max_y;
		min_x = p.m_x < min_x ? p.m_x : min_x;
		min_y = p.m_y < min_y ? p.m_y : min_y;
	}
	//大点儿没事

	double dx = (max_x - min_x);
	double dy = (max_y - min_y);
	//左下角
	CP_Point2D p0 = CP_Point2D(min_x - dx, min_y - dy*3);
	//上顶点
	CP_Point2D p1 = CP_Point2D(min_x - dx,max_y + dy);
	//右顶点
	CP_Point2D p2 = CP_Point2D(max_x + dx*3, max_y + dy);

	DelaunayTriangle triangle = DelaunayTriangle(p0, p1, p2);

	return triangle;
}
