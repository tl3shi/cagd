#include "StdAfx.h"
#include "DelaunayTriangle.h"
#include "GL/GLU.H" // ÒÑ¾­°üº¬GL/GL.h



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
}

void DelaunayTriangle::draw()
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(this->p0.m_x, this->p0.m_y);
	glVertex2d(this->p1.m_x, this->p1.m_y);
	glVertex2d(this->p2.m_x, this->p2.m_y);
	glEnd();
}

