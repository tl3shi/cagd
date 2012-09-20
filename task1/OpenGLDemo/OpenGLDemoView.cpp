
// OpenGLDemoView.cpp : COpenGLDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenGLDemo.h"
#endif

#include <GL/GLU.H> // 已经包含GL.h

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
// COpenGLDemoView 绘制

void COpenGLDemoView::OnDraw(CDC* pDC)
{
	COpenGLDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	wglMakeCurrent(pDC->m_hDC, m_hRC);

	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
/*
	int n=9;
	double a=4;
	double s=0;
	double ds=1.0/(n-1);
	double t=-4;
	double dt=-2*t/(n-1);
	double x, y;
	glBegin(GL_POLYGON);
	for (int i=0; i< n; i++, s+=ds, t+=dt)
	{
		x=(1-t*t)*s/(t*t+1);
		y=2*s*t/(t*t+1);
		glColor3d(1, s, (t+4)/16);
		glVertex3d(x*a, y*a, 0);
	} // for结构结束
	glEnd();
*/
	//T --begin
	double x0 = -5.0;
	double y0 = 0.0;
	glColor3d(0.5, 0.0, 0.0); 
	//glBegin(GL_POLYGON); 
	glBegin(GL_LINE_LOOP); 
	glVertex2f(x0, y0); 
	glVertex2f(x0, y0 + 2);
	glVertex2f(x0-1, y0 + 2);
	glVertex2f(x0-1, y0 + 2 + 0.25);
	glVertex2f(x0 + 1.25, y0 + 2 + 0.25);
	glVertex2f(x0 + 1.25, y0 + 2);
	glVertex2f(x0 + 0.25, y0 + 2);
	glVertex2f(x0 + 0.25, y0);
	glEnd();
	//T --end
	//A --begin
	GLfloat x1 = x0 + 1.25 + 0.25;
	GLfloat y1 = y0;
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1, y1);
	glVertex2f(x1+0.25+(y1+2.25)/sqrt(3.0), y1+2.25);
	glVertex2f(x1+0.5+2*(y1+2.25)/sqrt(3.0), y1);
	glVertex2f(x1+0.25+2*(y1+2.25)/sqrt(3.0), y1);
	glVertex2f(x1-0.25+2*(y1+2.25)/sqrt(3.0), y1+0.5);
	glVertex2f(x1-0.5*4+2*(y1+2.25)/sqrt(3.0), y1+0.5);
	glVertex2f(x1+0.25, y1);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1-0.5*4+2*(y1+2.25)/sqrt(3.0)+0.25/sqrt(2.0), y1+0.5 +0.25);
	glVertex2f(x1-0.25+2*(y1+2.25)/sqrt(3.0), y1+0.5+0.25);
	glVertex2f(x1+0.25+(y1+2.25)/sqrt(3.0), y1+2.25-0.25*sqrt(2.0));
	glEnd();
	//A --end
	//N --begin
	double x2 = x1+0.5+2*(y1+2.25)/sqrt(3.0)+0.25;
	double y2 = y0;

	double x2_end = x2 + 2;
	glBegin(GL_LINE_LOOP);
	glVertex2f(x2, y0);
	glVertex2f(x2, y0+2);
	glVertex2f(x2+0.25, y0+2);
	glVertex2f(x2_end-0.25, y0+0.25*sqrt(2.0));
	glVertex2f(x2_end-0.25, y0+2);
	glVertex2f(x2_end, y0+2);
	glVertex2f(x2_end, y0);
	glVertex2f(x2_end-0.25, y0);
	glVertex2f(x2+0.25, y0+2-0.25*sqrt(2.0));
	glVertex2f(x2+0.25, y0);
	glEnd();
	//N --end;
	
	//G --begin
	double dx = x2_end + 1.75;
	double dy = y0+1.0;
	double PI = 3.14159265;
	glBegin(GL_POINTS);
	for(float angle=0.75*PI;angle<2.25*PI;angle+=0.01f)
	{
		double x=1.0f*sin(angle);
		double y=1.0f*cos(angle);
		glVertex2f(dx+x,dy+y);
		
		double xx=1.25f*sin(angle);
		double yy=1.25f*cos(angle);
		glVertex2f(dx+xx,dy+yy);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	double g_x = dx + sin(0.75*PI);
	double g_y = dy + cos(0.75*PI);
	glVertex2f(g_x,g_y);
	glVertex2f(g_x-0.25,g_y);
	glVertex2f(g_x-0.25,g_y+0.25);
	glVertex2f(g_x+0.50,g_y+0.25);
	glVertex2f(g_x+0.5,g_y-0.5);
	glVertex2f(g_x+0.25,g_y-0.5);
	glVertex2f(dx + 1.25*sin(0.75*PI),g_y-0.25);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(dx+sin(2.25*PI),dy+cos(2.25*PI));
		glVertex2f(dx+1.25*sin(2.25*PI),dy+1.25*cos(2.25*PI));
	glEnd();
	//G --end;

	//L --begin
	double l_x = x0, l_y = y0-4;
	glBegin(GL_LINE_LOOP);
	glVertex2f(l_x,l_y);
	glVertex2f(l_x,l_y+2);
	glVertex2f(l_x+0.25,l_y+2);
	glVertex2f(l_x+0.25,l_y+0.25);
	glVertex2f(l_x+1.5,l_y+0.25);
	glVertex2f(l_x+1.5,l_y);
	glEnd();
	//L --end
	//E --begin
	double e_x = l_x+2, e_y=l_y;
	glBegin(GL_LINE_LOOP);
	glVertex2f(e_x,e_y);
	glVertex2f(e_x,e_y+2);
	glVertex2f(e_x+1.5,e_y+2);
	glVertex2f(e_x+1.5,e_y+2-0.25);
	glVertex2f(e_x+0.25,e_y+2-0.25);
	glVertex2f(e_x+0.25,e_y+(2-0.25*3)/2+0.5);
	glVertex2f(e_x+1.5,e_y+(2-0.25*3)/2+0.5);
	glVertex2f(e_x+1.5,e_y+(2-0.25*3)/2+0.25);
	glVertex2f(e_x+0.25,e_y+(2-0.25*3)/2+0.25);
	glVertex2f(e_x+0.25,e_y+0.25);
	glVertex2f(e_x+1.5,e_y+0.25);
	glVertex2f(e_x+1.5,e_y);
	glEnd();
	//E --end

	//I --begin
	double i_x = e_x + 3, i_y = l_y;
	glBegin(GL_LINE_LOOP);
	glVertex2f(i_x,i_y);
	glVertex2f(i_x,i_y+2);
	glVertex2f(i_x+0.25,i_y+2);
	glVertex2f(i_x+0.25,i_y);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex2f(i_x,i_y+2.1);
	glVertex2f(i_x,i_y+2.25);
	glVertex2f(i_x+0.25,i_y+2.25);
	glVertex2f(i_x+0.25,i_y+2.1);
	glEnd();

	//glPointSize(8.0);
	//glBegin(GL_POINTS);
	//glVertex2f(i_x+0.125, i_y+2.125);
	//glEnd();
	//I --end

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
