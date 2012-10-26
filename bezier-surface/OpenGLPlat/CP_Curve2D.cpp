#include "stdafx.h"
#include "CP_Curve2D.h"
#include "GL/GLU.H" 

#include <math.h>

CP_Bezier2D::CP_Bezier2D() {
	// Initialize field
	// m_sizeOfCtrlPoints = CTRLP_INIT_SIZE;
	m_numOfCtrlPoints = 0;
	m_pCtrlPoints = new CP_Point2D[CTRLP_INIT_SIZE];
}

CP_Bezier2D::~CP_Bezier2D() {
	delete []m_pCtrlPoints;
}

CP_Point2D CP_Bezier2D::mf_getEndingPoint() {
	return m_pCtrlPoints[0];
}

CP_Point2D CP_Bezier2D::mf_getStartingPoint() {
	return m_pCtrlPoints[m_numOfCtrlPoints];
}

// Function: To get point in t position using de Casteljau algorithm
CP_Point2D CP_Bezier2D::mf_getPoint(double t) {
	CP_Point2D *pTriangleCol = new CP_Point2D[m_numOfCtrlPoints];
	CP_Point2D savePoint; // Save pTriangleCol[j] point value
	CP_Point2D lastPoint; // Represent pTriangleCol[j-1] point

	// Initialize 0 column
	for (int i = 0; i < m_numOfCtrlPoints; ++i)
		pTriangleCol[i] = m_pCtrlPoints[i];

	// Calculate other columns using de Casteljau triangle
	for (int i = 1; i < m_numOfCtrlPoints; ++i) {
		lastPoint = pTriangleCol[i-1];
		for (int j = i; j < m_numOfCtrlPoints; ++j) {
			savePoint = pTriangleCol[j];
			pTriangleCol[j].m_x = (1-t) * lastPoint.m_x + t * pTriangleCol[j].m_x;
			pTriangleCol[j].m_y = (1-t) * lastPoint.m_y + t * pTriangleCol[j].m_y;
			lastPoint = savePoint;
		}
	}
	CP_Point2D result = pTriangleCol[m_numOfCtrlPoints - 1];

	delete []pTriangleCol;
	return result;
}

// Function: To draw bezier curve
void CP_Bezier2D::mf_draw() {
	int numberOfSegment = 40;
	double t = 0.0;
	double step = 1.0 / numberOfSegment;
	CP_Point2D p;

	// Draw bezier curve
	glColor3f(1.0, 0.0, 0.0); // Red
	glLineWidth(2.2f);
	glBegin(GL_LINE_STRIP);
	p = mf_getPoint(1/3.0);
	for (int i = 0; i < numberOfSegment; i++, t+=step)
	{
		p = mf_getPoint(t);
		glVertex3d(p.m_x, p.m_y, 0.0);
	}
	glEnd( );

	// Draw control points
	glColor3f(0.0, 0.0, 1.0); // Blue
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < m_numOfCtrlPoints; ++i)
		glVertex3d(m_pCtrlPoints[i].m_x, m_pCtrlPoints[i].m_y, 0);
	glEnd( );
}

// Function: To push point into array.
void CP_Bezier2D::mf_pushCtrlPoints(CP_Point2D point) {
	if (m_numOfCtrlPoints >= CTRLP_INIT_SIZE)
		return;
	m_pCtrlPoints[m_numOfCtrlPoints++] = point;
}
