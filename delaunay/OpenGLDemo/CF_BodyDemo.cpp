// CF_BodyDemo.cpp: 自定义的实体图形生成与绘制函数
#include "stdafx.h"
#include <math.h>
#include "CF_BodyDemo.h"

extern void cf_bodyCubeCreate_loop(CP_Body& b, CP_Loop& loop, int e1, bool samDir1, int e2, bool samDir2, int e3, bool samDir3, int e4, bool samDir4);

void cf_bodyCreate_loop(CP_Body& b, CP_Loop& loop, int e1, CP_Curve2D* c1, bool samDir1,
                                                   int e2, CP_Curve2D* c2, bool samDir2,
                                                   int e3, CP_Curve2D* c3, bool samDir3,
                                                   int e4, CP_Curve2D* c4, bool samDir4)
{
    CP_Coedge* coedge;
    CP_Edge*   edge;
    int ie[ ]={e1, e2, e3, e4};
    CP_Curve2D* ca[ ]={c1, c2, c3, c4};
    bool samDir[ ]={samDir1, samDir2, samDir3, samDir4};

    for (int i=0; i<4; i++)
    {
        edge=b.mf_getEdge(ie[i]);
        coedge = new CP_Coedge(ca[i], &loop, edge, samDir[i]);
        loop.mf_addCoedge(coedge);
        edge->mf_addCoedge(coedge);
    } // for循环结束
} // 函数cf_bodyCreate_loop结束

// solidOrWireframe: true for Solid
// normalFlag: true(法向量取正向); false(法向量取反向)
void cf_bodyDrawArc(const CP_Arc3D& a, bool solidOrWireframe, bool normalFlag)
{
    int numberOfSegment=int(fabs(a.m_endintAngle-a.m_startintAngle)/PI2*40);
    if (numberOfSegment<5)
        numberOfSegment=5;
    double t = 0.0;
    double step = 1.0/numberOfSegment;
    CP_Point3D p;
    CP_Vector3D v;

    if (solidOrWireframe)
    {
        v=a.m_axisX^a.m_axisY;
        v.mf_normalize( );
        if (!normalFlag)
            v*=(-1);
        glNormal3d(v.m_x, v.m_y, v.m_z);
    } // if结构结束

    for (int i = 0; i < numberOfSegment; i++)            
    {
        if (solidOrWireframe)
            glBegin(GL_POLYGON);
        else glBegin(GL_LINE_LOOP);
            glVertex3d(a.m_center.m_x, a.m_center.m_y, a.m_center.m_z);
            p = a.mf_getPoint(t);
            glVertex3d(p.m_x, p.m_y, p.m_z);
            t+=step;
            p = a.mf_getPoint(t);
            glVertex3d(p.m_x, p.m_y, p.m_z);
        glEnd();
    } // for循环结构结束
} // 函数cf_bodyDrawArc结束

// solidOrWireframe: true for Solid
// normalFlag: true(法向量取正向); false(法向量取反向)
void cf_bodyDrawCircle(const CP_Circle3D& c, bool solidOrWireframe, bool normalFlag)
{
    int numberOfSegment=40;
    double t = 0.0;
    double step = 1.0/numberOfSegment;
    CP_Point3D p;
    CP_Vector3D v;

    if (solidOrWireframe)
    {
        v=c.m_axisX^c.m_axisY;
        v.mf_normalize( );
        if (!normalFlag)
            v*=(-1);
        glNormal3d(v.m_x, v.m_y, v.m_z);
    } // if结构结束

    for (int i = 0; i < numberOfSegment; i++)            
    {
        if (solidOrWireframe)
            glBegin(GL_POLYGON);
        else glBegin(GL_LINE_LOOP);
            glVertex3d(c.m_center.m_x, c.m_center.m_y, c.m_center.m_z);
            p = c.mf_getPoint(t);
            glVertex3d(p.m_x, p.m_y, p.m_z);
            t+=step;
            p = c.mf_getPoint(t);
            glVertex3d(p.m_x, p.m_y, p.m_z);
        glEnd();
    } // for循环结构结束
} // 函数cf_bodyDrawCircle结束

// solidOrWireframe: true for Solid
void cf_bodyDrawSurface(const CP_Body& b, bool solidOrWireframe)
{
    CP_Face*      tf;
    CP_Surface3D* s;
    int n=b.mf_getFaceNumber( );
    for (int i=0; i<n; i++)
    {
        tf=b.mf_getFace(i);
        s=tf->m_surface;
        if (solidOrWireframe)
            s->mf_drawSolid(tf->m_sameNormalDirection);
        else s->mf_drawWireframe( );
    } // for循环结束
} // 函数cf_bodyDrawSurface结束

void cf_bodyDrawEdge(const CP_Body& b)
{
    CP_Edge*    te;
    CP_Curve3D* c;
    int n=b.mf_getEdgeNumber( );
    for (int i=0; i<n; i++)
    {
        te=b.mf_getEdge(i);
        c=te->m_curve3D;
        c->mf_draw( );
    } // for循环结束
} // 函数cf_bodyDrawEdge结束

void cf_bodyCubeCreate(CP_Body& b, double cx, double cy, double cz, double size)
{
    int i;
    CP_Face* tf;
    CP_Edge* te;
    CP_Vertex* tv, *tv0;
    CP_Loop* loop;
    CP_Plane* plane;
    CP_LineSegment3D* lineSegment;
    CP_Point3D* p;
    CP_Point3D cp;
    CP_Vector3D vx, vy;
    double halfSize = size/2;

    // 填写8个顶点
    int ivx[ ]={-1, +1, +1, -1, -1, +1, +1, -1};
    int ivy[ ]={-1, -1, +1, +1, -1, -1, +1, +1};
    int ivz[ ]={-1, -1, -1, -1, +1, +1, +1, +1};
    for (i=0; i<8; i++)
    {
        p=new CP_Point3D(cx+halfSize*ivx[i], cy+halfSize*ivy[i], cz+halfSize*ivz[i]);
        tv=new CP_Vertex(p);
        b.mf_addVertex(tv);
    } // for循环结束

    // 填写12条边
    //          0  1  2  3  4  5  6  7  8  9  10 11
    int ies[ ]={0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7};
    int iee[ ]={1, 2, 3, 0, 4, 5, 6, 7, 5, 6, 7, 4};
    for (i=0; i<12; i++)
    {
        tv0=(CP_Vertex*)(b.mf_getVertex(ies[i]));
        tv =(CP_Vertex*)(b.mf_getVertex(iee[i]));
        lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv->m_point));
        te=new CP_Edge(lineSegment, tv0, tv);
        b.mf_addEdge(te);
        tv0->mf_addAdjacentEdge(te);
        tv->mf_addAdjacentEdge(te);
    } // for循环结束

    // 填写6个面
    //               0     1      2      3      4      5
    int ifc[ ]     ={4,    0,     0,     1,     0,     2};
    int ifx[ ]     ={5,    3,     4,     2,     1,     3};
    int ify[ ]     ={7,    1,     3,     5,     4,     6};
    int ie1[ ]     ={8,    3,     4,     1,     0,     2};
    int ie2[ ]     ={9,    2,     11,    6,     5,     7};
    int ie3[ ]     ={10,   1,     7,     9,     8,     10};
    int ie4[ ]     ={11,   0,     3,     5,     4,     6};
    bool samDir1[ ]={true, false, true,  true,  true,  true};
    bool samDir2[ ]={true, false, false, true,  true,  true};
    bool samDir3[ ]={true, false, false, false, false, false};
    bool samDir4[ ]={true, false, true,  false, false, false};
    for (i=0; i<6; i++)
    {
        tv=(CP_Vertex*)(b.mf_getVertex(ifc[i]));
        cp=*(tv->m_point);
        tv=(CP_Vertex*)(b.mf_getVertex(ifx[i]));
        vx=*(tv->m_point)-cp;
        tv=(CP_Vertex*)(b.mf_getVertex(ify[i]));
        vy=*(tv->m_point)-cp;
        plane=new CP_Plane(cp, vx, vy);
        tf=new CP_Face(&b, plane, true);
        b.mf_addFace(tf);
        loop=new CP_Loop(tf);
        tf->mf_addLoop(loop);
        cf_bodyCubeCreate_loop(b, *loop, ie1[i], samDir1[i], ie2[i], samDir2[i], ie3[i], samDir3[i], ie4[i], samDir4[i]);
    } // for循环结束
} // 函数cf_bodyCubeCreate结束

void cf_bodyCubeDrawSolid(const CP_Body& b)
{
    glColor3f(1.0, 0.0, 0.0);
    cf_bodyDrawSurface(b, true);
} // 函数cf_bodyCubeDrawSolid结束

void cf_bodyCubeDrawWireframe(const CP_Body& b)
{
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2.2f);
    cf_bodyDrawSurface(b, false);
} // 函数cf_bodyCubeDrawWireframe结束

void cf_bodyCubeDrawEdge(const CP_Body& b)
{
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2.2f);
    cf_bodyDrawEdge(b);
} // 函数cf_bodyCubeDrawEdge结束


void cf_bodycylinderCreate_loop(CP_Body& b, CP_Loop& loop)
{
	CP_Coedge* coedge;
	CP_Edge*   edge;
	CP_Circle2D* circle2D;
	CP_LineSegment2D* lineSegment2D;

	lineSegment2D = new CP_LineSegment2D(0.0, 0.0, 0.0, 1.0);
	edge=b.mf_getEdge(2);
	coedge = new CP_Coedge(lineSegment2D, &loop, edge, true);
	loop.mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);

	circle2D = new CP_Circle2D();
	edge = b.mf_getEdge(0);
	coedge = new CP_Coedge(circle2D, &loop, edge, true);
	loop.mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);

	lineSegment2D = new CP_LineSegment2D(0.0, 1.0, 0.0, 0.0);
	edge=b.mf_getEdge(2);
	coedge = new CP_Coedge(lineSegment2D, &loop, edge, false);
	loop.mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);

	circle2D = new CP_Circle2D();
	edge = b.mf_getEdge(1);
	coedge = new CP_Coedge(circle2D, &loop, edge, false);
	loop.mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);
}

void cf_bodyCylinderCreate(CP_Body& b, double cx, double cy, double cz, double h, double r)
{
    CP_Face* tf;
    CP_Edge* te;
    CP_Vertex* tv1, *tv0;
    CP_Plane* plane;
    CP_LineSegment3D* lineSegment;
    CP_Point3D cp1, cp0, origin, cp00;
    CP_Vector3D vx, vy;
	CP_Circle * circle;
	//两个顶点
	cp0 =  CP_Point3D(cx + r, cy, cz);
	cp1 =  CP_Point3D(cx + r, cy, cz + h);
	tv0 = new CP_Vertex(&cp0);
	b.mf_addVertex(tv0);
	tv1 = new CP_Vertex(&cp1);
	b.mf_addVertex(tv1);
	
	//下底圆心
	origin =  CP_Point3D(cx, cy, cz);
	//上底圆心
	cp00 =  CP_Point3D(cx, cy, cz+h);

	//3条边
	vx = cp0 - origin;
	vy = CP_Point3D(cx, cy+r, cz) - origin;

	CP_Circle3D * circle0 = new CP_Circle3D(origin, vx, vy, r);
	tv0 = (CP_Vertex *)b.mf_getVertex(0);
	tv1= (CP_Vertex *)b.mf_getVertex(0);
	te = new CP_Edge(circle0, tv0, tv1);
	b.mf_addEdge(te);
	tv0->mf_addAdjacentEdge(te);
	tv1->mf_addAdjacentEdge(te);
	
	
	CP_Circle3D * circle1 = new  CP_Circle3D(cp00, vx, vy, r);
	tv0 = (CP_Vertex *)b.mf_getVertex(1);
	tv1 = (CP_Vertex *)b.mf_getVertex(1);
	te = new CP_Edge(circle1, tv0, tv1);
	b.mf_addEdge(te);
	tv0->mf_addAdjacentEdge(te);
	tv1->mf_addAdjacentEdge(te);

    lineSegment=new CP_LineSegment3D(cp0, cp1);
	tv0 = (CP_Vertex *)b.mf_getVertex(0);
	tv1 = (CP_Vertex *)b.mf_getVertex(1);
    te=new CP_Edge(lineSegment, tv0, tv1);
    b.mf_addEdge(te);
    tv0->mf_addAdjacentEdge(te);
    tv1->mf_addAdjacentEdge(te);
	
	//3个面
	//下底面
    //CP_Circle3D(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r);
	CP_Circle3D lowCircle = CP_Circle3D(origin, vx, vy, r);
	circle  = new CP_Circle(origin, vx, vy, r);
	tf = new CP_Face(&b, circle, true);
	b.mf_addFace(tf);

	CP_Loop * loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	CP_Circle2D* circle2D = new CP_Circle2D();
	CP_Edge* edge = b.mf_getEdge(0);
	CP_Coedge* coedge = new CP_Coedge(circle2D, loop, edge, false);
	loop->mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);

	//上底面
	CP_Circle3D upCircle = CP_Circle3D(cp00, vx, vy, r);
	circle = new CP_Circle(cp00, vx, vy, r);
	tf = new CP_Face(&b, circle, true);
	b.mf_addFace(tf);
	
	loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	circle2D = new CP_Circle2D();
	edge = b.mf_getEdge(1);
	coedge = new CP_Coedge(circle2D, loop, edge, true);
	loop->mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);
	
	//圆柱面
	CP_Cylinder* cylinder = new CP_Cylinder(origin, vx, vy, r, h);
	tf = new CP_Face(&b, cylinder, true);
	b.mf_addFace(tf);
	cf_bodycylinderCreate_loop(b, *loop);
} // 函数cf_bodyCylinderCreate结束

void cf_bodyCylinderDrawSolid(const CP_Body& b)
{
	glColor3f(1.0, 0.0, 0.0);
    cf_bodyDrawSurface(b, true);
} // 函数cf_bodyCylinderDrawSolid结束

void cf_bodyCylinderDrawWireframe(const CP_Body& b)
{
	glColor3f(0.0, 0.0, 1.0);
    glLineWidth(1.2f);
    cf_bodyDrawSurface(b, false);
} // 函数cf_bodyCylinderDrawWireframe结束

void cf_bodyCylinderDrawEdge(const CP_Body& b)
{
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(2.2f);
    cf_bodyDrawEdge(b);
} // 函数cf_bodyCylinderDrawEdge结束


void cf_bodyCubeCreate_loop(CP_Body& b, CP_Loop& loop, int e1, bool samDir1, int e2, bool samDir2, int e3, bool samDir3, int e4, bool samDir4)
{
    CP_Coedge* coedge;
    CP_Edge*   edge;
    CP_LineSegment2D* lineSegment2D;
    double dsx[ ]={0.0, 1.0, 1.0, 0.0};
    double dsy[ ]={0.0, 0.0, 1.0, 1.0};
    double dex[ ]={1.0, 1.0, 0.0, 0.0};
    double dey[ ]={0.0, 1.0, 1.0, 0.0};
    int ie[ ]={e1, e2, e3, e4};
    bool samDir[ ]={samDir1, samDir2, samDir3, samDir4};

    for (int i=0; i<4; i++)
    {
        lineSegment2D = new CP_LineSegment2D(dsx[i], dsy[i], dex[i], dey[i]);
        edge=b.mf_getEdge(ie[i]);
        coedge = new CP_Coedge(lineSegment2D, &loop, edge, samDir[i]);
        loop.mf_addCoedge(coedge);
        edge->mf_addCoedge(coedge);
    } // for循环结束
} // 函数cf_bodyCubeCreate_loop结束


