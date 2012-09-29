// tetrahedronView.cpp : CtetrahedronView 类的实现

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "tetrahedron.h"
#endif

#include "tetrahedronDoc.h"
#include "tetrahedronView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GL/GLU.H" 
#include <cmath>

double downPoint[2];
double  upPoint[2];
double angle;
#define  PI 3.14;

IMPLEMENT_DYNCREATE(CtetrahedronView, CView)

	BEGIN_MESSAGE_MAP(CtetrahedronView, CView)
		ON_WM_CONTEXTMENU()
		ON_WM_RBUTTONUP()
		ON_WM_CREATE()
		ON_WM_DESTROY()
		ON_WM_SIZE()
		ON_WM_ERASEBKGND()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MBUTTONUP()
	END_MESSAGE_MAP()

	// CtetrahedronView 构造/析构

	CtetrahedronView::CtetrahedronView()
	{
		// TODO: 在此处添加构造代码

	}

	CtetrahedronView::~CtetrahedronView()
	{
	}

	BOOL CtetrahedronView::PreCreateWindow(CREATESTRUCT& cs)
	{
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式

		return CView::PreCreateWindow(cs);
	}

	// CtetrahedronView 绘制

	void CtetrahedronView::OnDraw(CDC* pDC)
	{
		CtetrahedronDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		wglMakeCurrent(pDC->m_hDC, m_hRC);


		glClearColor(0,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		
		glRotatef(angle, 0.5, 0.5, 0.5);
		
		glBegin(GL_TRIANGLES);
		int a  = 3;
		glColor3d(1, 0, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(a, 0, 0);
		glVertex3d(0.5*a, 0, sqrt(6.0)/3.0*a);
		
		glColor3d(0, 1, 0);
		glVertex3d(0, 0, 0);
		glVertex3d(a, 0, 0);
		glVertex3d(0.5*a, -sqrt(3.0)/2.0*a, 0);
	
		glColor3d(0, 0, 1);
		glVertex3d(a, 0, 0);
		glVertex3d(0.5*a, -sqrt(3.0)/2.0*a, 0);
		glVertex3d(0.5*a, 0, sqrt(6.0)/3.0*a);

		glEnd();
		
		SwapBuffers(pDC->m_hDC);
		wglMakeCurrent(NULL, NULL);
	}

	void CtetrahedronView::OnRButtonUp(UINT /* nFlags */, CPoint point)
	{
		ClientToScreen(&point);
		OnContextMenu(this, point);
	}

	void CtetrahedronView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
	{
#ifndef SHARED_HANDLERS
		theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	}


	// CtetrahedronView 诊断

#ifdef _DEBUG
	void CtetrahedronView::AssertValid() const
	{
		CView::AssertValid();
	}

	void CtetrahedronView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}

	CtetrahedronDoc* CtetrahedronView::GetDocument() const // 非调试版本是内联的
	{
		ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtetrahedronDoc)));
		return (CtetrahedronDoc*)m_pDocument;
	}
#endif //_DEBUG


	// CtetrahedronView 消息处理程序


	int CtetrahedronView::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CView::OnCreate(lpCreateStruct) == -1)
			return -1;

		// The PIXELFORMATDESCRIPTOR structure describes
		//		the pixel format of a drawing surface.
		PIXELFORMATDESCRIPTOR pfd =
		{ 
			sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
			1,                     			// version number 
			PFD_DRAW_TO_WINDOW |   	// support window 
			PFD_SUPPORT_OPENGL |   	// support OpenGL 
			PFD_DOUBLEBUFFER,	// double buffered
			PFD_TYPE_RGBA,
			24,                    	// 24-bit color depth 
			0, 0, 0, 0, 0, 0,      // color bits ignored 
			0,                     	// no alpha buffer 
			0,                     	// shift bit ignored 
			0,                     	// no accumulation buffer 
			0, 0, 0, 0,            	// accum bits ignored 
			32,                    	// 32-bit z-buffer (depth)
			0,                     	// no stencil buffer 
			0,                     	// no auxiliary buffer 
			PFD_MAIN_PLANE,        // main layer 
			0,                     	// reserved 
			0, 0, 0                	// layer masks ignored 
		}; 

		CClientDC dc(this);
		// Get the best available match of pixel format for the device context
		// In other words, if this computer doesn't support features that I
		// asked for, try to get the next best thing.  i.e. 16-bit color mode
		// instead of 24-bit color mode.
		int pixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);

		// Set the pixel format to the best pixel format I can get (see above)
		// and if that operation fails, bring up a message box that tells the
		// user the error.
		if (!SetPixelFormat(dc.m_hDC, pixelFormat, &pfd))
		{
			MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( )"),
				_T("Application Error"), MB_ICONERROR);
		}

		// Creates an OpenGL rendering context so that OpenGL knows how to draw
		// to this view. You can't use OpenGL in MFC without using the handle
		// that this function returns
		m_hRC = wglCreateContext(dc.m_hDC);
		return 0;
	}


	void CtetrahedronView::OnDestroy()
	{
		CView::OnDestroy();
		// ////////////////////////////////////////////////////////////////
		// New codes begin:

		// Set : a specified OpenGL rendering context ==> NULL
		// Set : current rendering context ==> NULL
		wglMakeCurrent(NULL, NULL);

		// Delete the handle to an OpenGL rendering context 
		wglDeleteContext(m_hRC);
		m_hRC=NULL;
		// New codes end.
		// ////////////////////////////////////////////////////////////////

	}


	void CtetrahedronView::OnSize(UINT nType, int cx, int cy)
	{
		CView::OnSize(nType, cx, cy);

		// ////////////////////////////////////////////////////////////////
		// New codes begin:

		CClientDC dc(this);
		wglMakeCurrent(dc.m_hDC, m_hRC);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		double d=10;
		double n=100;

		glOrtho(-cx/n, cx/n, -cy/n, cy/n, -d, d);

		glMatrixMode(GL_MODELVIEW);
		glViewport(0, 0, cx, cy);
		wglMakeCurrent(NULL, NULL);

		// New codes end.
		// ////////////////////////////////////////////////////////////////

	}


	BOOL CtetrahedronView::OnEraseBkgnd(CDC* pDC)
	{
		return TRUE;
		//return CView::OnEraseBkgnd(pDC);
	}

	void CtetrahedronView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		CRect rect;
		GetClientRect(&rect);
		downPoint[0] = point.x;
		downPoint[1] = rect.Height() - point.y;
		CView::OnLButtonDown(nFlags, point);
	}


	void CtetrahedronView::OnLButtonUp(UINT nFlags, CPoint point)
	{

		CRect rect;
		GetClientRect(&rect);
		upPoint[0] = point.x;
		upPoint[1] = rect.Height() - point.y;
		//此坐标是以左下角为原点的坐标，得转换成opengl中心坐标系
		upPoint[0] = upPoint[0] - rect.Width()/2.0 ;
		upPoint[1] = rect.Height()/2.0 - upPoint[1];
		
		downPoint[0] = downPoint[0] - rect.Width()/2.0 ;
		downPoint[1] = rect.Height()/2.0 - downPoint[1];
		
		int ab = downPoint[0]*upPoint[0] + downPoint[1]*upPoint[1];
		double a = sqrt(pow((double)downPoint[0],2) + pow((double)downPoint[1],2));
		double b = sqrt(pow((double)upPoint[0],2) + pow((double)upPoint[1],2));

		angle = acos(ab/(a*b))*180/PI;
		Invalidate(FALSE);
		CView::OnLButtonUp(nFlags, point);
	}


	void CtetrahedronView::OnMButtonUp(UINT nFlags, CPoint point)
	{
		angle += 30.0;
		if (angle > 360.0){
			angle = 0.0;
		}
		Invalidate(FALSE);
		CView::OnMButtonUp(nFlags, point);
	}
