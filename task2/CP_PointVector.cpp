#include "CP_PointVector.h"
#include <cmath>
#include <assert.h>

CP_Point2D::CP_Point2D(double newx, double newy)
{
    m_x = newx;
    m_y = newy;
}
CP_Point3D::CP_Point3D(double newx, double newy, double newz)
{
    m_x = newx;
    m_y = newy;
    m_z = newz;
}

CP_Vector2D::CP_Vector2D(double newx, double newy)
{
    m_x = newx;
    m_y = newy;
}

CP_Vector2D& CP_Vector2D::operator+=(const CP_Vector2D& v)
{
   this->m_x += v.m_x; 
   this->m_y += v.m_y;
   return *this;
}

CP_Vector2D& CP_Vector2D::operator -= (const CP_Vector2D& v)
{
    this->m_x -= v.m_x;
    this->m_y -= v.m_y;
    return *this;
}


CP_Vector2D& CP_Vector2D::operator *= (double num)
{
    this->m_x *= num;
    this->m_y *= num;
    return *this;
}
CP_Vector2D& CP_Vector2D::operator /= (double num)
{
    /*
    if ((num - 0)< 0.0000000001){
        throw exception("Divisor cannot be 0");
    }*/
    assert(num != 0.0);
    this->m_x /= num;
    this->m_y /= num;
    return *this;
}
double CP_Vector2D::operator ^(const CP_Vector2D& v)
{
    return m_x * v.m_x + m_y * v.m_y;
}
//单目减
CP_Vector2D CP_Vector2D::operator -() const
{
    //取反?
    return CP_Vector2D(-m_x, -m_y);
}
double CP_Vector2D::mf_getLength( ) const // 取长度
{
    return sqrt(m_x * m_x + m_y * m_y);
}
CP_Vector2D CP_Vector2D::mf_getPerpendicularVector() const //得到一个垂直的向量
{
    CP_Vector2D temp;
    temp.m_x = 1;
    temp.m_y = 1 / m_y * m_x;
    temp.mf_normalize();
    return temp;
}
void CP_Vector2D::mf_normalize() // 单位化
{
    double length = mf_getLength();
    if (length != 0.0)
    {
        m_x = m_x / length;
        m_y = m_y / length;
    }else{
        //TODO
    }
}
void CP_Vector2D::mf_setValue(double newx, double newy)
{
    m_x = newx;
    m_y = newy;
}

CP_Vector3D::CP_Vector3D (double newx, double newy, double newz)
{
    m_x = newx;
    m_y = newy;
    m_z = newz;
}

    //赋值操作
CP_Vector3D& CP_Vector3D::operator += (const CP_Vector3D& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
}
CP_Vector3D& CP_Vector3D::operator -= (const CP_Vector3D& v)
{ m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
}
CP_Vector3D& CP_Vector3D::operator *= (double num)
{
    m_x *= num;
    m_y *= num;
    m_z *= num;
}
CP_Vector3D& CP_Vector3D::operator /= (double num)
{
    assert(num != 0.0);
    m_x /= num;
    m_y /= num;
    m_z /= num;
 
}
CP_Vector3D& CP_Vector3D::operator ^= (const CP_Vector3D& v)
{
    m_x *= v.m_x;
    m_y *= v.m_y;
    m_z *= v.m_z;
}

//单目减
CP_Vector3D CP_Vector3D::operator - () const
{
    return CP_Vector3D(-m_x, -m_y, -m_z);
}

double CP_Vector3D::mf_getLength () const // 取长度
{
    return sqrt(pow(m_x,2.0) + pow(m_y,2.0) + pow(m_z,2.0));
}
CP_Vector3D CP_Vector3D::mf_getPerpendicularVector( ) const //得到一个垂直的向量
{
    //TODO
}

void CP_Vector3D::mf_normalize() // 单位化
{
    double length = mf_getLength();
    if (length != 0.0)
    {
        m_x = m_x / length;
        m_y = m_y / length;
        m_z = m_z / length;
    }else{
        //TODO    
    }
}
void CP_Vector3D::mf_setValue(double newx, double newy, double newz)
{
    m_x = newx;
    m_y = newy;
    m_z = newz;
}

extern CP_Point2D operator + (const CP_Point2D& pt, const CP_Vector2D& v)
{
    CP_Point2D point;
    point.m_x = pt.m_x + v.m_x;
    point.m_y = pt.m_y + v.m_y;
    return point;
}
extern CP_Point2D operator - (const CP_Point2D& pt, const CP_Vector2D& v)
{
    CP_Point2D point;
    point.m_x = pt.m_x - v.m_x;
    point.m_y = pt.m_y - v.m_y;
    return point;
}
extern CP_Vector2D operator - (const CP_Point2D& p, const CP_Point2D& q)
{
    CP_Vector2D vector;
    vector.m_x = p.m_x - q.m_x;
    vector.m_y = p.m_x - q.m_y;
    return vector;
}
extern CP_Point3D operator + (const CP_Point3D& pt, const CP_Vector3D& v)
{
    CP_Point3D point;
    point.m_x = pt.m_x + v.m_x;
    point.m_y = pt.m_y + v.m_y;
    point.m_z = pt.m_z + v.m_z;
    return point;

}
extern CP_Point3D operator - (const CP_Point3D& pt, const CP_Vector3D& v)
{
    CP_Point3D point;
    point.m_x = pt.m_x - v.m_x;
    point.m_y = pt.m_y - v.m_y;
    point.m_z = pt.m_z - v.m_z;
    return point;

}
extern CP_Vector3D operator - (const CP_Point3D& p, const CP_Point3D& q)
{
    CP_Vector3D vector;
    vector.m_x = p.m_x - q.m_x;
    vector.m_z = p.m_z - q.m_z;
    vector.m_y = p.m_y - q.m_y;
    return vector;
}

extern CP_Vector2D operator + (const CP_Vector2D& u, const CP_Vector2D& v)
{
    CP_Vector2D vector;
    vector.m_x = u.m_x + v.m_x;
    vector.m_y = u.m_y + v.m_y;
    return vector;
}
extern CP_Vector2D operator - (const CP_Vector2D& u, const CP_Vector2D& v)
{
    CP_Vector2D vector;
    vector.m_x = u.m_x - v.m_x;
    vector.m_y = u.m_y - v.m_y;
    return vector;
} 
extern double  operator * (const CP_Vector2D& u, const CP_Vector2D& v) // 点积
{
    return u.m_x * v.m_x + u.m_y + v.m_y;
}
extern CP_Vector2D operator * (const CP_Vector2D& v, double num)
{
    CP_Vector2D vector;
    vector.m_x = num * v.m_x;
    vector.m_y = num * v.m_y;
    return vector;
    
}
extern CP_Vector2D operator / (const CP_Vector2D& v, double num)
{
    assert (num != 0.0);
    CP_Vector2D vector;
    vector.m_x =  v.m_x / num;
    vector.m_y = v.m_y / num;
    return vector;
}

extern CP_Vector3D operator + (const CP_Vector3D& u, const CP_Vector3D& v)
{
    CP_Vector3D vector;
    vector.m_x = u.m_x + v.m_x;
    vector.m_z = u.m_z + v.m_z;
    vector.m_y = u.m_y + v.m_y;
    return vector;

}
extern CP_Vector3D operator - (const CP_Vector3D& u, const CP_Vector3D& v)
{
    CP_Vector3D vector;
    vector.m_x = u.m_x - v.m_x;
    vector.m_z = u.m_z - v.m_z;
    vector.m_y = u.m_y - v.m_y;
    return vector;
}
extern double operator * (const CP_Vector3D& u, const CP_Vector3D& v) // 点积
{
    return u.m_x * v.m_x + u.m_y * v.m_y + u.m_z * v.m_z;   
}
extern CP_Vector3D operator ^ (const CP_Vector3D& u, const CP_Vector3D& v) // 叉积
{
    CP_Vector3D vector;
    vector.m_x = u.m_x * v.m_x;
    vector.m_z = u.m_z * v.m_z;
    vector.m_y = u.m_y * v.m_y;
    return vector;
}
extern CP_Vector3D operator * (const CP_Vector3D& v, double num)
{
    CP_Vector3D vector;
    vector.m_x = num * v.m_x;
    vector.m_z = num * v.m_z;
    vector.m_y = num * v.m_y;
    return vector;
}

extern CP_Vector3D operator / (const CP_Vector3D& v, double num)
{
    assert(num != 0.0);
    CP_Vector3D vector;
    vector.m_x = 1 / num * v.m_x;
    vector.m_z = 1 / num * v.m_z;
    vector.m_y = 1 / num * v.m_y;
    return vector;
}
