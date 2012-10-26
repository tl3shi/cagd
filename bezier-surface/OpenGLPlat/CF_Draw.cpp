#include "stdafx.h"
#include "CF_Draw.h"
#include <math.h>
#include "CP_PointVector.h"

// To draw regular tetrahedron 
// Four points : A(0, 1, 0), B(0, -1/3, 2¡Ì2/3), C(-¡Ì6/3, -1/3, -¡Ì2/3), D(¡Ì6/3, -1/3, -¡Ì2/3)
void cf_drawRTetrahedron() {
	double sqrt2= sqrt(2.0);
	double sqrt6 = sqrt(6.0);
	double l = 2;

	CP_Vector3D vA(0, l, 0);
	CP_Vector3D vB(0, -l/3, 2*l*sqrt2/3);
	CP_Vector3D vC(-l*sqrt6/3, -l/3, -l*sqrt2/3);
	CP_Vector3D vD(l*sqrt6/3, -l/3, -l*sqrt2/3);

	// Face ABC
	glColor3f(1.0, 0.0, 0.0); // Red
	glBegin(GL_POLYGON);
	glVertex3d(vA.m_x, vA.m_y, vA.m_z);
	glVertex3d(vB.m_x, vB.m_y, vB.m_z);
	glVertex3d(vC.m_x, vC.m_y, vC.m_z);
	glEnd();

	// Face ACD
	glColor3f(0.0, 0.0, 1.0); // Blue
	glBegin(GL_POLYGON);
	glVertex3d(vA.m_x, vA.m_y, vA.m_z);
	glVertex3d(vC.m_x, vC.m_y, vC.m_z);
	glVertex3d(vD.m_x, vD.m_y, vD.m_z);
	glEnd();

	// Face ADB
	glColor3f(0.0, 1.0, 0.0); // Green
	glBegin(GL_POLYGON);
	glVertex3d(vA.m_x, vA.m_y, vA.m_z);
	glVertex3d(vD.m_x, vD.m_y, vD.m_z);
	glVertex3d(vB.m_x, vB.m_y, vB.m_z);
	glEnd();

	// Face CBD
	glColor3f(1.0, 0.0, 1.0); // 
	glBegin(GL_POLYGON);
	glVertex3d(vC.m_x, vC.m_y, vC.m_z);
	glVertex3d(vB.m_x, vB.m_y, vB.m_z);
	glVertex3d(vD.m_x, vD.m_y, vD.m_z);
	glEnd();
}