#include "StdAfx.h"
#include "CP_PointVector.h"
#include <math.h>


CP_Point2D::CP_Point2D (double newx /*= 0.0*/, double newy /*= 0.0*/) {
	m_x = newx;
	m_y = newy;
}


CP_Point3D::CP_Point3D(double newx /*= 0.0*/, double newy /*= 0.0*/ , double newz /*= 0.0*/) {
	m_x = newx;
	m_y = newy;
	m_z = newz;
}

CP_Vector2D::CP_Vector2D(double newx/* =0.0 */, double newy/* =0.0 */) {
	m_x = newx;
	m_y = newy;
}

CP_Vector2D::CP_Vector2D(const CP_Vector2D& v) {
	m_x = v.m_x;
	m_x = v.m_y;
}

CP_Vector2D& CP_Vector2D::operator += (const CP_Vector2D& v) {
	m_x += v.m_x;
	m_y += v.m_y;
	return *this;
}
CP_Vector2D& CP_Vector2D::operator -= (const CP_Vector2D& v) {
	m_x -= v.m_x;
	m_y -= v.m_y;
	return *this;
}
CP_Vector2D& CP_Vector2D::operator *= (double num) {
	m_x *= num;
	m_y *= num;
	return *this;
}
CP_Vector2D& CP_Vector2D::operator /= (double num) {
	m_x /= num;
	m_y /= num;
	return *this;
}
double CP_Vector2D::operator ^(const CP_Vector2D& v) {
	return m_x * v.m_x - m_y * v.m_x;
}

// 单目减
CP_Vector2D CP_Vector2D::operator - ( ) const {
	return CP_Vector2D(- m_x, - m_y);
}

// 取长度
double CP_Vector2D::mf_getLength( ) const {
	return sqrt( pow(m_x, 2) + pow(m_y, 2));
}

//得到一个垂直的向量
CP_Vector2D CP_Vector2D::mf_getPerpendicularVector( ) const {
	return CP_Vector2D(m_y / mf_getLength(), 
		- m_x / mf_getLength());
}

// 单位化
void CP_Vector2D::mf_normalize( ) {
	m_x /= mf_getLength();
	m_y /= mf_getLength();
} 

void CP_Vector2D::mf_setValue(double newx /*=0.0*/, double newy /*=0.0*/) {
	m_x = newx;
	m_y = newy;
}

CP_Vector3D::CP_Vector3D (double newx /*=0.0*/, double newy /*=0.0*/, double newz /*=0.0*/) {
	m_x = newx;
	m_y = newy;
	m_z = newz;
}

CP_Vector3D::CP_Vector3D(const CP_Vector3D& v) {
	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;
}

//赋值操作
CP_Vector3D& CP_Vector3D::operator += (const CP_Vector3D& v) {
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;
	return *this;
}

CP_Vector3D& CP_Vector3D::operator -= (const CP_Vector3D& v) {
	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;
	return *this;
}

CP_Vector3D& CP_Vector3D::operator *= (double num) {
	m_x *= num;
	m_y *= num;
	m_z *= num;
	return *this;
}

CP_Vector3D& CP_Vector3D::operator /= (double num) {
	m_x /= num;
	m_y /= num;
	m_z /= num;
	return *this;
}
 
CP_Vector3D& CP_Vector3D::operator ^= (const CP_Vector3D& v) {
	double tmpX = m_x;
	double tmpY = m_y;
	double tmpZ = m_z;
	m_x = tmpY * v.m_z - tmpZ * v.m_y;
	m_y = tmpZ * v.m_x - tmpX * v.m_z;
	m_z = tmpX * v.m_y - tmpY * v.m_x;
	return *this;	
}

//单目减
CP_Vector3D CP_Vector3D::operator - ( ) const {
	return CP_Vector3D(-m_x, -m_y, -m_z);
}

// 取长度
double CP_Vector3D::mf_getLength ( ) const {
	return sqrt(pow(m_x, 2) + pow(m_y, 2) + pow(m_z, 2));
}

//得到一个垂直的向量
CP_Vector3D CP_Vector3D::mf_getPerpendicularVector( ) const {
	return CP_Vector3D(m_y - m_z, m_z - m_x, m_x - m_y);
} 

// 单位化
void CP_Vector3D::mf_normalize( ) {
	m_x /= mf_getLength();
	m_y /= mf_getLength();
	m_z /= mf_getLength();
}

void CP_Vector3D::mf_setValue(double newx /*=0.0*/, double newy /*=0.0*/, double newz /*=0.0*/) {
	m_x = newx;
	m_y = newy;
	m_z = newz;
}