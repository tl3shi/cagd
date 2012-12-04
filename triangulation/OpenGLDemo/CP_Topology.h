// CP_Topology.h: 定义类CP_Body、CP_Face、CP_Loop、CP_Coedge、CP_Edge、CP_Vertex
#ifndef CP_TOPOLOGY_H
#define CP_TOPOLOGY_H

#include "CP_Curve2D.h"
#include "CP_Curve3D.h"
#include "CP_Surface3D.h"

class CP_Body;
class CP_Face;
class CP_Loop;
class CP_Coedge;
class CP_Edge;
class CP_Vertex;

class CP_Body : public CObject
{
protected:
    CObArray m_faceArray;     // 组成体的面组
    CObArray m_edgeArray;     // 组成体的边组
    CObArray m_vertexArray;   // 组成体的顶点组
public:
    CP_Body( ) { };
    virtual ~CP_Body( );

    CP_Face*    mf_getFace(int index) const;
    CP_Edge*    mf_getEdge(int index) const;
    CP_Vertex*  mf_getVertex(int index) const;
    int         mf_getFaceNumber( ) const {return (int)m_faceArray.GetSize();};
    int         mf_getEdgeNumber( ) const {return (int)m_edgeArray.GetSize();};
    int         mf_getVertexNumber( ) const {return (int)m_vertexArray.GetSize();};
    int         mf_findFaceIndex(CP_Face *face) const;
    int         mf_findEdgeIndex(CP_Edge *edge) const;
    int         mf_findVertexIndex(CP_Vertex *vertex) const;
    void        mf_addFace(CP_Face *face);
    void        mf_addEdge(CP_Edge *edge);
    void        mf_addVertex(CP_Vertex *vertex);
    void        mf_removeFace(int index);
    void        mf_removeEdge(int index);
    void        mf_removeVertex(int index);
    void        mf_removeAllFace( );
    void        mf_removeAllEdge( );
    void        mf_removeAllVertex( );
}; // 类CP_Body定义结束

class CP_Face : public CObject
{
public:
    CP_Body* m_parent; // 不由类CP_Face负责的数据

    // 由类CP_Face负责的数据
    CP_Surface3D* m_surface;
    bool          m_sameNormalDirection; // 与m_surface的自然法向量相同还是相反
protected:
    CObArray m_loopArray; // 第一个是外环，之后均为内环，内环之间的并列的，即任一内环均不会在其他内环的内部

public:
    CP_Face( );
    CP_Face(CP_Body *parent, CP_Surface3D *surf, bool samedir);
    virtual ~CP_Face( );

    int         mf_getLoopNumber( ) const {return (int)m_loopArray.GetSize();};
    CP_Loop*    mf_getLoop(int index) const;
    int         mf_findLoopIndex(CP_Loop *loop) const;
    void        mf_addLoop(CP_Loop *loop);
    void        mf_removeLoop(int index);
    void        mf_removeAllLoop( );
}; // 类CP_Face定义结束

class CP_Loop : public CObject
{
public:
    CP_Face* m_parent; // 不由类CP_Loop负责的数据
protected:
    CObArray m_coedgeArray; // 由类CP_Loop负责的数据
public:
    CP_Loop( );
    CP_Loop(CP_Face* face);
    virtual ~CP_Loop( );

    int         mf_getCoedgeNumber( ) const {return (int)m_coedgeArray.GetSize();};
    CP_Coedge*  mf_getCoedge(int index) const;
    int         mf_findCoedgeIndex(CP_Coedge *coedge) const;
    void        mf_addCoedge(CP_Coedge *coedge);
    void        mf_removeCoedge(int index);
    void        mf_removeAllCoedge( );
}; // 类CP_Loop定义结束

class CP_Coedge : public CObject
{
public:
    CP_Curve2D* m_curve2D; // 由类CP_Coedge负责的数据
    CP_Loop*    m_parent;  // 不由类CP_Coedge负责的数据
    CP_Edge*    m_edge;    // 不由类P_Coedge负责的数据
    bool        m_sameDirection;
public:
    CP_Coedge( );
    CP_Coedge(CP_Curve2D *curve, CP_Loop *parent, CP_Edge *edge, bool sameDir);
    virtual ~CP_Coedge( );
}; // 类CP_Coedge定义结束

class CP_Edge : public CObject
{
public:
    CP_Curve3D*  m_curve3D;
    CP_Vertex*   m_startingVertex;
    CP_Vertex*   m_endingVertex;
protected:
    CObArray m_coedgeArray; // 对应的环边(CP_Coedge)集合
public:
    CP_Edge( );
    CP_Edge(CP_Curve3D* c, CP_Vertex* s, CP_Vertex* e);
    virtual ~CP_Edge( );

    int        mf_getCoedgeNumber( ) const {return (int)m_coedgeArray.GetSize();};
    CP_Coedge* mf_getCoedge(int index) const;
    int        mf_findCoedgeIndex(CP_Coedge *coedge) const;
    void       mf_addCoedge(CP_Coedge *coedge) {m_coedgeArray.Add(coedge);};
    void       mf_removeCoedge(int index) {m_coedgeArray.RemoveAt(index);};
    void       mf_removeAllCoedge( ) {m_coedgeArray.RemoveAll( );};
}; // 类CP_Edge定义结束

class CP_Vertex : public CObject
{
public:
    CP_Point3D*    m_point;
protected:
    CObArray    m_adjacentEdgeArray; // 以该顶点为端点的边(CP_Edge)集合
public:
    CP_Vertex( );
    CP_Vertex(CP_Point3D* p);
    virtual ~CP_Vertex( );

    int         mf_getAdjacentEdgeNumber( ) const {return (int)m_adjacentEdgeArray.GetSize();};
    CP_Edge*    mf_getAdjacentEdge(int index) const;
    int         mf_findAdjacentEdgeIndex(CP_Edge *edge) const;
    void        mf_addAdjacentEdge(CP_Edge *edge) {m_adjacentEdgeArray.Add(edge);};
    void        mf_removeAdjacentEdge(int index) {m_adjacentEdgeArray.RemoveAt(index);};
    void        mf_removeAllAdjacentEdge( ) {m_adjacentEdgeArray.RemoveAll( );};
}; // 类CP_Vertex定义结束

#endif