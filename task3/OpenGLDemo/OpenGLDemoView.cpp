
// OpenGLDemoView.cpp : COpenGLDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenGLDemo.h"
#endif

#include <GL/GLU.H> // 已经包含GL.h
#include "CP_Curve2D.H"

#include "OpenGLDemoDoc.h"
#include "OpenGLDemoView.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenGLDemoView

IMPLEMENT_DYNCREATE(COpenGLDemoView, CView)

BEGIN_MESSAGE_MAP(COpenGLDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// COpenGLDemoView 构造/析构

COpenGLDemoView::COpenGLDemoView()
{
	// TODO: 在此处添加构造代码

}

COpenGLDemoView::~COpenGLDemoView()
{
}

BOOL COpenGLDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}
void drawOneLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glBegin(GL_LINES); 
	glVertex2f(x1, y1); 
	glVertex2f(x2, y2); 
	glEnd();
}

CP_Vector2D getBezierPoint(CP_Vector2D controlPoints[], double t, int i, int j)
{
	if (j == 0)
	{
		return controlPoints[i];
	}
	return getBezierPoint(controlPoints, t, i-1, j-1) * (1-t)+ getBezierPoint(controlPoints, t, i, j-1) * t;
}

void COpenGLDemoView::OnDraw(CDC* pDC)
{
	COpenGLDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    wglMakeCurrent(pDC->m_hDC, m_hRC);
    
	glClearColor(0,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0); // 红色
    glLoadIdentity();
    
	const int maxControlPoint = 4;
	CP_Vector2D controlPoints[maxControlPoint];
	controlPoints[0] = CP_Vector2D(0.0, 0.0);
	controlPoints[1] = CP_Vector2D(1.0, 3.0);
	controlPoints[2] = CP_Vector2D(2.0, 1.0);
	controlPoints[3] = CP_Vector2D(3.0, 3.0);
	
	/*
	CP_Vector2D p = getBezierPoint(controlPoints, 0.33333, 3, 3);
	CString msg;
	msg.Format(L"x=%f,y=%f", p.m_x, p.m_y);
	MessageBox(_T(" " + msg));
	*/
	int segment = 5;	
		
	glBegin(GL_LINES);
	double t = 0;
	for (int i = 0; i < segment; i++)
	{
		t += 1/segment;
		CP_Vector2D p = getBezierPoint(controlPoints, t, 3, 3);
		glVertex2d(p.m_x, p.m_y);
	}
	glEnd();
	glFlush();
	
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);

}


// COpenGLDemoView 打印


void COpenGLDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


BOOL COpenGLDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void COpenGLDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void COpenGLDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void COpenGLDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COpenGLDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COpenGLDemoView 诊断

#ifdef _DEBUG
void COpenGLDemoView::AssertValid() const
{
	CView::AssertValid();
}

void COpenGLDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenGLDemoDoc* COpenGLDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenGLDemoDoc)));
	return (COpenGLDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenGLDemoView 消息处理程序


int COpenGLDemoView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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
		MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( ) "),
			_T("Application Error"), MB_ICONERROR);
	}
	// Creates an OpenGL rendering context so that OpenGL knows how to draw
	// to this view. You can't use OpenGL in MFC without using the handle
	// that this function returns
	m_hRC = wglCreateContext(dc.m_hDC);
	return 0;
}


void COpenGLDemoView::OnDestroy()
{
	CView::OnDestroy();
	// Set : a specified OpenGL rendering context ==> NULL
	// Set : current rendering context ==> NULL
	wglMakeCurrent(NULL, NULL);

	// Delete the handle to an OpenGL rendering context 
	wglDeleteContext(m_hRC);
	m_hRC=NULL;


}


void COpenGLDemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
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

}


BOOL COpenGLDemoView::OnEraseBkgnd(CDC* pDC)
{

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
