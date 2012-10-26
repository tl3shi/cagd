// CP_Surface3D.h: 定义CP_Surface3D、CP_Plane、CP_Sphere、CP_Cylinder
#ifndef CP_SURFACE3D_H
#define CP_SURFACE3D_H

#include "CP_PointVector.h"

class CP_Surface3D
{
public:
    CP_Surface3D( ) { };
    virtual ~CP_Surface3D( ) { };
    virtual CP_Point3D mf_getPoint(double u, double v) const=0;
    virtual CP_Point3D mf_getPoint(const CP_Point2D& uv) const=0;
    virtual CP_Vector3D mf_getNormal(double u, double v) const=0;
    virtual CP_Vector3D mf_getNormal(const CP_Point2D& uv) const=0;
    virtual CP_Vector3D mf_getUnitNormal(double u, double v) const=0;
    virtual CP_Vector3D mf_getUnitNormal(const CP_Point2D& uv) const=0;

    virtual void mf_drawSolid(bool normalFlag=true)=0; // normalFlag: true(法向量取正向); false(法向量取反向)
    virtual void mf_drawWireframe( )=0;
}; // 类CP_Surface3D定义结束

// 参数方程为: S(u, v)=m_center + m_axisX*u + m_axisY*v
class CP_Plane : public CP_Surface3D
{
public:
    CP_Point3D m_center;
    CP_Vector3D m_axisX;
    CP_Vector3D m_axisY;
public:
    CP_Plane( );
    CP_Plane(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy);
    virtual ~CP_Plane( ) { };
    virtual CP_Point3D mf_getPoint(double u, double v) const;
    virtual CP_Point3D mf_getPoint(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getNormal(double u, double v) const;
    virtual CP_Vector3D mf_getNormal(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getUnitNormal(double u, double v) const;
    virtual CP_Vector3D mf_getUnitNormal(const CP_Point2D& uv) const;

    virtual void mf_drawSolid(bool normalFlag=true); // normalFlag: true(法向量取正向); false(法向量取反向)
    virtual void mf_drawWireframe( );
}; // 类CP_Plane定义结束

// 参数方程为: S(u, v)=m_center + m_axisX*x + m_axisY*y +vz*z
// 其中: x = m_radius * sin(v * PI) * cos(u * PI2);
//       y = m_radius * sin(v * PI) * sin(u * PI2);
//         z = m_radius * cos(v * PI);
//       vz = m_axisX ^ m_axisY;
// 参数u和v的基本定义域为u, v \in [0, 1]。
class CP_Sphere : public CP_Surface3D
{
public:
    CP_Point3D m_center;
    CP_Vector3D m_axisX;
    CP_Vector3D m_axisY;
    double  m_radius;
public:
    CP_Sphere( );
    CP_Sphere(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r);
    virtual ~CP_Sphere( ) { };
    virtual CP_Point3D mf_getPoint(double u, double v) const;
    virtual CP_Point3D mf_getPoint(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getNormal(double u, double v) const;
    virtual CP_Vector3D mf_getNormal(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getUnitNormal(double u, double v) const;
    virtual CP_Vector3D mf_getUnitNormal(const CP_Point2D& uv) const;

    virtual void mf_drawSolid(bool normalFlag=true); // normalFlag: true(法向量取正向); false(法向量取反向)
    virtual void mf_drawWireframe( );
}; // 类CP_Sphere定义结束

// 参数方程为: m_center + m_axisX*x + m_axisY*y +vz*z
// 其中, x = m_radius * cos(u * PI2);
//       y = m_radius * sin(u * PI2);
//       z = v * m_height;
//       vz = m_axisX ^ m_axisY;
// 参数u和v的基本定义域为u, v \in [0, 1]。
class CP_Cylinder : public CP_Surface3D
{
public:
    CP_Point3D m_center;
    CP_Vector3D m_axisX;
    CP_Vector3D m_axisY;
    double   m_radius;
    double   m_height;
public:
    CP_Cylinder( );
    CP_Cylinder(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r, double h);
    virtual ~CP_Cylinder( ) { };
    virtual CP_Point3D mf_getPoint(double u, double v) const;
    virtual CP_Point3D mf_getPoint(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getNormal(double u, double v) const;
    virtual CP_Vector3D mf_getNormal(const CP_Point2D& uv) const;
    virtual CP_Vector3D mf_getUnitNormal(double u, double v) const;
    virtual CP_Vector3D mf_getUnitNormal(const CP_Point2D& uv) const;

    virtual void mf_drawSolid(bool normalFlag=true); // normalFlag: true(法向量取正向); false(法向量取反向)
    virtual void mf_drawWireframe( );
}; // 类CP_Cylinder定义结束

#endif