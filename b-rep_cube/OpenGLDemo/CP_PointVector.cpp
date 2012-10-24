// CP_PointVector.cpp 实现类CP_Point2D、CP_Point3D、CP_Vector2D和CP_Vector3D
#include "stdafx.h"
#include "CP_PointVector.h"

#include <math.h>

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Point2D开始
CP_Point2D::CP_Point2D(double newx, double newy):m_x(newx), m_y(newy)
{
} // 类CP_Point2D构造函数结束
// 实现类CP_Point2D结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Point3D开始
CP_Point3D::CP_Point3D(double newx, double newy, double newz):m_x(newx), m_y(newy), m_z(newz)
{
} // 类CP_Point3D构造函数结束
// 实现类CP_Point3D结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Point3D开始
CP_Vector2D::CP_Vector2D (double newx, double newy):m_x(newx), m_y(newy)
{
} // 类CP_Vector2D构造函数结束

CP_Vector2D& CP_Vector2D::operator += (const CP_Vector2D& v)
{ 
    m_x += v.m_x;
    m_y += v.m_y; 
    return *this; 
} //成员函数operator +=结束

CP_Vector2D& CP_Vector2D::operator -= (const CP_Vector2D& v)
{ 
    m_x -= v.m_x;
    m_y -= v.m_y; 
    return *this; 
} //成员函数operator -=结束

CP_Vector2D& CP_Vector2D::operator *= (double num)
{
    m_x *= num;
    m_y *= num; 
    return *this; 
} //成员函数operator *=结束

CP_Vector2D& CP_Vector2D::operator /= (double num)
{
    m_x /= num;  // 注意这里没有处理除数为0的情形
    m_y /= num; 
    return *this;
} //成员函数operator /=结束

double CP_Vector2D::operator ^(const CP_Vector2D& v)
{
    return( m_x* v.m_y- m_y* v.m_x );
} //成员函数operator ^结束

CP_Vector2D CP_Vector2D::operator - () const
{ 
    return CP_Vector2D (-m_x, -m_y); 
} //成员函数operator -结束

double CP_Vector2D::mf_getLength( )  const                             
{ 
    return sqrt(m_x*m_x + m_y*m_y); 
} //成员函数mf_getLength结束

CP_Vector2D CP_Vector2D::mf_getPerpendicularVector() const
{
    return CP_Vector2D(-m_y, m_x);
} //成员函数mf_getPerpendicularVector结束

void CP_Vector2D::mf_normalize( )
{
    double a = mf_getLength( );
    (*this) /= a; // 注意: 这里没有处理当长度为0的情况
} //成员函数mf_normalize结束

void CP_Vector2D::mf_setValue(double newx, double newy)
{
    m_x=newx;
    m_y=newy;
} //成员函数mf_setValue结束

// 实现类CP_Vector2D结束
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Vector3D开始
CP_Vector3D::CP_Vector3D (double newx, double newy, double newz):m_x(newx), m_y(newy), m_z(newz)
{
} // 类CP_Vector3D构造函数结束

CP_Vector3D& CP_Vector3D::operator += (const CP_Vector3D& v)
{ 
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;  
    return *this; 
} //成员函数operator +=结束

CP_Vector3D& CP_Vector3D::operator -= (const CP_Vector3D& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z; 
    return *this; 
} //成员函数operator -=结束

CP_Vector3D& CP_Vector3D::operator *= (double num)
{ 
    m_x *= num;
    m_y *= num;
    m_z *= num; 
    return *this; 
} //成员函数operator *=结束

CP_Vector3D& CP_Vector3D::operator /= (double num)
{
    num = 1.0/num;
    m_x *= num;
    m_y *= num;
    m_z *= num;
    return *this;
} //成员函数operator /=结束

CP_Vector3D& CP_Vector3D::operator ^= (const CP_Vector3D& v)
{ 
    double a =   m_y * v.m_z - m_z * v.m_y;
    double b = - m_x * v.m_z + m_z * v.m_x;
    double c =   m_x * v.m_y - m_y * v.m_x;

    m_x = a;
    m_y = b;
    m_z = c;
    return *this;
} //成员函数operator ^=结束

CP_Vector3D CP_Vector3D::operator - ( ) const
{ 
    return CP_Vector3D (-m_x, -m_y, -m_z); 
} //成员函数operator -结束

double CP_Vector3D::mf_getLength( )  const                             
{ 
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z); 
} //成员函数mf_getLength结束

CP_Vector3D CP_Vector3D::mf_getPerpendicularVector( ) const
{
    CP_Vector3D vecReturn;
    if(    fabs(m_y)<fabs(m_z))
    {
        vecReturn.m_x=m_z;
        vecReturn.m_y=0.0;
        vecReturn.m_z=-m_x;
    }
    else
    {
        vecReturn.m_x=-m_y;
        vecReturn.m_y=m_x;
        vecReturn.m_z=0.0;
    }
    return vecReturn;
} //成员函数mf_getPerpendicularVector结束

void CP_Vector3D::mf_normalize( )
{
    double a = mf_getLength( );
    (*this) /= a; // 注意: 这里没有处理除数为0的情况
} //成员函数mf_normalize结束

void CP_Vector3D::mf_setValue(double newx, double newy, double newz)
{
    m_x=newx;
    m_y=newy;
    m_z=newz;
} //成员函数mf_setValue结束

// 实现类CP_Vector3D结束
// ////////////////////////////////////////////////////////////////////////////

CP_Point2D operator + (const CP_Point2D& p, const CP_Vector2D& v)
{
    return CP_Point2D (p.m_x + v.m_x, p.m_y + v.m_y); 
} //函数operator +结束

CP_Point2D operator - (const CP_Point2D& pt, const CP_Vector2D& v)
{
    return CP_Point2D (pt.m_x - v.m_x, pt.m_y - v.m_y); 
} //函数operator -结束

CP_Vector2D operator - (const CP_Point2D& p, const CP_Point2D& q)
{
    return CP_Vector2D (p.m_x - q.m_x, p.m_y - q.m_y); 
} //函数operator -结束

CP_Point3D operator + (const CP_Point3D& pt, const CP_Vector3D& v)
{
    return CP_Point3D (pt.m_x + v.m_x, pt.m_y + v.m_y, pt.m_z + v.m_z); 
} //函数operator +结束

CP_Point3D operator - (const CP_Point3D& pt, const CP_Vector3D& v)
{
    return CP_Point3D (pt.m_x - v.m_x, pt.m_y - v.m_y, pt.m_z - v.m_z); 
} //函数operator -结束

CP_Vector3D operator - (const CP_Point3D& p, const CP_Point3D& q)
{
    return CP_Vector3D (p.m_x - q.m_x, p.m_y - q.m_y, p.m_z - q.m_z); 
} //函数operator -结束

CP_Vector2D operator + (const CP_Vector2D& u, const CP_Vector2D& v)
{
    return CP_Vector2D (u.m_x + v.m_x, u.m_y + v.m_y); 
} //函数operator +结束

CP_Vector2D operator - (const CP_Vector2D& u, const CP_Vector2D& v)
{
    return CP_Vector2D (u.m_x - v.m_x, u.m_y - v.m_y);
} //函数operator -结束

// 点积
double  operator * (const CP_Vector2D& u, const CP_Vector2D& v)
{
    return u.m_x * v.m_x + u.m_y * v.m_y;
} //函数operator *结束

CP_Vector2D operator * (const CP_Vector2D& v, double num)
{
    return CP_Vector2D (v.m_x * num, v.m_y * num);
} //函数operator *结束

CP_Vector2D operator / (const CP_Vector2D& v, double num)
{
    return CP_Vector2D (v.m_x / num, v.m_y / num); // 注意: 这里没有处理除数为0的情况
} //函数operator /结束

CP_Vector3D operator + (const CP_Vector3D& u, const CP_Vector3D& v)
{
   return CP_Vector3D(u.m_x + v.m_x, u.m_y + v.m_y, u.m_z + v.m_z);
} //函数operator +结束

CP_Vector3D operator - (const CP_Vector3D& u, const CP_Vector3D& v)
{
    return CP_Vector3D (u.m_x - v.m_x, u.m_y - v.m_y, u.m_z - v.m_z);
} //函数operator -结束

// 点积
double operator * (const CP_Vector3D& u, const CP_Vector3D& v)
{
    return (u.m_x * v.m_x+u.m_y * v.m_y+ u.m_z * v.m_z);
} //函数operator *结束

// 叉积
CP_Vector3D operator ^ (const CP_Vector3D& u, const CP_Vector3D& v)
{
    return CP_Vector3D(u.m_y * v.m_z - u.m_z*v.m_y, 
                       -u.m_x*v.m_z+u.m_z*v.m_x,
                       u.m_x*v.m_y-u.m_y*v.m_x
                      );
} //函数operator ^结束

CP_Vector3D operator * (const CP_Vector3D& v, double num)
{
    return CP_Vector3D (v.m_x * num, v.m_y * num, v.m_z * num);
} //函数operator *结束

CP_Vector3D operator / (const CP_Vector3D& v, double num)
{
    num = 1.0/num; // 注意: 这里没有处理除数为0的情况
    return CP_Vector3D (v.m_x * num, v.m_y * num, v.m_z * num);
} //函数operator /结束


