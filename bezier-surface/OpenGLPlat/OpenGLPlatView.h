
// OpenGLPlatView.h : COpenGLPlatView 类的接口
//

#pragma once
#include "CP_Surface3D.h"

class COpenGLPlatView : public CView
{
protected: // 仅从序列化创建
	COpenGLPlatView();
	DECLARE_DYNCREATE(COpenGLPlatView)

// 特性
public:
	COpenGLPlatDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~COpenGLPlatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	// OpenGl渲染句柄(a handle to an OpenGL rendering context)
	HGLRC m_hRC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	int m_rX;
	int m_rY;
	CP_Bezier3D m_bezier;
public:
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // OpenGLPlatView.cpp 中的调试版本
inline COpenGLPlatDoc* COpenGLPlatView::GetDocument() const
   { return reinterpret_cast<COpenGLPlatDoc*>(m_pDocument); }
#endif

