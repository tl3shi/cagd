// CP_Topology.cpp: 实现CP_Body、CP_Face、CP_Loop、CP_Coedge、CP_Edge、CP_Vertex
#include "stdafx.h"
#include "CP_Topology.h"
#include "GL/GLU.H" // 已经包含GL/GL.h

#include <math.h>

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Body开始

CP_Body::~CP_Body( )
{
    int i;
    int n = (int)m_faceArray.GetSize( );
    for (i = 0; i < n; i++)
        delete m_faceArray[i];
    m_faceArray.RemoveAll( );

    n = (int)m_edgeArray.GetSize( );
    for (i = 0; i < n; i++)
        delete m_edgeArray[i];
    m_edgeArray.RemoveAll( );

    n = m_vertexArray.GetSize( );
	/*
    for (i = 0; i < n; i++)
        delete m_vertexArray[i];
    m_vertexArray.RemoveAll( );
	*/
} // 类CP_Body的析构函数结束

CP_Face* CP_Body::mf_getFace(int index) const
{
    int n = (int)m_faceArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Face *)(m_faceArray[index]);
} // 成员函数mf_getFace结束

CP_Edge* CP_Body::mf_getEdge(int index) const
{
    int n = m_edgeArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Edge *)(m_edgeArray[index]);
} // 成员函数mf_getEdge结束

CP_Vertex* CP_Body::mf_getVertex(int index) const
{
    int n = m_vertexArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Vertex *)(m_vertexArray[index]);
} // 成员函数mf_getVertex结束

int CP_Body::mf_findFaceIndex(CP_Face *face) const
{
    int n = (int)m_faceArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_faceArray[i] == face)
            return i;
    return -1;
} // 成员函数mf_findFaceIndex结束

int CP_Body::mf_findEdgeIndex(CP_Edge *edge) const
{
    int n = (int)m_edgeArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_edgeArray[i] == edge)
            return i;
    return -1;
} // 成员函数mf_findEdgeIndex结束

int CP_Body::mf_findVertexIndex(CP_Vertex *vertex) const
{
    int n = (int)m_vertexArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_vertexArray[i] == vertex)
            return i;
    return -1;
} // 成员函数mf_findVertexIndex结束

void CP_Body::mf_addFace(CP_Face *face)
{
    m_faceArray.Add(face);
} // 成员函数mf_addFace结束

void CP_Body::mf_addEdge(CP_Edge *edge)
{
    m_edgeArray.Add(edge);
} // 成员函数mf_addEdge结束

void CP_Body::mf_addVertex(CP_Vertex *vertex)
{
    m_vertexArray.Add(vertex);
} // 成员函数mf_addVertex结束

void CP_Body::mf_removeFace(int index)
{
    CP_Face* face=mf_getFace(index);
    if (face!=NULL)
    {
        delete face;
        m_faceArray.RemoveAt(index);
    } // if 结构结束
} // 成员函数mf_removeFace结束

void CP_Body::mf_removeEdge(int index)
{
    CP_Edge* edge=mf_getEdge(index);
    if (edge!=NULL)
    {
        delete edge;
        m_edgeArray.RemoveAt(index);
    } // if 结构结束
} // 成员函数mf_removeEdge结束

void CP_Body::mf_removeVertex(int index)
{
    CP_Vertex* v=mf_getVertex(index);
    if (v!=NULL)
    {
        delete v;
        m_vertexArray.RemoveAt(index);
    } // if 结构结束
} // 成员函数mf_removeVertex结束

void CP_Body::mf_removeAllFace()
{
    int n = (int)m_faceArray.GetSize( );
    for (int i = 0; i < n; i++)
        delete m_faceArray[i];
    m_faceArray.RemoveAll( );
} // 成员函数mf_removeAllFace结束

void CP_Body::mf_removeAllEdge()
{
    int n = (int)m_edgeArray.GetSize( );
    for (int i = 0; i < n; i++)
        delete m_edgeArray[i];
    m_edgeArray.RemoveAll( );
} // 成员函数mf_removeAllEdge结束

void CP_Body::mf_removeAllVertex()
{
    int n = (int)m_vertexArray.GetSize( );
    for (int i = 0; i < n; i++)
        delete m_vertexArray[i];
    m_vertexArray.RemoveAll( );
} // 成员函数mf_removeAllVertex结束

// 实现类CP_Body结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Face开始
CP_Face::CP_Face( ): m_parent(NULL), m_surface(NULL), m_sameNormalDirection(true)
{
} // 类CP_Face的构造函数结束

CP_Face::CP_Face(CP_Body *parent, CP_Surface3D *surf, bool samedir)
    : m_parent(parent), m_surface(surf), m_sameNormalDirection(samedir)
{
} // 类CP_Face的构造函数结束

CP_Face::~CP_Face( )
{
	delete m_surface;
    int n=(int)m_loopArray.GetSize( );
	for (int i = 0; i < n; i++)
		delete m_loopArray[i];
	m_loopArray.RemoveAll( );
} // 类CP_Face的析构函数结束

CP_Loop* CP_Face::mf_getLoop(int index) const
{
    int n = (int)m_loopArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Loop *)(m_loopArray[index]);
} // 成员函数mf_getLoop结束

int CP_Face::mf_findLoopIndex(CP_Loop *loop) const
{
    int n = (int)m_loopArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_loopArray[i] == loop)
            return i;
    return -1;
} // 成员函数mf_findLoopIndex结束

void CP_Face::mf_addLoop(CP_Loop *loop)
{
    m_loopArray.Add(loop);
} // 成员函数mf_addLoop结束

void CP_Face::mf_removeLoop(int index)
{
    CP_Loop* loop=mf_getLoop(index);
    if (loop!=NULL)
    {
        delete loop;
        m_loopArray.RemoveAt(index);
    } // if 结构结束
} // 成员函数mf_removeLoop结束

void CP_Face::mf_removeAllLoop()
{
    int n=(int)m_loopArray.GetSize( );
	for (int i = 0; i < n; i++)
		delete m_loopArray[i];
	m_loopArray.RemoveAll( );
} // 成员函数mf_removeAllLoop结束

// 实现类CP_Face结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Loop开始
CP_Loop::CP_Loop( ): m_parent(NULL)
{
} // 类CP_Loop的构造函数结束

CP_Loop::CP_Loop(CP_Face* face): m_parent(face)
{
} // 类CP_Loop的构造函数结束

CP_Loop::~CP_Loop( )
{
    int n = (int)m_coedgeArray.GetSize();
    for (int i=0; i<n; i++)
        delete m_coedgeArray[i];
    m_coedgeArray.RemoveAll();
} // 类CP_Loop的析构函数结束

CP_Coedge* CP_Loop::mf_getCoedge(int index) const
{
    int n = (int)m_coedgeArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Coedge *)(m_coedgeArray[index]);
} // 成员函数mf_getCoedge结束

int CP_Loop::mf_findCoedgeIndex(CP_Coedge *coedge) const
{
    int n = (int)m_coedgeArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_coedgeArray[i] == coedge)
            return i;
    return -1;
} // 成员函数mf_findCoedgeIndex结束

void CP_Loop::mf_addCoedge(CP_Coedge *coedge)
{
    m_coedgeArray.Add(coedge);
} // 成员函数mf_addCoedge结束

void CP_Loop::mf_removeCoedge(int index)
{
    CP_Coedge* coedge=mf_getCoedge(index);
    if (coedge!=NULL)
    {
        delete coedge;
        m_coedgeArray.RemoveAt(index);
    } // if 结构结束
} // 成员函数mf_removeCoedge结束

void CP_Loop::mf_removeAllCoedge( )
{
    int n = (int)m_coedgeArray.GetSize();
    for (int i=0; i<n; i++)
        delete m_coedgeArray[i];
    m_coedgeArray.RemoveAll();
} // 成员函数mf_removeAllCoedge结束

// 实现类CP_Loop结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Coedge开始
CP_Coedge::CP_Coedge( ) : m_curve2D(NULL), m_parent(NULL), m_edge(NULL), m_sameDirection(true)
{
} // 类CP_Coedge的构造函数结束

CP_Coedge::CP_Coedge(CP_Curve2D *curve, CP_Loop *parent, CP_Edge *edge, bool sameDir)
    : m_curve2D(curve), m_parent(parent), m_edge(edge), m_sameDirection(sameDir)
{
} // 类CP_Coedge的构造函数结束

CP_Coedge::~CP_Coedge( )
{
    delete m_curve2D;
} // 类CP_Coedge的析构函数结束

// 实现类CP_Coedge结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Edge开始
CP_Edge::CP_Edge( ) : m_curve3D(NULL), m_startingVertex(NULL), m_endingVertex(NULL)
{
} // 类CP_Edge的构造函数结束

CP_Edge::CP_Edge(CP_Curve3D* c, CP_Vertex* s, CP_Vertex* e)
    : m_curve3D(c), m_startingVertex(s), m_endingVertex(e)
{
} // 类CP_Edge的构造函数结束

CP_Edge::~CP_Edge( )
{
    delete m_curve3D;
} // 类CP_Edge的析构函数结束

CP_Coedge* CP_Edge::mf_getCoedge(int index) const
{
    int n = (int)m_coedgeArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Coedge *)(m_coedgeArray[index]);
} // 成员函数mf_getCoedge结束

int CP_Edge::mf_findCoedgeIndex(CP_Coedge *coedge) const
{
    int n = (int)m_coedgeArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_coedgeArray[i] == coedge)
            return i;
    return -1;
} // 成员函数mf_findCoedgeIndex结束

// 实现类CP_Edge结束
// ////////////////////////////////////////////////////////////////////////////

// ////////////////////////////////////////////////////////////////////////////
// 实现类CP_Vertex开始
CP_Vertex::CP_Vertex( ): m_point(NULL)
{
} // 类CP_Vertex的构造函数结束

CP_Vertex::CP_Vertex(CP_Point3D* p): m_point(p)
{
} // 类CP_Vertex的构造函数结束

CP_Vertex::~CP_Vertex( )
{
    delete m_point;
} // 类CP_Vertex的析构造函数结束

CP_Edge* CP_Vertex::mf_getAdjacentEdge(int index) const
{
    int n = (int)m_adjacentEdgeArray.GetSize( );
    if (index<0 || index>=n)
        return NULL;
	return (CP_Edge *)(m_adjacentEdgeArray[index]);
} // 成员函数mf_getAdjacentEdge结束

int CP_Vertex::mf_findAdjacentEdgeIndex(CP_Edge *edge) const
{
    int n = (int)m_adjacentEdgeArray.GetSize();
    for (int i=0; i<n; i++)
        if (m_adjacentEdgeArray[i] == edge)
            return i;
    return -1;
} // 成员函数mf_findAdjacentEdgeIndex结束

// 实现类CP_Vertex结束
// ////////////////////////////////////////////////////////////////////////////

