#include "stdafx.h"
#include "CP_Surface3D.h"
#include "GL/GLU.H" 

CP_Bezier3D::CP_Bezier3D(): m_numOfCtrlPointsU(4), m_numOfCtrlPointsV(4),
	m_segmentNumU(40), m_segmentNumV(40){

	m_pCtrlPoints = new CP_Point3D *[4];
	for (int i = 0; i < 4; ++i)
		m_pCtrlPoints[i] = new CP_Point3D[4];

	for (int i = 0; i < m_numOfCtrlPointsU; ++i) {
		for (int j = 0; j < m_numOfCtrlPointsV; ++j) {
			m_pCtrlPoints[i][j] = CP_Point3D((i - 2)*1.2, (j - 2)*1.2, 0);
		}
	}
	m_pCtrlPoints[1][1].m_z = 3;
	m_pCtrlPoints[1][2].m_z = 3;
	m_pCtrlPoints[2][1].m_z = 3;
	m_pCtrlPoints[2][2].m_z = 3;

	m_pCtrlPoints[0][1].m_z = 1;
	m_pCtrlPoints[0][2].m_z = 1;
	m_pCtrlPoints[1][0].m_z = 1;
	m_pCtrlPoints[2][0].m_z = 1;
	m_pCtrlPoints[3][1].m_z = 1;
	m_pCtrlPoints[3][2].m_z = 1;
	m_pCtrlPoints[1][3].m_z = 1;
	m_pCtrlPoints[2][3].m_z = 1;
}

CP_Bezier3D::~CP_Bezier3D() {
	for (int i = 0; i < m_numOfCtrlPointsU; ++i)
		delete []m_pCtrlPoints[i];
	delete []m_pCtrlPoints;
}

// Function: Calculate point position using de Casteljau algorithms
CP_Point3D CP_Bezier3D::mf_getPoint(double u, double v) const{
	CP_Point3D *pTriangleColV = new CP_Point3D[m_numOfCtrlPointsV]; // One column of triangle 
	CP_Point3D *pCtrlPtU = new CP_Point3D[m_numOfCtrlPointsU]; // Control points of u direciton
	CP_Point3D savePoint; // Save pTriangleCol[j] point value
	CP_Point3D lastPoint; // Represent pTriangleCol[j-1] point

	// Calculate value p(i,v)
	for (int i = 0; i < m_numOfCtrlPointsU; ++i) {
		// Initialize 0 column
		for (int j = 0; j < m_numOfCtrlPointsV; ++j)
			pTriangleColV[j] = m_pCtrlPoints[i][j];

		// Calculate p(i,v) using de Casteljau triangle
		for (int j = 1; j < m_numOfCtrlPointsV; ++j) {
			lastPoint = pTriangleColV[j-1];
			for (int k = j; k < m_numOfCtrlPointsV; ++k) {
				savePoint = pTriangleColV[k];
				pTriangleColV[k].m_x = (1-v) * lastPoint.m_x + v * pTriangleColV[k].m_x;
				pTriangleColV[k].m_y = (1-v) * lastPoint.m_y + v * pTriangleColV[k].m_y;
				pTriangleColV[k].m_z = (1-v) * lastPoint.m_z + v * pTriangleColV[k].m_z;
				lastPoint = savePoint;
			}
		}
		pCtrlPtU[i] = pTriangleColV[m_numOfCtrlPointsV - 1]; // p(i,v) value
	}

	// Calculate final position using de Casteljau triangle
	for (int i = 1; i < m_numOfCtrlPointsU; ++i) {
		lastPoint = pCtrlPtU[i-1];
		for (int j = i; j < m_numOfCtrlPointsU; ++j) {
			savePoint = pCtrlPtU[j];
			pCtrlPtU[j].m_x = (1-u) * lastPoint.m_x + u * pCtrlPtU[j].m_x;
			pCtrlPtU[j].m_y = (1-u) * lastPoint.m_y + u * pCtrlPtU[j].m_y;
			pCtrlPtU[j].m_z = (1-u) * lastPoint.m_z + u * pCtrlPtU[j].m_z;
			lastPoint = savePoint;
		}
	}
	
	CP_Point3D resultPoint = pCtrlPtU[m_numOfCtrlPointsU - 1];

	delete []pTriangleColV;
	delete []pCtrlPtU;

	return resultPoint;
}

CP_Point3D CP_Bezier3D::mf_getPoint(const CP_Point2D& uv) const {
	return mf_getPoint(uv.m_x, uv.m_y);
}

// Function: Get normal of triangle as normal of point. Triangle: (u, v) (u + du, v) (u + du, v + dv) 
CP_Vector3D CP_Bezier3D::mf_getNormal(double u, double v) const {
	double du = 1.0 / m_segmentNumU;
	double dv = 1.0 / m_segmentNumV;
	CP_Point3D p1 = mf_getPoint(u, v);
	CP_Point3D p2 = mf_getPoint(u + du, v);
	CP_Point3D p3 = mf_getPoint(u + du, v + dv);

	CP_Vector3D v1(p2.m_x - p1.m_x, p2.m_y - p1.m_y, p2.m_z - p1.m_z);
	CP_Vector3D v2(p3.m_x - p1.m_x, p3.m_y - p1.m_y, p3.m_z - p1.m_z);
	v1 ^= v2;
	return v1;
}

CP_Vector3D CP_Bezier3D::mf_getNormal(const CP_Point2D& uv) const {
	return mf_getNormal(uv.m_x, uv. m_y);
}

CP_Vector3D CP_Bezier3D::mf_getUnitNormal(double u, double v) const {
	CP_Vector3D vector = mf_getNormal(u, v);
	vector.mf_normalize();
	return vector;
}

CP_Vector3D CP_Bezier3D::mf_getUnitNormal(const CP_Point2D& uv) const {
	return mf_getUnitNormal(uv.m_x, uv.m_y);
}

void CP_Bezier3D::mf_drawSolid(bool normalFlag/* =true */) {
	double du = 1.0 / m_segmentNumU;
	double dv = 1.0 / m_segmentNumV;
	double u = 0;
	double v = 0;

	glColor3d(1, 0, 0);
	for (int i = 0; i < m_segmentNumU; ++i) {
		for (int j = 0; j < m_segmentNumV; ++j) {
			CP_Point3D pUV =  mf_getPoint(u, v);
			CP_Point3D pUduVdv = mf_getPoint(u + du, v + dv);
			CP_Point3D pUVdv = mf_getPoint(u, v + dv);

			// Triangle (u,v) (u+du,v) (u+du,v+dv)
			glBegin(GL_POLYGON);
			CP_Point3D point = pUV;
			CP_Vector3D vector = mf_getNormal(u, v);
			glNormal3d(vector.m_x, vector.m_y, vector.m_z);
			glVertex3d(point.m_x, point.m_y, point.m_z);

			point = mf_getPoint(u + du, v);
			glVertex3d(point.m_x, point.m_y, point.m_z);

			point = pUduVdv;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			glEnd();

			// Triangle (u,v) (u,v+dv) (u+du,v+dv)
			glBegin(GL_POLYGON);
			point = pUV;
			vector = mf_getTriangleNormal(pUV, pUduVdv, pUVdv);
			glNormal3d(vector.m_x, vector.m_y, vector.m_z);
			glVertex3d(point.m_x, point.m_y, point.m_z);

			point = pUVdv;
			glVertex3d(point.m_x, point.m_y, point.m_z);

			point = pUduVdv;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			glEnd();

			u += du;
		}
		u = 0;
		v += dv;
	}	
}

void CP_Bezier3D::mf_drawWireframe() {
	double du = 1.0 / m_segmentNumU;
	double dv = 1.0 / m_segmentNumV;
	double u = 0;
	double v = 0;

	glColor3d(1, 0, 0);
	for (int i = 0; i < m_segmentNumU; ++i) {
		for (int j = 0; j < m_segmentNumV; ++j) {

			CP_Point3D pUV =  mf_getPoint(u, v);
			CP_Point3D pUduVdv = mf_getPoint(u + du, v + dv);

			glBegin(GL_LINE_LOOP);
			CP_Point3D point = pUV;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			point = mf_getPoint(u + du, v);
			glVertex3d(point.m_x, point.m_y, point.m_z);
			point = pUduVdv;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			glEnd();

			glBegin(GL_LINE_STRIP);
			point = pUV;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			point = mf_getPoint(u, v + dv);
			glVertex3d(point.m_x, point.m_y, point.m_z);
			point = pUduVdv;
			glVertex3d(point.m_x, point.m_y, point.m_z);
			glEnd();
			u += du;
		}
		u = 0;
		v += dv;
	}
}

void CP_Bezier3D::mf_drawControlPoints() {
	glColor3d(0, 0, 1);
	for (int i = 0; i < m_numOfCtrlPointsU; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_numOfCtrlPointsV; ++j) {
			glVertex3d(m_pCtrlPoints[i][j].m_x, m_pCtrlPoints[i][j].m_y, m_pCtrlPoints[i][j].m_z);
		}
		glEnd();
	}

	for (int i = 0; i < m_numOfCtrlPointsV; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < m_numOfCtrlPointsU; ++j) {
			glVertex3d(m_pCtrlPoints[j][i].m_x, m_pCtrlPoints[j][i].m_y, m_pCtrlPoints[j][i].m_z);
		}
		glEnd();
	}
}

// Function: Get triangle normal v = v(p2p1) ^ v(p3p1)
CP_Vector3D CP_Bezier3D::mf_getTriangleNormal(CP_Point3D p1, CP_Point3D p2, CP_Point3D p3) {
	CP_Vector3D v1(p2.m_x - p1.m_x, p2.m_y - p1.m_y, p2.m_z - p1.m_z);
	CP_Vector3D v2(p3.m_x - p1.m_x, p3.m_y - p1.m_y, p3.m_z - p1.m_z);
	v1 ^= v2;
	return v1;
}