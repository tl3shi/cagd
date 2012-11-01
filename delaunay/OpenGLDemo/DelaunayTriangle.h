#include "CP_PointVector.h"

class DelaunayEdge
{
public:
	CP_Point2D p0, p1;
	DelaunayEdge(CP_Point2D &p0, CP_Point2D &p1);
	DelaunayEdge();
	~DelaunayEdge(){};

};
class DelaunayTriangle
{
public:
	CP_Point2D p0, p1, p2;
	DelaunayTriangle();
	DelaunayTriangle(CP_Point2D &p0, CP_Point2D &p1, CP_Point2D &p2);
	void draw();

	~DelaunayTriangle(){};
};

