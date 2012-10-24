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

#include <algorithm>
#include <vector>
#include <math.h>
#include "CP_Surface3D.h"
using namespace std;


vector<CP_Vector2D> ctrlPoints;//vector模拟点，de Casteljau算法时方便计算
bool isReady = false;//鼠标双击后开始画bezier曲线，否则用虚线画控制点轨迹
CP_Vector2D hoverPoint;//记录即将选择的控制点坐标
int besierSegment = 30;//分为多少段
bool firstClick = false;//有第一个点后才开始记录 mouse坐标，生成虚线
const int MAXCTRPOINT = 10;
bool enableMaxCtrlPoint = false;

// COpenGLDemoView

IMPLEMENT_DYNCREATE(COpenGLDemoView, CView)

BEGIN_MESSAGE_MAP(COpenGLDemoView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &COpenGLDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void initLights();

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

CP_Vector2D getBezierPoint(CP_Vector2D controlPoints[], double t, int i, int j)
{
	if (j == 0)
	{
		return controlPoints[i];
	}
	return getBezierPoint(controlPoints, t, i-1, j-1) * (1-t)+ getBezierPoint(controlPoints, t, i, j-1) * t;
}

CP_Vector2D getBezierPoint(vector<CP_Vector2D> controlPoints, double t, int i, int j)
{
	if (j == 0)
	{
		return controlPoints[i];
	}
	return getBezierPoint(controlPoints, t, i-1, j-1) * (1-t)+ getBezierPoint(controlPoints, t, i, j-1) * t;
}

CP_Vector2D getBezierPointNotRecurrent(vector<CP_Vector2D> controlPoints, double t)
{
	vector<CP_Vector2D> tempPoints(controlPoints);
	for (unsigned int i = 1; i <= tempPoints.size(); i++){
		for (unsigned int j = 0; j < tempPoints.size()-i; j++)
			tempPoints[j] = tempPoints[j] * (1-t) + tempPoints[j+1] * t; 
	}
	return tempPoints[0];
}

CP_Vector3D getBezierPointNotRecurrent(vector<CP_Vector3D> controlPoints, double t)
{
	vector<CP_Vector3D> tempPoints(controlPoints);
	for (unsigned int i = 1; i <= tempPoints.size(); i++){
		for (unsigned int j = 0; j < tempPoints.size()-i; j++)
			tempPoints[j] = tempPoints[j] * (1-t) + tempPoints[j+1] * t; 
	}
	return tempPoints[0];
}

/************************************************************************/
/* B(i,n t) = n!/(i!(n-i)!) t^i (1-t)*n-i  
*	n: 控制点数量-1 
	i:控制点序号
	t:参数
/************************************************************************/
double B(int i, int n, double t)
{
	assert (n >= i);
	assert (t >= 0);
	assert (t <= 1);
	double result = 1;
	for (unsigned j = n; j > n-i ; j-- ){
		result*=j;
	}
	for (unsigned j = i; j > 1; j--){
		result /= j;
	}
	result = result * pow(t, i) * pow(1-t, n-i);
	return result;
}
//demo 单位阵
void drawBizerSample()
{
	const int maxControlPoint = 4;
	CP_Vector2D controlPoints[maxControlPoint];
	controlPoints[0] = CP_Vector2D(0.0, 0.0);
	controlPoints[1] = CP_Vector2D(1.0, 3.0);
	controlPoints[2] = CP_Vector2D(2.0, 1.0);
	controlPoints[3] = CP_Vector2D(3.0, 3.0);

	glLoadIdentity();
	glColor3d(1.0, 0, 0);
	glBegin(GL_LINE_STRIP);
	double t = 0;
	for (unsigned int i = 0; i < besierSegment; i++)
	{
		t += 1.0/besierSegment;
		CP_Vector2D p = getBezierPoint(controlPoints, t, 3, 3);
		glVertex2d(p.m_x, p.m_y);
	}
	glEnd();
	
	glLoadIdentity();
	glTranslated(-0.5, 0.0, 0.0);   
	glColor3d(1.0, 0, 1.0);
	glBegin(GL_LINE_STRIP);
	t = 0;
	for (unsigned int i = 0; i < besierSegment; i++)
	{
		t += 1.0/besierSegment;
		CP_Vector2D p;
		for (unsigned kk = 0 ; kk < maxControlPoint; kk++) 
		{
			p += controlPoints[kk] * B(kk, 3, t);
		}

		glVertex2d(p.m_x, p.m_y);
	}

	glEnd();
	glFlush(); 
}

void drawBezierSurfaceDemo()
{
	initLights();

	vector<CP_Vector3D> controlPoints[4];
	controlPoints[0].push_back( CP_Vector3D(-1.5, -1.5, 4.0));
	controlPoints[0].push_back( CP_Vector3D(-0.5, -1.5, 2.0));
	controlPoints[0].push_back( CP_Vector3D(0.5, -1.5, -1.0));
	controlPoints[0].push_back( CP_Vector3D(1.5, -1.5, 2.0));
	
	controlPoints[1].push_back(CP_Vector3D(-1.5, -0.5, 1.0));
	controlPoints[1].push_back(CP_Vector3D(-0.5, -0.5, 3.0));
	controlPoints[1].push_back(CP_Vector3D(0.5, -0.5,  0.0));
	controlPoints[1].push_back(CP_Vector3D(1.5, -0.5, -1.0));

	controlPoints[2].push_back(CP_Vector3D(-1.5, 0.5, 4.0));
	controlPoints[2].push_back(CP_Vector3D(-0.5, 0.5, 0.0));
	controlPoints[2].push_back(CP_Vector3D(0.5, 0.5, 3.0));
	controlPoints[2].push_back(CP_Vector3D(1.5, 0.5, 4.0));
	
	controlPoints[3].push_back(CP_Vector3D(-1.5, 1.5, -2.0));
	controlPoints[3].push_back(CP_Vector3D(-0.5, 1.5, -2.0));
	controlPoints[3].push_back(CP_Vector3D(0.5, 1.5, 0.0));
	controlPoints[3].push_back(CP_Vector3D(1.5, 1.5, -1.0));

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);     // 设置深度方程
	glLoadIdentity();
	glRotated(65,1,1,1);
	glColor3d(1.0, 0, 0);
	//把控制定点画出
	unsigned int i=0, j=0;
	for(i=0;i<4;i++) {      // 绘制水平线
		glBegin(GL_LINE_STRIP);
		for(j=0;j<4;j++)
			glVertex3d(controlPoints[i][j].m_x, controlPoints[i][j].m_y, controlPoints[i][j].m_z);
		glEnd();
	}
	
	for(i=0;i<4;i++) {      // 绘制垂直线
		glBegin(GL_LINE_STRIP);
		for(j=0;j<4;j++)
			glVertex3d(controlPoints[j][i].m_x, controlPoints[j][i].m_y, controlPoints[j][i].m_z);
		glEnd();
	}
	
	double t = 0;

	glColor3d(0.5, 0.3, 0.1);
	//besierSegment = 30;
	//const int uNum = 30, vNum = 30;
	besierSegment = 50;
	const int uNum = 50, vNum = 50;
	CP_Vector3D bezierPoints [uNum][vNum];

	for (unsigned int u = 0; u < uNum; u++)
	{
		t = u * 1.0 / besierSegment;
		vector<CP_Vector3D> newControl;
		for (unsigned int k = 0; k < 4; k++)
		{
			CP_Vector3D p = getBezierPointNotRecurrent(controlPoints[k], t);
			newControl.push_back(p);
		}
		
		glBegin(GL_LINE_STRIP);
		for (unsigned int v = 0; v < vNum; v++)
		{
			t = v * 1.0 / besierSegment;
			CP_Vector3D p = getBezierPointNotRecurrent(newControl, t);
			glVertex3d(p.m_x / 1.0, p.m_y / 1.0, p.m_z / 1.0);
			bezierPoints[u][v] = p;
		}
		glEnd();
	}
	
	glColor3d(0.5, 0.2, 0.6);
	glLoadIdentity();
	glTranslated(-3.5, 0, 0);
	glRotated(65,1,1,1);
	
	//随机产生颜色
	srand(time(NULL));
	for (unsigned u = 0; u < uNum -1; u++)
	{
		for(unsigned v = 0; v < vNum - 1; v++ )
		{  
			double c = rand() % 100 /100.0;
			//srand(u+c);
			double cc = rand() % 100 /100.0;
			//srand(cc-u);
			double ccc = rand() % 100 /100.0;
			glColor3d(ccc, 1-c, cc);
			bool quads = false; 
			if(quads){
				glBegin(GL_QUADS);
				CP_Vector3D p = bezierPoints[u][v];
				glVertex3d(p.m_x / 1.0, p.m_y / 1.0, p.m_z / 1.0);
				p = bezierPoints[u+1][v];
				glVertex3d(p.m_x / 1.0, p.m_y / 1.0, p.m_z / 1.0);
				p = bezierPoints[u+1][v+1];
				glVertex3d(p.m_x / 1.0, p.m_y / 1.0, p.m_z / 1.0);
				p = bezierPoints[u][v+1];
				glVertex3d(p.m_x / 1.0, p.m_y / 1.0, p.m_z / 1.0);
				glEnd();
			}else{
				//三角形
				CP_Vector3D p0 = bezierPoints[u][v];
				CP_Vector3D p1 = bezierPoints[u+1][v];
				CP_Vector3D p2 = bezierPoints[u+1][v+1];
				CP_Vector3D p3 = bezierPoints[u][v+1];
				
				CP_Vector3D t_p = (p1 - p0) ^ (p2 - p0);
				t_p.mf_normalize();

				glBegin(GL_TRIANGLES);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p0.m_x, p0.m_y, p0.m_z);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p1.m_x, p1.m_y, p1.m_z);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p2.m_x, p2.m_y, p2.m_z);
				glEnd();
				
				glColor3d(1-ccc, c, 1-cc);

				t_p = (p3 - p2) ^ (p0 - p2);
				t_p.mf_normalize();
				glBegin(GL_TRIANGLES);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p2.m_x, p2.m_y, p2.m_z);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p3.m_x, p3.m_y, p3.m_z);
				glNormal3d(t_p.m_x, t_p.m_y, t_p.m_z);
				glVertex3d(p0.m_x, p0.m_y, p0.m_z);
				glEnd();
			}
		}
	}
	glFlush();

}
void initLights()
{
    // *************************************************
    // Setting lights and materials begins
    // 设置光源与材料
    GLfloat material_ambient  [ ] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat material_diffuse  [ ] = { 0.2f, 0.8f, 0.4f, 0.8f };
    GLfloat material_specular [ ] = { 0.2f, 0.8f, 0.4f, 0.8f };
    GLfloat material_emission [ ] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat material_shininess[ ] = { 20.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,  material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, material_emission);
    glMaterialfv(GL_FRONT, GL_SHININESS,material_shininess);
		
    GLfloat light_position [ ] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_ambient  [ ] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat light_diffuse  [ ] = { 0.5f, 0.5f, 0.5f, 0.2f };
    GLfloat light_specular [ ] = { 0.5f, 0.5f, 0.5f, 0.2f };
    glLightfv(GL_LIGHT0, GL_POSITION,  light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,   light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,   light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light_specular);

    glEnable (GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable (GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc (GL_LESS);
    // Setting lights and materials ends
    // ****************************************************

}
void drawSphere()
{

	initLights();

    glLoadIdentity();
    CP_Sphere a;
    glTranslated(-5.0, 0.0, 0.0);
    glColor3f(1.0, 0.0, 0.0); // 红色
    a.mf_drawSolid(false);
    glTranslated(5.0, 0.0, 0.0);
    a.mf_drawSolid(true);
    glTranslated(5.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 1.0); // 蓝色
    a.mf_drawWireframe( );
}
void manualDrawBezier()
{
	if(isReady)
	{
		glColor3f(1.0, 0.0, 0.0);
		//bazier
		glBegin(GL_LINE_STRIP);
		double t = 0;
		for (unsigned int i = 0; i < besierSegment; i++)
		{
			t += 1.0/besierSegment;
			//CP_Vector2D p = getBezierPoint(ctrlPoints, t, ctrlPoints.size()-1, ctrlPoints.size()-1 );
			CP_Vector2D p = getBezierPointNotRecurrent(ctrlPoints, t);	
			//CString msg;
			//msg.Format(L"x=%f,y=%f", p.m_x, p.m_y);
			//MessageBox(_T(" " + msg));
			glVertex2d(p.m_x, p.m_y);
		}
		glEnd();
		glFlush();
	}
	//else //control points also show
	{
		glBegin(GL_LINE_STRIP);
		glColor3f(0.0, 0.0, 1.0);
		for (unsigned int i = 0; i < ctrlPoints.size(); i++)
		{	
			glVertex2d(ctrlPoints[i].m_x, ctrlPoints[i].m_y);
		}
		glEnd();
		glFlush();

	}
}

void COpenGLDemoView::OnDraw(CDC* pDC)
{
	COpenGLDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    wglMakeCurrent(pDC->m_hDC, m_hRC);
    
	glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//drawBizerSample();
	drawBezierSurfaceDemo();
	//drawSphere();
	glPopMatrix();


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

void COpenGLDemoView::OnRButtonUp(UINT nflags, CPoint point)
{

	if (enableMaxCtrlPoint && ctrlPoints.size() > MAXCTRPOINT){
		MessageBox(L"递归算法比较垃圾，控制点多了，难得等哦……就不再加了哈");
	}else
	{
		CRect rect;
		GetClientRect(&rect);
		int x = point.x;
		int y = rect.Height() - point.y;

		CP_Vector2D p = CP_Vector2D(x, y);
		ctrlPoints.push_back(p);
	}
	isReady = true;
	Invalidate(TRUE);
	CView::OnRButtonUp(nflags, point);
}
/*
void COpenGLDemoView::OnContextMenu(CWnd*   pWnd  , CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
*/

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

	//MessageBox(L"鼠标左键选择控制点位置\r\n右键生成画bezier曲线\r\n双击左键清空控制点");
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
	//TODO how?
	//delete vector<CP_Vector2D *> ctrlPoints;

}


void COpenGLDemoView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	CClientDC dc(this);
	wglMakeCurrent(dc.m_hDC, m_hRC);

	glViewport(0, 0, (GLsizei) cx, (GLsizei) cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (cx <= cy)
		  glOrtho(-4.0, 4.0, -4.0*(GLfloat)cy/(GLfloat)cx,
				  4.0*(GLfloat)cy/(GLfloat)cx, -4.0, 4.0);
	else
		  glOrtho(-4.0*(GLfloat)cx/(GLfloat)cy,
			      4.0*(GLfloat)cx/(GLfloat)cy, -4.0, 4.0, -4.0, 4.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	wglMakeCurrent(NULL, NULL);

}


BOOL COpenGLDemoView::OnEraseBkgnd(CDC* pDC)
{

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

//本来想写个鼠标移动，实时有个虚线条 把控制点连接起来的
void COpenGLDemoView::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	if(!firstClick)
		return;

	CRect rect;
	GetClientRect(&rect);
	int x = point.x;
	int y = rect.Height() - point.y;

	hoverPoint = CP_Vector2D(x, y);
	*/
	CView::OnMouseMove(nFlags, point);
}

void COpenGLDemoView::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if (enableMaxCtrlPoint && ctrlPoints.size() >= MAXCTRPOINT){
		MessageBox(L"递归算法比较垃圾，控制点多了，难得等哦……就不再加了哈");
		isReady = true;
	}else
	{
		CRect rect;
		GetClientRect(&rect);
		int x = point.x;
		int y = rect.Height() - point.y;

		CP_Vector2D p = CP_Vector2D(x, y);
		ctrlPoints.push_back(p);
		isReady = false;
	}
	Invalidate(TRUE);
	CView::OnLButtonUp(nFlags, point);
}

void COpenGLDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/*
	MessageBox(L"清空控制点，重画");
	ctrlPoints.clear();
	isReady = false;
	Invalidate(FALSE);
	*/
}