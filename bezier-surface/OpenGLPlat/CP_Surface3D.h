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

	// normalFlag: true(法向量取正向); false(法向量取反向)
	virtual void mf_drawSolid(bool normalFlag=true)=0;
	virtual void mf_drawWireframe( )=0;
}; // 类CP_Surface3D定义结束


// Class for bezier surface
class CP_Bezier3D : public CP_Surface3D
{
public:
	CP_Bezier3D();
	~CP_Bezier3D();

public:
	CP_Point3D mf_getPoint(double u, double v) const;
	CP_Point3D mf_getPoint(const CP_Point2D& uv) const;
	CP_Vector3D mf_getNormal(double u, double v) const;
	CP_Vector3D mf_getNormal(const CP_Point2D& uv) const;
	CP_Vector3D mf_getUnitNormal(double u, double v) const;
	CP_Vector3D mf_getUnitNormal(const CP_Point2D& uv) const;

	// normalFlag: true(法向量取正向); false(法向量取反向)
	void mf_drawSolid(bool normalFlag=true);
	void mf_drawWireframe();
	void mf_drawControlPoints();

	// public field
public:
	int m_segmentNumU;
	int m_segmentNumV;

	// private field
private:
	CP_Point3D **m_pCtrlPoints;
	int m_numOfCtrlPointsU;
	int m_numOfCtrlPointsV;	

	// private method
private:
	CP_Vector3D mf_getTriangleNormal(CP_Point3D p1, CP_Point3D p2, CP_Point3D p3);
};


#endif