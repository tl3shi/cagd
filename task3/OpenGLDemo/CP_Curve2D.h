// CP_Curve2D.h: 定义CP_Curve2D、CP_Circle2D、CP_Arc2D
#ifndef CP_CURVE2D_H
#define CP_CURVE2D_H

#include "CP_PointVector.h"

class CP_Curve2D
{
public:
    CP_Curve2D( ) { };
    virtual ~CP_Curve2D( ) { };
    virtual CP_Point2D mf_getStartingPoint( )=0;
    virtual CP_Point2D mf_getEndingPoint( )=0;
    virtual CP_Point2D mf_getPoint(double t)=0;
    virtual void mf_draw( )=0;
}; // 类CP_Curve2D定义结束
// 参数方程为: C(t)=m_center+m_axisX*(cos(t*2PI)*m_radius) 
//                  +m_axisY*(sin(t*2PI)*m_radius)
// 参数t应当介于0和1之间

class CP_Circle2D : public CP_Curve2D
{
public:
    CP_Point2D m_center;
    CP_Vector2D m_axisX;
    CP_Vector2D m_axisY;

    double m_radius;     // 圆的半径
public:
    CP_Circle2D( );
    CP_Circle2D(const CP_Point2D& c, const CP_Vector2D& vx, const CP_Vector2D& vy, double r);
    virtual~CP_Circle2D( ) { };

    virtual CP_Point2D mf_getStartingPoint( );
    virtual CP_Point2D mf_getEndingPoint( );
    virtual CP_Point2D mf_getPoint(double t); // 参数t应当介于0和1之间
    virtual void mf_draw( );
};

// 参数方程为: C(t)=m_center + m_axisX * (cos(a)*m_radius) 
//                  + m_axisY * (sin(a) * m_radius),
//其中a=m_startintAngle+(m_endintAngle-m_startintAngle)*t;
// 参数t应当介于0和1之间
class CP_Arc2D : public CP_Curve2D
{
public:
    CP_Point2D m_center;
    CP_Vector2D m_axisX;
    CP_Vector2D m_axisY;

    double m_radius;     // 圆的半径
    double m_startintAngle, m_endintAngle;
    // 圆弧的起始角和终止角, 单位为弧度
public:
    CP_Arc2D( );
    CP_Arc2D(const CP_Point2D& c, const CP_Vector2D& vx, const CP_Vector2D& vy, double r, double sa, double ea);
    virtual ~CP_Arc2D( ) { };

    virtual CP_Point2D mf_getStartingPoint( );
    virtual CP_Point2D mf_getEndingPoint( );
    virtual CP_Point2D mf_getPoint(double t);
    // 参数t应当介于0和1之间
    virtual void mf_draw( );
};

#endif

