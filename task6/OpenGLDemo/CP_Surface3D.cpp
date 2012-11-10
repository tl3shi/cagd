// CP_Surface3D.cpp: 实现CP_Surface3D、CP_Plane、CP_Sphere、CP_Cylinder
#include "stdafx.h"
#include "CP_Surface3D.h"
#include "GL/GLU.H" // 已经包含GL/GL.h
#include <math.h>
// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Plane开始
CP_Plane::CP_Plane( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0)
{
} // 类CP_Plane构造函数结束
CP_Plane::CP_Plane(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z)
{
} // 类CP_Plane构造函数结束
CP_Point3D CP_Plane::mf_getPoint(double u, double v) const
{
	return m_center + m_axisX*u + m_axisY*v;
} // 成员函数mf_getPoint结束
CP_Point3D CP_Plane::mf_getPoint(const CP_Point2D& uv) const
{
	return m_center + m_axisX*uv.m_x + m_axisY*uv.m_y;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_Plane::mf_getNormal(double u, double v) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Plane::mf_getNormal(const CP_Point2D& uv) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Plane::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_Plane::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_Plane::mf_drawSolid(bool normalFlag)
{
	double u[]={0.0, 1.0, 1.0, 0.0, 0.0};
	double v[]={0.0, 0.0, 1.0, 1.0, 0.0};
	CP_Point3D p;
	CP_Vector3D vz = mf_getUnitNormal(0.0, 0.0);
	if (!normalFlag)
		vz*=(-1);
	glNormal3d(vz.m_x, vz.m_y, vz.m_z);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 3; i++)
	{
		p = mf_getPoint(u[i], v[i]);
		glVertex3d(p.m_x, p.m_y, p.m_z);
	} // for循环结构结束
	glEnd();
	glBegin(GL_POLYGON);
	for (int i = 2; i < 5; i++)
	{
		p = mf_getPoint(u[i], v[i]);
		glVertex3d(p.m_x, p.m_y, p.m_z);
	} // for循环结构结束
	glEnd();
} // 成员函数mf_drawSolid结束
void CP_Plane::mf_drawWireframe( )
{
	double u[]={0.0, 1.0, 1.0, 0.0, 0.0};
	double v[]={0.0, 0.0, 1.0, 1.0, 0.0};
	CP_Point3D p;
	glLineWidth(2.2f);
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 3; i++)
	{
		p = mf_getPoint(u[i], v[i]);
		glVertex3d(p.m_x, p.m_y, p.m_z);
	} // for循环结构结束
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int i = 2; i < 5; i++)
	{
		p = mf_getPoint(u[i], v[i]);
		glVertex3d(p.m_x, p.m_y, p.m_z);
	} // for循环结构结束
	glEnd();
} // 成员函数mf_drawWireframe结束
// 实现类CP_Plane结束
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Sphere开始
CP_Sphere::CP_Sphere( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(2.0)
{
} // 类CP_Sphere构造函数结束
CP_Sphere::CP_Sphere(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r)
{
} // 类CP_Sphere构造函数结束
CP_Point3D CP_Sphere::mf_getPoint(double u, double v) const
{
	double r1 = m_radius * sin(v * PI);
	double x = r1 * cos(u * PI2);
	double y = r1 * sin(u * PI2);
	double z = m_radius * cos(v * PI);
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_center + m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getPoint结束
CP_Point3D CP_Sphere::mf_getPoint(const CP_Point2D& uv) const
{
	double r1 = m_radius * sin(uv.m_y * PI);
	double x = r1 * cos(uv.m_x * PI2);
	double y = r1 * sin(uv.m_x * PI2);
	double z = m_radius * cos(uv.m_y * PI);
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_center + m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_Sphere::mf_getNormal(double u, double v) const
{
	double sv = sin(v * PI);
	double A=-2*PI*PI*m_radius*m_radius*sv;
	double x = A*sv * cos(u * PI2);
	double y = A*sv * sin(u * PI2);
	double z = A*cos(v * PI);
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Sphere::mf_getNormal(const CP_Point2D& uv) const
{
	double sv = sin(uv.m_y * PI);
	double A=-2*PI*PI*m_radius*m_radius*sv;
	double x = A*sv * cos(uv.m_x * PI2);
	double y = A*sv * sin(uv.m_x * PI2);
	double z = A*cos(uv.m_y * PI);
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Sphere::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = mf_getNormal(u, v);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_Sphere::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = mf_getNormal(uv);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_Sphere::mf_drawSolid(bool normalFlag)
{
	int i, j, k;
	int nu=40;
	int nv=20;
	double stepU=1.0/nu;
	double stepV=1.0/nv;
	double du, dv;
	CP_Point3D p, p0;
	CP_Vector3D v, v0;
	// 下端三角形组: 注意mf_getPoint(u, 0.0)均为同一个点
	p0=mf_getPoint(0.0, 0.0);
	v0=mf_getUnitNormal(0.0, 0.0);
	if (!normalFlag)
		v0*=(-1);
	for (j=0, du=0.0; j<nu; j++)
	{
		glBegin(GL_POLYGON);
		glNormal3d(v0.m_x, v0.m_y, v0.m_z);
		glVertex3d(p0.m_x, p0.m_y, p0.m_z);
		p=mf_getPoint(du, stepV);
		v=mf_getUnitNormal(du, stepV);
		if (!normalFlag)
			v*=(-1);
		glNormal3d(v.m_x, v.m_y, v.m_z);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		du+=stepU;
		p=mf_getPoint(du, stepV);
		v=mf_getUnitNormal(du, stepV);
		if (!normalFlag)
			v*=(-1);
		glNormal3d(v.m_x, v.m_y, v.m_z);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		glEnd();
	} // for循环结构结束
	// 中间三角形组
	for (i=2, dv=stepV; i<nv; i++, dv+=stepV)
	{
		for (j=0, du=0.0; j<nu; j++, du+=stepU)
		{
			double uu[ ]={du, du+stepU, du+stepU, du,       du};
			double vv[ ]={dv, dv,       dv+stepV, dv+stepV, dv};
			glBegin(GL_POLYGON);
			for (k=0; k<3; k++)
			{
				p=mf_getPoint(uu[k], vv[k]);
				v=mf_getUnitNormal(uu[k], vv[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			} // 内层(k)for循环结构结束
			glEnd();
			glBegin(GL_POLYGON);
			for (k=2; k<5; k++)
			{
				p=mf_getPoint(uu[k], vv[k]);
				v=mf_getUnitNormal(uu[k], vv[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			} // 内层(k)for循环结构结束
			glEnd();
		} // 内层(j)for循环结构结束
	} // 外层(i)for循环结构结束
	// 上端三角形组: 注意mf_getPoint(u, 1.0)均为同一个点
	p0=mf_getPoint(0.0, 1.0);
	v0=mf_getUnitNormal(0.0, 1.0);
	if (!normalFlag)
		v0*=(-1);
	for (j=0, du=0.0; j<nu; j++)
	{
		glBegin(GL_POLYGON);
		glNormal3d(v0.m_x, v0.m_y, v0.m_z);
		glVertex3d(p0.m_x, p0.m_y, p0.m_z);
		p=mf_getPoint(du, 1-stepV);
		v=mf_getUnitNormal(du, 1-stepV);
		if (!normalFlag)
			v*=(-1);
		glNormal3d(v.m_x, v.m_y, v.m_z);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		du+=stepU;
		p=mf_getPoint(du, 1-stepV);
		v=mf_getUnitNormal(du, 1-stepV);
		if (!normalFlag)
			v*=(-1);
		glNormal3d(v.m_x, v.m_y, v.m_z);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		glEnd();
	} // for循环结构结束
} // 成员函数mf_drawSolid结束
void CP_Sphere::mf_drawWireframe( )
{
	int i, j, k;
	int nu=40;
	int nv=20;
	double stepU=1.0/nu;
	double stepV=1.0/nv;
	double u, v;
	CP_Point3D p, p0;
	// 下端三角形组: 注意mf_getPoint(u, 0.0)均为同一个点
	glLineWidth(2.2f);
	p0=mf_getPoint(0.0, 0.0);
	for (j=0, u=0.0; j<nu; j++)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3d(p0.m_x, p0.m_y, p0.m_z);
		p=mf_getPoint(u, stepV);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		u+=stepU;
		p=mf_getPoint(u, stepV);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		glEnd();
	} // for循环结构结束
	// 中间三角形组
	for (i=2, v=stepV; i<nv; i++, v+=stepV)
	{
		for (j=0, u=0.0; j<nu; j++, u+=stepU)
		{
			double uu[ ]={u, u+stepU, u+stepU, u,       u};
			double vv[ ]={v, v,       v+stepV, v+stepV, v};
			glBegin(GL_LINE_LOOP);
			for (k=0; k<3; k++)
			{
				p=mf_getPoint(uu[k], vv[k]);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			} // 内层(k)for循环结构结束
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (k=2; k<5; k++)
			{
				p=mf_getPoint(uu[k], vv[k]);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			} // 内层(k)for循环结构结束
			glEnd();
		} // 内层(j)for循环结构结束
	} // 外层(i)for循环结构结束
	// 上端三角形组: 注意mf_getPoint(u, 1.0)均为同一个点
	p0=mf_getPoint(0.0, 1.0);
	for (j=0, u=0.0; j<nu; j++)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3d(p0.m_x, p0.m_y, p0.m_z);
		p=mf_getPoint(u, 1-stepV);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		u+=stepU;
		p=mf_getPoint(u, 1-stepV);
		glVertex3d(p.m_x, p.m_y, p.m_z);
		glEnd();
	} // for循环结构结束
} // 成员函数mf_drawWireframe结束
// 实现类CP_Sphere结束
// ////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Cylinder开始
CP_Cylinder::CP_Cylinder( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0), m_height(3.0)
{
} // 类CP_Cylinder构造函数结束
CP_Cylinder::CP_Cylinder(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r, double h)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r), m_height(h)
{
} // 类CP_Cylinder构造函数结束
CP_Point3D CP_Cylinder::mf_getPoint(double u, double v) const
{
	double x = m_radius * cos(u * PI2);
	double y = m_radius * sin(u * PI2);
	double z = v * m_height;
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_center + m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getPoint结束
CP_Point3D CP_Cylinder::mf_getPoint(const CP_Point2D& uv) const
{
	double x = m_radius * cos(uv.m_x * PI2);
	double y = m_radius * sin(uv.m_x * PI2);
	double z = uv.m_y * m_height;
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_center + m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_Cylinder::mf_getNormal(double u, double v) const
{
	return m_axisX * cos(u*PI2) + m_axisY * sin(u*PI2);
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Cylinder::mf_getNormal(const CP_Point2D& uv) const
{
	return m_axisX * cos(uv.m_x*PI2) + m_axisY * sin(uv.m_x*PI2);
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Cylinder::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = mf_getNormal(u, v);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_Cylinder::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = mf_getNormal(uv);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_Cylinder::mf_drawSolid(bool normalFlag)
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[ ]={0.0, 0.0, 1.0, 1.0, 0.0};
	CP_Point3D p;
	CP_Vector3D v;
	for (i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[ ]={u, u+stepU, u+stepU, u, u};
		glBegin(GL_POLYGON);
		for (k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if (!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
		glBegin(GL_POLYGON);
		for (k=2; k<5; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if (!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	} // 外层(i)for循环结构结束
} // 成员函数mf_drawSolid结束
void CP_Cylinder::mf_drawWireframe( )
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[ ]={0.0, 0.0, 1.0, 1.0, 0.0};
	CP_Point3D p;
	glLineWidth(2.2f);
	for (i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[ ]={u, u+stepU, u+stepU, u, u};
		glBegin(GL_LINE_LOOP);
		for (k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
		glBegin(GL_LINE_LOOP);
		for (k=2; k<5; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	} // 外层(i)for循环结构结束
} // 成员函数mf_drawWireframe结束
// 实现类CP_Cylinder结束
// ////////////////////////////////////////////////////////////////////////////


// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Circle开始
CP_Circle::CP_Circle( ) : m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0)
{
} // 类CP_Circle构造函数结束
CP_Circle::CP_Circle(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r)
{
} // 类CP_Circle构造函数结束
CP_Point3D CP_Circle::mf_getPoint(double u, double v) const
{
	double x = v * m_radius * cos(u * PI2);
	double y = v * m_radius * sin(u * PI2);
	return m_center + m_axisX*x + m_axisY*y;
} // 成员函数mf_getPoint结束
CP_Point3D CP_Circle::mf_getPoint(const CP_Point2D& uv) const
{
	double x = m_radius * cos(uv.m_x * PI2);
	double y = m_radius * sin(uv.m_y * PI2);
	return m_center + m_axisX*x + m_axisY*y;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_Circle::mf_getNormal(double u, double v) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Circle::mf_getNormal(const CP_Point2D& uv) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_Circle::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize();
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_Circle::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_Circle::mf_drawSolid(bool normalFlag)
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	CP_Vector3D v;
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_POLYGON);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
} // 成员函数mf_drawSolid结束束
void CP_Circle::mf_drawWireframe( )
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	glLineWidth(2.2f);
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_LINE_LOOP);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
} // 成员函数mf_drawWireframe结束
// 实现类CP_Circle结束
/// ////////////////////////////////////////////////////////////////////////////

// 实现类CP_ArcSurface开始
CP_ArcSurface::CP_ArcSurface( ) : m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0)
{
} // 类CP_ArcSurface构造函数结束

CP_ArcSurface::CP_ArcSurface(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r,double sa, double ea)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r),startAngle(sa),endAngle(ea)
{
} // 类CP_ArcSurface构造函数结束

CP_Point3D CP_ArcSurface::mf_getPoint(double u, double v) const
{
	double a = startAngle+(endAngle-startAngle)*u;
	double x = v * m_radius * cos(a * PI2);
	double y = v * m_radius * sin(a * PI2);
    return m_center + m_axisX * x + m_axisY * y;
} // 成员函数mf_getPoint结束
CP_Point3D CP_ArcSurface::mf_getPoint(const CP_Point2D& uv) const
{
	double a = startAngle + (endAngle-startAngle)*uv.m_x;
	double b = startAngle + (endAngle-startAngle)*uv.m_y;
	double x = m_radius * cos(a * PI2);
	double y = m_radius * sin(b * PI2);
	return m_center + m_axisX*x + m_axisY*y;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_ArcSurface::mf_getNormal(double u, double v) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_ArcSurface::mf_getNormal(const CP_Point2D& uv) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_ArcSurface::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize();
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_ArcSurface::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_ArcSurface::mf_drawSolid(bool normalFlag)
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	CP_Vector3D v;
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_POLYGON);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
} // 成员函数mf_drawSolid结束束
void CP_ArcSurface::mf_drawWireframe( )
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	glLineWidth(2.2f);
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_LINE_LOOP);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
} // 成员函数mf_drawWireframe结束
// 实现类CP_ArcSurface结束
// /////////////////////////////////////////////

// 实现类CP_ArcSquSurface开始
CP_ArcSquSurface::CP_ArcSquSurface( ) : m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0)
{
} // 类CP_ArcSquSurface构造函数结束

CP_ArcSquSurface::CP_ArcSquSurface(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r,double sa, double ea)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r),startAngle(sa),endAngle(ea)
{
} // 类CP_ArcSquSurface构造函数结束

CP_Point3D CP_ArcSquSurface::mf_getPoint(double u, double v) const
{
	double a = startAngle+(endAngle-startAngle)*u;
	double x = v * m_radius * cos(a * PI2);
	double y = v * m_radius * sin(a * PI2);
    return m_center + m_axisX * x + m_axisY * y;
} // 成员函数mf_getPoint结束
CP_Point3D CP_ArcSquSurface::mf_getPoint(const CP_Point2D& uv) const
{
	double a = startAngle + (endAngle-startAngle)*uv.m_x;
	double b = startAngle + (endAngle-startAngle)*uv.m_y;
	double x = m_radius * cos(a * PI2);
	double y = m_radius * sin(b * PI2);
	return m_center + m_axisX*x + m_axisY*y;
} // 成员函数mf_getPoint结束
CP_Vector3D CP_ArcSquSurface::mf_getNormal(double u, double v) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_ArcSquSurface::mf_getNormal(const CP_Point2D& uv) const
{
	return m_axisX ^ m_axisY;
} // 成员函数mf_getNormal结束
CP_Vector3D CP_ArcSquSurface::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize();
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_ArcSquSurface::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = m_axisX ^ m_axisY;
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
void CP_ArcSquSurface::mf_drawSolid(bool normalFlag)
{
	int i, k;
	int nu=40;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	CP_Vector3D v;
	
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_POLYGON);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
	//计算endAngle+Pi的那个点的参数
	//end + Pi(1/2 * 2Pi) = start + (end-start)*u ===> u =1.0+0.5/end-start 
	glBegin(GL_TRIANGLES);
	{				//center start 对称的endangle
		double uu[] = {0.0,0.0,1.0 + 0.5/(endAngle-startAngle)};
		double vvv[] = {0.0,1.0,1.0};
		for (k = 0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vvv[k]);
			v=mf_getUnitNormal(uu[k], vvv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		}
	}	
	glEnd();
	
	glBegin(GL_TRIANGLES);
	{				//center end 对称的startangle
		double uu[] = {0.0,1.0, 0.5/(endAngle-startAngle)};
		double vvv[] = {0.0,1.0,1.0};
		for (k = 0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vvv[k]);
			v=mf_getUnitNormal(uu[k], vvv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		}
	}	
	glEnd();
	double sa = this->startAngle;
	this->startAngle=1-sa; //u3
	double ea = this->endAngle;
	this->endAngle=1-ea;//u4
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_POLYGON);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			if(!normalFlag)
				v*=(-1);
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
	this->startAngle = sa;
	this->endAngle = ea;
	/*	
	//对称再画扇形
	用rotation 方法画不知道是否正确?
	*/	

} // 成员函数mf_drawSolid结束束
void CP_ArcSquSurface::mf_drawWireframe( )
{
	int i, k;
	int nu=10;
	double stepU=1.0/nu;
	double u;
	double vv[] = {0.0, 1.0, 1.0};
	CP_Point3D p;
	CP_Vector3D v;
	
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_LINE_LOOP);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
		
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
	//计算endAngle+Pi的那个点的参数
	//end + Pi(1/2 * 2Pi) = start + (end-start)*u ===> u =1.0+0.5/end-start 
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		glBegin(GL_LINE_LOOP);
		{				//center start 对称的endangle
			double uu[] = {0.0,0.0,1.0 + 0.5/(endAngle-startAngle)};
			double vvv[] = {0.0,1.0,1.0};
			for (k = 0; k<3; k++)
			{
				p=mf_getPoint(uu[k], vvv[k]);
				v=mf_getUnitNormal(uu[k], vvv[k]);
			
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
		}	
		glEnd();
	}	
	glBegin(GL_LINE_LOOP);
	{				//center end 对称的startangle
		double uu[] = {0.0,1.0, 0.5/(endAngle-startAngle)};
		double vvv[] = {0.0,1.0,1.0};
		for (k = 0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vvv[k]);
			v=mf_getUnitNormal(uu[k], vvv[k]);
		
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		}
	}	
	glEnd();
	double sa = this->startAngle;
	this->startAngle=1-sa; //u3
	double ea = this->endAngle;
	this->endAngle=1-ea;//u4
	for(i=0, u=0.0; i<nu; i++, u+=stepU)
	{
		double uu[]={u, u, u+stepU};
		glBegin(GL_LINE_LOOP);
		for(k=0; k<3; k++)
		{
			p=mf_getPoint(uu[k], vv[k]);
			v=mf_getUnitNormal(uu[k], vv[k]);
			
			glNormal3d(v.m_x, v.m_y, v.m_z);
			glVertex3d(p.m_x, p.m_y, p.m_z);
		} // 内层(k)for循环结构结束
		glEnd();
	}
	this->startAngle = sa;
	this->endAngle = ea;
} // 成员函数mf_drawWireframe结束
// 实现类CP_ArcSquSurface结束
// /////////////////////////////////////////////



// 实现类CP_CylinderTrimmed开始
CP_CylinderTrimmed::CP_CylinderTrimmed( ): m_center(0.0, 0.0, 0.0), m_axisX(1.0, 0.0, 0.0), m_axisY(0.0, 1.0, 0.0), m_radius(1.0), m_height(3.0)
{
} // 类CP_CylinderTrimmed构造函数结束
CP_CylinderTrimmed::CP_CylinderTrimmed(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r, double h,double u1,double u2,double u3,double u4,double v1,double v2)
	: m_center(c.m_x, c.m_y, c.m_z), m_axisX(vx.m_x, vx.m_y, vx.m_z), m_axisY(vy.m_x, vy.m_y, vy.m_z), m_radius(r), m_height(h)
	,m_u1(u1),m_u2(u2),m_u3(u3),m_u4(u4),m_v1(v1),m_v2(v2)
{
} // 类CP_CylinderTrimmed构造函数结束
CP_Point3D CP_CylinderTrimmed::mf_getPoint(double u, double v) const
{
	if ((v > m_v1 && v < m_v2) &&
		(u < m_u2 && u > m_u1 || u < m_u4 && u > m_u3 ))
		return NULL;

	double x = m_radius * cos(u * PI2);
	double y = m_radius * sin(u * PI2);
	double z = v * m_height;
	CP_Vector3D vz = m_axisX ^ m_axisY;
	return m_center + m_axisX*x + m_axisY*y +vz*z;
} // 成员函数mf_getPoint结束
CP_Point3D CP_CylinderTrimmed::mf_getPoint(const CP_Point2D& uv) const
{
	return mf_getPoint(uv.m_x, uv.m_y);
} // 成员函数mf_getPoint结束
CP_Vector3D CP_CylinderTrimmed::mf_getNormal(double u, double v) const
{
	if ((v > m_v1 && v < m_v2) &&
		(u < m_u2 && u > m_u1 || u < m_u4 && u > m_u3 ))
		return NULL;
	return m_axisX * cos(u*PI2) + m_axisY * sin(u*PI2);
} // 成员函数mf_getNormal结束
CP_Vector3D CP_CylinderTrimmed::mf_getNormal(const CP_Point2D& uv) const
{
	return mf_getNormal(uv.m_x, uv.m_y);
	return m_axisX * cos(uv.m_x*PI2) + m_axisY * sin(uv.m_x*PI2);
} // 成员函数mf_getNormal结束
CP_Vector3D CP_CylinderTrimmed::mf_getUnitNormal(double u, double v) const
{
	CP_Vector3D vz = mf_getNormal(u, v);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束
CP_Vector3D CP_CylinderTrimmed::mf_getUnitNormal(const CP_Point2D& uv) const
{
	CP_Vector3D vz = mf_getNormal(uv);
	vz.mf_normalize( );
	return vz;
} // 成员函数mf_getUnitNormal结束

void CP_CylinderTrimmed::mf_drawSolid(bool normalFlag)
{
	int m_numOfSegment = 40;
	double step = 1.0 / m_numOfSegment;

	double u = 0;
	double vv[] = {0.0, 0.0, 1.0, 1.0, 0.0};
	double vv2[] = {0.0, 0.0, m_v1, m_v1, 0.0};
	double vv3[] = {m_v2, m_v2, 1.0, 1.0, m_v2};
	CP_Point3D p;
	CP_Vector3D v;

	for(int i = 0; i < m_numOfSegment; ++i) 
	{
		double uu[] = {u, u + step, u + step, u, u};
		// Draw 4 triangles 上下两个三角形
		if (u <= m_u2 && u >= m_u1 || u <= m_u4 && u >= m_u3) 
		{
			glBegin(GL_POLYGON);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv2[k]);
				v = mf_getUnitNormal(uu[k], vv2[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_POLYGON);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv2[k]);
				v = mf_getUnitNormal(uu[k], vv2[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_POLYGON);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv3[k]);
				v = mf_getUnitNormal(uu[k], vv3[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_POLYGON);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv3[k]);
				v = mf_getUnitNormal(uu[k], vv3[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
		}// Draw two triangles 
		else {
			glBegin(GL_POLYGON);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv[k]);
				v = mf_getUnitNormal(uu[k], vv[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_POLYGON);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv[k]);
				v = mf_getUnitNormal(uu[k], vv[k]);
				if (!normalFlag)
					v*=(-1);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
		}
		u+= step;
	}
} // 成员函数mf_drawSolid结束
void CP_CylinderTrimmed::mf_drawWireframe( )
{
	int m_numOfSegment = 40;
	double step = 1.0 / m_numOfSegment;

	double u = 0;
	double vv[] = {0.0, 0.0, 1.0, 1.0, 0.0};
	double vv2[] = {0.0, 0.0, m_v1, m_v1, 0.0};
	double vv3[] = {m_v2, m_v2, 1.0, 1.0, m_v2};
	CP_Point3D p;
	CP_Vector3D v;

	for(int i = 0; i < m_numOfSegment; ++i) 
	{
		double uu[] = {u, u + step, u + step, u, u};
		// Draw 4 triangles 上下各两个三角形
		if (u <= m_u2 && u >= m_u1 || u <= m_u4 && u >= m_u3) 
		{
			glBegin(GL_LINE_LOOP);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv2[k]);
				v = mf_getUnitNormal(uu[k], vv2[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv2[k]);
				v = mf_getUnitNormal(uu[k], vv2[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv3[k]);
				v = mf_getUnitNormal(uu[k], vv3[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv3[k]);
				v = mf_getUnitNormal(uu[k], vv3[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
		}// Draw two triangles 左边 和右边的情况，跟圆柱面一样
		else {
			glBegin(GL_LINE_LOOP);
			for (int k = 0; k < 3; ++k) {
				p = mf_getPoint(uu[k], vv[k]);
				v = mf_getUnitNormal(uu[k], vv[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
			glBegin(GL_LINE_LOOP);
			for (int k = 2; k < 5; k++) {
				p = mf_getPoint(uu[k], vv[k]);
				v = mf_getUnitNormal(uu[k], vv[k]);
				glNormal3d(v.m_x, v.m_y, v.m_z);
				glVertex3d(p.m_x, p.m_y, p.m_z);
			}
			glEnd();
		}
		u+= step;
	}
	
} // 成员函数mf_drawWireframe结束
// 实现类CP_CylinderTrimmed结束
// ////////////////////////////////////////////////////////////////////////////