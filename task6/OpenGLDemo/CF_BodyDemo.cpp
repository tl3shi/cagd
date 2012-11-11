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


void cf_bodyCylinderCreate(CP_Body& b, double cx, double cy, double cz, double h, double r)
{
    CP_Face* tf;
    CP_Edge* te;
    CP_Vertex* tv1, *tv0;
    CP_LineSegment3D* lineSegment;
    CP_Point3D *cp1, *cp0;
	CP_Point3D origin, cp00;
    CP_Vector3D vx3d, vy3d;
    CP_Vector2D vx2d, vy2d;
	CP_Circle * circle;


	//两个顶点
	cp0 =  new CP_Point3D(cx + r, cy, cz);
	cp1 =  new CP_Point3D(cx + r, cy, cz + h);
	tv0 = new CP_Vertex(cp0);
	b.mf_addVertex(tv0);
	tv1 = new CP_Vertex(cp1);
	b.mf_addVertex(tv1);
	
	//下底圆心
	origin =  CP_Point3D(cx, cy, cz);
	//上底圆心
	cp00 =  CP_Point3D(cx, cy, cz+h);

	//3条边
	vx3d = CP_Vector3D(1,0,0);
	vy3d = CP_Vector3D(0,1,0);
	
	vx2d = CP_Vector2D(1,0);
	vy2d = CP_Vector2D(0,1);
	//下底边
	CP_Circle3D * circle0 = new CP_Circle3D(origin, vx3d, vy3d, r);
	tv0 = (CP_Vertex *)b.mf_getVertex(0);
	te = new CP_Edge(circle0, tv0, tv0);
	b.mf_addEdge(te);
	tv0->mf_addAdjacentEdge(te);
	//上底边
	CP_Circle3D * circle1 = new  CP_Circle3D(cp00, vx3d, vy3d, r);
	tv0 = (CP_Vertex *)b.mf_getVertex(1);
	te = new CP_Edge(circle1, tv0, tv0);
	b.mf_addEdge(te);
	tv0->mf_addAdjacentEdge(te);
	//棱
	tv0 = (CP_Vertex *)b.mf_getVertex(0);
	tv1 = (CP_Vertex *)b.mf_getVertex(1);
    lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv1->m_point));
    te=new CP_Edge(lineSegment, tv0, tv1);
    b.mf_addEdge(te);
    tv0->mf_addAdjacentEdge(te);
    tv1->mf_addAdjacentEdge(te);
	
	
	CP_Point2D origin2 = CP_Point2D(cx, cy);
	//3个面
	//下底面
    //CP_Circle3D(const CP_Point3D& c, const CP_Vector3D& vx, const CP_Vector3D& vy, double r);
	circle  = new CP_Circle(origin, vx3d, vy3d, r);
	tf = new CP_Face(&b, circle, true);
	b.mf_addFace(tf);
	CP_Loop * loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	CP_Circle2D* circle2D = new CP_Circle2D(origin2, vx2d, vy2d, r);
	CP_Edge* edge = b.mf_getEdge(0);
	CP_Coedge* coedge = new CP_Coedge(circle2D, loop, edge, false);
	loop->mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);

	//上底面
	circle = new CP_Circle(cp00, vx3d, vy3d, r);
	tf = new CP_Face(&b, circle, true);
	b.mf_addFace(tf);
	loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	circle2D = new CP_Circle2D(origin2, vx2d, vy2d, r);
	edge = b.mf_getEdge(1);
	coedge = new CP_Coedge(circle2D, loop, edge, true);
	loop->mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);
	
	//圆柱面
	CP_Cylinder* cylinder = new CP_Cylinder(origin, vx3d, vy3d, r, h);
	tf = new CP_Face(&b, cylinder, true);
	b.mf_addFace(tf);
	loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	//展开是长方形 由上下底边0，1 和侧边2
	cf_bodyCubeCreate_loop(b, *loop, 0, true, 2, true, 1, false, 2, false);

} // 函数cf_bodyCylinderCreate结束

void cf_bodyCylinderDrawSolid(const CP_Body& b)
{
	glColor3f(1.0, 0.0, 0.0);
    cf_bodyDrawSurface(b, true);
} // 函数cf_bodyCylinderDrawSolid结束

void cf_bodyCylinderDrawWireframe(const CP_Body& b)
{
	glColor3f(0.0, 1.0, 0.0);
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


void cf_bodyCylinderTrimmedCreate(CP_Body& b, double cx, double cy, double cz, double h, double r, double u1, double u2, double v1, double v2)
{
	
	//根据PPT中猜测u3,u4的值
	double u3 = 1 - u2 ; 
	double u4 = 1 - u1;
	
	CP_Vertex * t_vetex, *tv0, *tv1;
    CP_Face* tf;
    CP_Edge* te;
    CP_LineSegment3D* lineSegment;
	CP_Circle * circle;

	//下底面圆心
	CP_Point3D originLow = CP_Point3D(cx, cy, cz);
	//上底面圆心
	CP_Point3D originUp = CP_Point3D(cx, cy, cz+h);

	CP_Point2D origin2d= CP_Point2D(cx, cy);

	//方向单位向量
	CP_Vector3D vx3d = CP_Vector3D(1,0,0);
	CP_Vector3D vy3d = CP_Vector3D(0,1,0);
	
	CP_Vector2D vx2d = CP_Vector2D(1,0);
	CP_Vector2D vy2d = CP_Vector2D(0,1);
	//没写循环，方便调试
#pragma region 填充顶点
	//下顶点
	CP_Point3D p0 = CP_Point3D(cx+r,cy,cz);
	//上顶点
	CP_Point3D p1 = CP_Point3D(cx+r,cy,cz+h);
	
	CP_Cylinder t = CP_Cylinder(originLow, vx3d, vy3d, r, h);
	
	CP_Point3D p2 = t.mf_getPoint(u1, v1);
	CP_Point3D p3 = t.mf_getPoint(u2, v1);
	CP_Point3D p4 = t.mf_getPoint(u3, v1);
	CP_Point3D p5 = t.mf_getPoint(u4, v1);
	
	CP_Point3D p6 = t.mf_getPoint(u1, v2);
	CP_Point3D p7 = t.mf_getPoint(u2, v2);
	CP_Point3D p8 = t.mf_getPoint(u3, v2);
	CP_Point3D p9 = t.mf_getPoint(u4, v2);
	
	t_vetex = new CP_Vertex(&p0);
	b.mf_addVertex(t_vetex);//v0
	
	t_vetex = new CP_Vertex(&p1);
	b.mf_addVertex(t_vetex);//v1
	
	t_vetex = new CP_Vertex(&p2);
	b.mf_addVertex(t_vetex);//v2
	
	t_vetex = new CP_Vertex(&p3);
	b.mf_addVertex(t_vetex);//v3
	
	t_vetex = new CP_Vertex(&p4);
	b.mf_addVertex(t_vetex);//v4
	
	t_vetex = new CP_Vertex(&p5);
	b.mf_addVertex(t_vetex);//v5
	
	t_vetex = new CP_Vertex(&p6);
	b.mf_addVertex(t_vetex);//v6
	
	t_vetex = new CP_Vertex(&p7);
	b.mf_addVertex(t_vetex);//v7
	
	t_vetex = new CP_Vertex(&p8);
	b.mf_addVertex(t_vetex);//v8

	t_vetex = new CP_Vertex(&p9);
	b.mf_addVertex(t_vetex);//v9
#pragma endregion 顶点填充完毕

#pragma  region 填充边
	//下底边E0
	CP_Circle3D * circle0 = new CP_Circle3D(originLow, vx3d, vy3d, r);
	t_vetex = (CP_Vertex *)b.mf_getVertex(0);
	te = new CP_Edge(circle0, t_vetex , t_vetex);
	b.mf_addEdge(te);
	t_vetex->mf_addAdjacentEdge(te);
	//棱E1
	tv0 = (CP_Vertex *)b.mf_getVertex(0);
	tv1 = (CP_Vertex *)b.mf_getVertex(1);
    lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv1->m_point));
    te=new CP_Edge(lineSegment, tv0, tv1);
    b.mf_addEdge(te);
    tv0->mf_addAdjacentEdge(te);
    tv1->mf_addAdjacentEdge(te);
	
	//上底边E2
	CP_Circle3D * circle1 = new  CP_Circle3D(originUp, vx3d, vy3d, r);
	t_vetex = (CP_Vertex *)b.mf_getVertex(1);
	te = new CP_Edge(circle1, t_vetex, t_vetex);
	b.mf_addEdge(te);
	t_vetex->mf_addAdjacentEdge(te);
	
	//E3 v2-v3连线
	//double startAngle = u1 * PI2;
	//double endAngle = u2 * PI2;

	CP_Point3D originCentralBelow = CP_Point3D(cx, cy, cz+h*v1);
	CP_Point3D originCentralUp = CP_Point3D(cx, cy, cz+h*v2);
	//四个原弧线            E3    E5,	E11, E13
	double startAngle[] = {u1*PI2, u3*PI2, u1*PI2, u3*PI2 };
	double endAngle[] =   {u2*PI2, u4*PI2, u2*PI2, u4*PI2};
	int vertexArcArr0[] =   {2,	4,		6,		8};
	int vertexArcArr1[] =   {3,	5,		7,		9};
	CP_Point3D t_origin[]	= {originCentralBelow, originCentralBelow, originCentralUp, originCentralUp};
	//四个横直线段    E4,		E6,	E12,	E14
	int vertexHor0[] = {3,	5,	7,	6};
	int vertexHor1[] = {4,	2,	8,	9};
	//四个竖直线段	E7,E8,E9,E10
	int vertexVet0[] = {2,7,8,5};
	int vertexVet1[] = {6,3,4,9};
	//不方便循环啊，maybe 写个 add_edge(egde,index)
	
	//中空的下底 E3456
	for (unsigned int temp = 0; temp<2; temp++)
	{
		//E3  E5
		CP_Arc3D *e3 = new CP_Arc3D(t_origin[temp], vx3d, vy3d, r, startAngle[temp], endAngle[temp]);
		tv0 = (CP_Vertex *)b.mf_getVertex(vertexArcArr0[temp]);
		tv1 = (CP_Vertex *)b.mf_getVertex(vertexArcArr1[temp]);
		te = new CP_Edge(e3, tv0, tv1);
		b.mf_addEdge(te);
		tv0->mf_addAdjacentEdge(te);
		tv1->mf_addAdjacentEdge(te);

		//E4  E6
		tv0 = (CP_Vertex *)b.mf_getVertex(vertexHor0[temp]);
		tv1 = (CP_Vertex *)b.mf_getVertex(vertexHor1[temp]);
	    lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv1->m_point));
	    te=new CP_Edge(lineSegment, tv0, tv1);
	    b.mf_addEdge(te);
	    tv0->mf_addAdjacentEdge(te);
	    tv1->mf_addAdjacentEdge(te);
	}
	//竖直的E7,8,9,10
	for(unsigned int temp=0; temp<4; temp++)
	{
		tv0 = (CP_Vertex *)b.mf_getVertex(vertexVet0[temp]);
		tv1 = (CP_Vertex *)b.mf_getVertex(vertexVet1[temp]);
	    lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv1->m_point));
	    te=new CP_Edge(lineSegment, tv0, tv1);
	    b.mf_addEdge(te);
	    tv0->mf_addAdjacentEdge(te);
	    tv1->mf_addAdjacentEdge(te);

	}
	//中空的上底 E11,E12,E13,E14
	for (unsigned int temp = 2; temp<4; temp++)
	{
		//E11  E13
		CP_Arc3D *e3 = new CP_Arc3D(t_origin[temp], vx3d, vy3d, r, startAngle[temp], endAngle[temp]);
		tv0 = (CP_Vertex *)b.mf_getVertex(vertexArcArr0[temp]);
		tv1 = (CP_Vertex *)b.mf_getVertex(vertexArcArr1[temp]);
		te = new CP_Edge(e3, tv0, tv1);
		b.mf_addEdge(te);
		tv0->mf_addAdjacentEdge(te);
		tv1->mf_addAdjacentEdge(te);

		//E12  E14
		tv0 = (CP_Vertex *)b.mf_getVertex(vertexHor0[temp]);
		tv1 = (CP_Vertex *)b.mf_getVertex(vertexHor1[temp]);
	    lineSegment=new CP_LineSegment3D(*(tv0->m_point), *(tv1->m_point));
	    te=new CP_Edge(lineSegment, tv0, tv1);
	    b.mf_addEdge(te);
	    tv0->mf_addAdjacentEdge(te);
	    tv1->mf_addAdjacentEdge(te);
	}
#pragma endregion  边填充完毕

#pragma region 填充面

	//下底面
	circle  = new CP_Circle(originLow, vx3d, vy3d, r);
	tf = new CP_Face(&b, circle, true);
	b.mf_addFace(tf);
	CP_Loop * loop = new CP_Loop(tf);
	tf->mf_addLoop(loop);
	CP_Circle2D* circle2D = new CP_Circle2D(origin2d, vx2d, vy2d, r);
	CP_Edge* edge = b.mf_getEdge(0);
	CP_Coedge* coedge = new CP_Coedge(circle2D, loop, edge, false);
	loop->mf_addCoedge(coedge);
	edge->mf_addCoedge(coedge);
	
	{
		//上底面
		circle = new CP_Circle(originUp, vx3d, vy3d, r);
		tf = new CP_Face(&b, circle, true);
		b.mf_addFace(tf);
		CP_Loop *loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		CP_Circle2D * circle2D = new CP_Circle2D(origin2d, vx2d, vy2d, r);
		CP_Edge *edge = b.mf_getEdge(2);
		CP_Coedge	*coedge = new CP_Coedge(circle2D, loop, edge, true);
		loop->mf_addCoedge(coedge);
		edge->mf_addCoedge(coedge);
	}
	
	{
		//苦逼的f2
		//空洞的圆柱面
		CP_CylinderTrimmed *tt = new CP_CylinderTrimmed(originLow,vx3d,vy3d,r,h,u1,u2,u3,u4,v1,v2);
		tf = new CP_Face(&b, tt, true);
		b.mf_addFace(tf);
		loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		//符合右手定则，法向量指向体外
		cf_bodyCubeCreate_loop(b, *loop, 0, true, 1, true, 2, false, 1, false);
		CP_Loop *loop1 = new CP_Loop(tf);
		tf->mf_addLoop(loop1);
		cf_bodyCubeCreate_loop(b, *loop1, 3, false, 7, true, 11, true, 8, true);
		CP_Loop *loop2 = new CP_Loop(tf);
		tf->mf_addLoop(loop2);
		cf_bodyCubeCreate_loop(b, *loop2, 5, false, 9, false, 13, true, 10, false);
	}
	
	{
		//F3 由两个扇形面和 两个两个三角形组成
		CP_ArcSquSurface * arcSqu = new CP_ArcSquSurface(originCentralBelow, vx3d, vy3d, r, u1, u2);
		tf = new CP_Face(&b, arcSqu, true);
		b.mf_addFace(tf);
		CP_Loop *loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		//CP_Arc2D里面没有乘以2PI，真垃圾，不统一
		//e3
		CP_Arc2D *arc1 = new CP_Arc2D(origin2d, vx2d, vy2d, r, u1*PI2, u2*PI2);
		//e5
		CP_Arc2D *arc2 = new CP_Arc2D(origin2d, vx2d, vy2d, r, u3*PI2, u4*PI2);
		//e4
		CP_LineSegment2D *line1 = new CP_LineSegment2D(arc1->mf_getEndingPoint(),arc2->mf_getStartingPoint());
		//e6
		CP_LineSegment2D *line2 = new CP_LineSegment2D(arc2->mf_getEndingPoint(),arc1->mf_getStartingPoint());
		cf_bodyCreate_loop(b,*loop,6,line2,true, 3,arc1,true, 4,line1,true, 5,arc2,true);
	}

	{
		//f4
		CP_ArcSquSurface* arcSqu = new CP_ArcSquSurface(originCentralUp, vx3d, vy3d, r, u1, u2);
		tf = new CP_Face(&b, arcSqu, true);
		b.mf_addFace(tf);
		CP_Loop *loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		//e11
		CP_Arc2D *arc1 = new CP_Arc2D(origin2d, vx2d, vy2d,r,u1*PI2,u2*PI2);
		//e13
		CP_Arc2D *arc2 = new CP_Arc2D(origin2d, vx2d, vy2d, r, u3*PI2, u4*PI2);
		//e12
		CP_LineSegment2D *line1 = new CP_LineSegment2D(arc1->mf_getEndingPoint(),arc2->mf_getStartingPoint());
		//e14
		CP_LineSegment2D *line2 = new CP_LineSegment2D(arc2->mf_getEndingPoint(),arc1->mf_getStartingPoint());
		cf_bodyCreate_loop(b,*loop,14,line2,false, 11,arc1,true, 12,line1,true, 13,arc2,true);
	}

	{
	//f5
		CP_Vertex*	tv = (CP_Vertex*)(b.mf_getVertex(4));
		CP_Point3D cp = *(tv->m_point);
		tv = (CP_Vertex*)(b.mf_getVertex(3));
		CP_Vector3D vx = *(tv->m_point) - cp;//x:V3-V4
		tv = (CP_Vertex*)(b.mf_getVertex(8));
		CP_Vector3D vy = *(tv->m_point) - cp;//y:V8-V4
		CP_Plane * plane = new CP_Plane(cp, vx, vy);
		tf = new CP_Face(&b, plane, true);
		b.mf_addFace(tf);
		loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		cf_bodyCubeCreate_loop(b, *loop,4,false,8,false,12,true,9,true); 
	}
	{
	//f6
		CP_Vertex*	tv = (CP_Vertex*)(b.mf_getVertex(2));
		CP_Point3D cp = *(tv->m_point);
		tv = (CP_Vertex*)(b.mf_getVertex(5));
		CP_Vector3D vx = *(tv->m_point) - cp;//x:V5-V2
		tv = (CP_Vertex*)(b.mf_getVertex(6));
		CP_Vector3D vy = *(tv->m_point) - cp;//y:V6-V2
		CP_Plane * plane = new CP_Plane(cp, vx, vy);
		tf = new CP_Face(&b, plane, true);
		b.mf_addFace(tf);
		loop = new CP_Loop(tf);
		tf->mf_addLoop(loop);
		cf_bodyCubeCreate_loop(b, *loop,6,false,10,true,14,false,7,false); 
	}
	
	
}
void cf_bodyCylinderTrimmedDrawSolid(const CP_Body& b)
{
	glColor3f(1.0, 0.0, 0.0);
    cf_bodyDrawSurface(b, true);
}
void cf_bodyCylinderTrimmedDrawWireframe(const CP_Body& b)
{
	glColor3f(0.0, 1.0, 0.0);
    cf_bodyDrawSurface(b, false);
}
void cf_bodyCylinderTrimmedDrawEdge(const CP_Body& b)
{
	glColor3f(0.0, 0.0, 1.0);
	cf_bodyDrawEdge(b);
}
