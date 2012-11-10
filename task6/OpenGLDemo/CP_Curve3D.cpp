// CP_Curve3D.cpp: 实现CP_Curve3D、CP_LineSegment3D、CP_Circle3D、CP_Arc3D
#include "stdafx.h"
#include "CP_Curve3D.h"
#include "GL/GLU.H" // 已经包含GL/GL.h

#include <math.h>

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_LineSegment3D开始
CP_LineSegment3D::CP_LineSegment3D(double x1, double y1, double z1, double x2, double y2, double z2)
    :m_startingPoint(x1, y1, z1), m_endingPoint(x2, y2, z2)
{
} // 类CP_LineSegment3D构造函数结束

CP_LineSegment3D::CP_LineSegment3D(const CP_Point3D& s, const CP_Point3D& e)
    :m_startingPoint(s.m_x, s.m_y, s.m_z), m_endingPoint(e.m_x, e.m_y, e.m_z)
{
} // 类CP_LineSegment3D构造函数结束

CP_Point3D CP_LineSegment3D::mf_getStartingPoint( )
{
    return m_startingPoint;
} // 成员函数mf_getStartingPoint结束

CP_Point3D CP_LineSegment3D::mf_getEndingPoint( )
{
    return m_endingPoint;
} // 成员函数mf_getEndingPoint结束

CP_Point3D CP_LineSegment3D::mf_getPoint(double t) const
{
    return m_startingPoint + (m_endingPoint - m_startingPoint) * t;
} // 成员函数mf_getPoint结束

void CP_LineSegment3D::mf_draw( )
{
    glLineWidth(2.25f);
    glBegin(GL_LINES);
        glVertex3d(m_startingPoint.m_x, m_startingPoint.m_y, m_startingPoint.m_z);
        glVertex3d(m_endingPoint.m_x, m_endingPoint.m_y, m_endingPoint.m_z);
    glEnd();
} // 成员函数mf_draw结束

// 实现类CP_LineSegment3D结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Circle3D开始
CP_Circle3D::CP_Circle3D( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0)
{
} // 类CP_Circle3D构造函数结束

CP_Circle3D::CP_Circle3D(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r)
    : m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r)
{
} // 类CP_Circle3D构造函数结束

CP_Point3D CP_Circle3D::mf_getStartingPoint( )
{
    return m_center + m_axisX * m_radius;
} // 成员函数mf_getStartingPoint结束

CP_Point3D CP_Circle3D::mf_getEndingPoint( )
{
    return m_center + m_axisX * m_radius;
} // 成员函数mf_getEndingPoint结束

CP_Point3D CP_Circle3D::mf_getPoint(double t) const
{
    double a = t * PI2;
    return (m_center+m_axisX*(cos(a)*m_radius)+m_axisY*(sin(a)*m_radius));
} // 成员函数mf_getPoint结束

void CP_Circle3D::mf_draw( )
{
    glLineWidth(2.2f);
    int numberOfSegment=40;
    double t = 0.0;
    double step = 1.0/numberOfSegment;
    CP_Point3D p;

    glLineWidth(2.2f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < numberOfSegment; i++, t+=step)            
    {
        p = mf_getPoint(t);
        glVertex3d(p.m_x, p.m_y, p.m_z);
    } // for循环结构结束
    glEnd();
} // 成员函数mf_draw结束

// 实现类CP_Circle3D结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Arc3D开始
CP_Arc3D::CP_Arc3D( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0), m_startintAngle(0.0), m_endintAngle(PI_2)
{
} // 类CP_Arc3D构造函数结束

CP_Arc3D::CP_Arc3D(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r, double sa, double ea)
    : m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r), m_startintAngle(sa), m_endintAngle(ea) 
{
} // 类CP_Arc3D构造函数结束

CP_Point3D CP_Arc3D::mf_getStartingPoint( )
{
    return m_center + m_axisX * (cos(m_startintAngle)*m_radius) + m_axisY * (sin(m_startintAngle) * m_radius);
} // 成员函数mf_getStartingPoint结束

CP_Point3D CP_Arc3D::mf_getEndingPoint( )
{
    return m_center + m_axisX * (cos(m_endintAngle)*m_radius) + m_axisY * (sin(m_endintAngle) * m_radius);
} // 成员函数mf_getEndingPoint结束

CP_Point3D CP_Arc3D::mf_getPoint(double t) const
{
    double a = m_startintAngle+(m_endintAngle-m_startintAngle)*t;
    return m_center + m_axisX * (cos(a)*m_radius) + m_axisY * (sin(a) * m_radius);
} // 成员函数mf_getPoint结束

void CP_Arc3D::mf_draw( )
{
    glLineWidth(2.2f);
    int numberOfSegment=int(fabs(m_endintAngle-m_startintAngle)/PI2*40);
    if (numberOfSegment<5)
        numberOfSegment=5;
    double t = 0.0;
    double step = 1.0/numberOfSegment;
    CP_Point3D p;

    glLineWidth(2.2f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= numberOfSegment; i++, t+=step)            
    {
        p = mf_getPoint(t);
        glVertex3d(p.m_x, p.m_y, p.m_z);
    } // for循环结构结束
    glEnd();
} // 成员函数mf_draw结束
// 实现类CP_Arc3D结束
// ////////////////////////////////////////////////////////////////////////////
