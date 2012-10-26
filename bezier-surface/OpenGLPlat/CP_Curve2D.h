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

#define CTRLP_INIT_SIZE 100
class CP_Bezier2D : public CP_Curve2D 
{
private:
	CP_Point2D *m_pCtrlPoints;
	int m_numOfCtrlPoints;
	// In this case, I do not use dynamic growth array. I will write it if needed.
	// int m_sizeOfCtrlPoints;

public:
	CP_Bezier2D();
	~CP_Bezier2D();

	CP_Point2D mf_getStartingPoint( );
	CP_Point2D mf_getEndingPoint( );
	CP_Point2D mf_getPoint(double t);
	void mf_draw();

	void mf_pushCtrlPoints(CP_Point2D point);
};

#endif