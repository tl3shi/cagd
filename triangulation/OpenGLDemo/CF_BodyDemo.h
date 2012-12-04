// CF_BodyDemo.h: 自定义的实体图形生成与绘制函数

#ifndef CF_BODYDEMO_H
#define CF_BODYDEMO_H

#include "GL/GLU.H" // 已经包含GL/GL.h
#include "CP_Topology.h"

extern void cf_bodyCreate_loop(CP_Body& b, CP_Loop& loop, int e1, CP_Curve2D* c1, bool samDir1,
                                                          int e2, CP_Curve2D* c2, bool samDir2,
                                                          int e3, CP_Curve2D* c3, bool samDir3,
                                                          int e4, CP_Curve2D* c4, bool samDir4);

extern void cf_bodyDrawArc(const CP_Arc3D& a, bool solidOrWireframe, bool normalFlag);// solidOrWireframe: true for Solid // normalFlag: true(法向量取正向); false(法向量取反向)
extern void cf_bodyDrawCircle(const CP_Circle3D& c, bool solidOrWireframe, bool normalFlag);// solidOrWireframe: true for Solid // normalFlag: true(法向量取正向); false(法向量取反向)
extern void cf_bodyDrawSurface(const CP_Body& b, bool solidOrWireframe=true); // true for Solid
extern void cf_bodyDrawEdge(const CP_Body& b);

extern void cf_bodyCubeCreate(CP_Body& b, double cx, double cy, double cz, double size);
extern void cf_bodyCubeDrawSolid(const CP_Body& b);
extern void cf_bodyCubeDrawWireframe(const CP_Body& b);
extern void cf_bodyCubeDrawEdge(const CP_Body& b);

extern void cf_bodyCylinderCreate(CP_Body& b, double cx, double cy, double cz, double h, double r);
extern void cf_bodyCylinderDrawSolid(const CP_Body& b);
extern void cf_bodyCylinderDrawWireframe(const CP_Body& b);
extern void cf_bodyCylinderDrawEdge(const CP_Body& b);

#endif