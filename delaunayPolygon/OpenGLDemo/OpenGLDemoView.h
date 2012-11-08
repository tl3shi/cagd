
// OpenGLDemoView.h : COpenGLDemoView 类的接口
//

#pragma once


class COpenGLDemoView : public CView
{
protected: // 仅从序列化创建
	COpenGLDemoView();
	DECLARE_DYNCREATE(COpenGLDemoView)

// 特性
public:
	COpenGLDemoDoc* GetDocument() const;

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
	virtual ~COpenGLDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	// OpenGL渲染句柄(a handle to an OpenGL rendering context)
	HGLRC m_hRC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void Onfilltriangle();
	afx_msg void OnAddPoints();
};

#ifndef _DEBUG  // OpenGLDemoView.cpp 中的调试版本
inline COpenGLDemoDoc* COpenGLDemoView::GetDocument() const
   { return reinterpret_cast<COpenGLDemoDoc*>(m_pDocument); }
#endif

