// OpenGLDemoView.cpp : COpenGLDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "OpenGLDemo.h"
#endif


#include "OpenGLDemoDoc.h"
#include "OpenGLDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <algorithm>
#include <vector>
#include <math.h>

#include "GL/GLU.H" // 已经包含GL/GL.h
#include <gl/glut.h>

#include "DataStructureDelaunay.h"
#include "Utils.cpp"
#include "TriangulationDelaunay.cpp"

#pragma region Algorithem Delaunay Global var  Begin

bool bcandraw=true;//能否绘图
CObList *mpNodeList,*mpTriList,*mpEdgeList;
//节点链，三角形链，候选节点链，新的候选节点链,边链
//CObList *mpAllNode,*mpAllEdge;//界面上所有的点和边
int currentnode;//总结点数
int waihuan;//外环节点数
int neihuan;//内环节点数
int end_in;//外环节点数，内环节点数，目前已完成处理的内环节点数
int curhuan;//当前环数
bool wai;//是否外环
int currentedge=0;//总边数
int tri_index=0;//三角形索引
bool in_start;//内环开始输入的状态
int N;//当前多边形边数
int k;//当前处理边数

CNode *pL11,*pL12,*pLk1,*pLk2,*pLo1,*pLo2,*pLm1,*pLm2,*pLn1,*pLn2;//各边的始终点
CEdge *pLk,*pL1,*pLo,*pLm,*pLn;//各边

bool hasGetResult = false;


void findedge(int k,CEdge*& pLk)
{
    POSITION pos;
    pos=mpEdgeList->FindIndex(k-1);
    pLk=(CEdge*)mpEdgeList->GetAt(pos);
}
void findnode(CEdge* pedge, CNode*& pbeginnode, CNode*& pendnode)
{
    POSITION pos;
    pos=mpNodeList->FindIndex(pedge->L1-1);
    pbeginnode=(CNode*)mpNodeList->GetAt(pos);
    pos=mpNodeList->FindIndex(pedge->L2-1);
    pendnode=(CNode*)mpNodeList->GetAt(pos);
}
#pragma endregion Algorithem Delaunay Global var  end


using namespace std;



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
    ON_BN_CLICKED(IDC_OuterEnd,OnBtnClickOuterEnd)
    ON_BN_CLICKED(IDC_InnerEnd,OnBtnClickInnerEnd)
    ON_BN_CLICKED(IDC_PolygonOK,OnBtnClickPolygonOK)
    ON_BN_CLICKED(IDC_Begin,OnBtnClickBegin)
    ON_BN_CLICKED(IDC_Redraw,OnBtnClickRedraw)

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
void drawString(int x, int y, char* str)
{
    glColor3d(0.0, 0.0, 0.0);
    int n = strlen(str);  
    //设置要在屏幕上显示字符的起始位置 
    glRasterPos2i(x,y);  
    //逐个显示字符串中的每个字符  
    for (int i = 0; i < n; i++)  
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(str+i)); 
}
void drawString(int x, int y, CString cstr)
{
    char * str = (LPSTR)(LPCTSTR)cstr;
    glColor3d(0.0, 0.0, 0.0);
    int n = strlen(str);  
    //设置要在屏幕上显示字符的起始位置 
    glRasterPos2i(x,y);  
    //逐个显示字符串中的每个字符  
    for (int i = 0; i < n; i++)  
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *(str+i)); 
}
int clientClientRectHeight;
void drawMFCPoint(int x , int y)
{
    int yy = clientClientRectHeight - y;
    glVertex2d(x, yy);
}
void COpenGLDemoView::OnDraw(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    clientClientRectHeight =  rect.Height();

	COpenGLDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
    wglMakeCurrent(pDC->m_hDC, m_hRC);
    
	glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
    drawString(0,0,"tanglei");
#pragma region draw


    POSITION pos;
    CNode node, node0;
    CString str;
    int i=0;

    glBegin(GL_LINE_LOOP);
    int t_index = 0;
    for (pos=mpNodeList->GetHeadPosition();pos!=NULL;)
    {
        node=*(CNode*)mpNodeList->GetNext(pos);
        if (node.NO_in != t_index)
        {
            glEnd();
            glBegin(GL_LINE_LOOP);
            t_index = node.NO_in;
        }
        drawMFCPoint(node.x, node.y);
    }
    glEnd();
    
    CString cst;
    for (pos=mpNodeList->GetHeadPosition();pos!=NULL;)
    {
        node=*(CNode*)mpNodeList->GetNext(pos);
        cst.Format("%d",node.index);
        drawString(node.x, clientClientRectHeight - node.y + 5, cst);
    }
    

    if (hasGetResult) 
    {
        POSITION pos;
        CTriange * tri; 
        glBegin(GL_TRIANGLES);
        srand((unsigned int)time(NULL));
        for (pos=mpTriList->GetHeadPosition();pos!=NULL;)
        {
            
            double a = rand()%100 / 100.0;
            double b = rand()%100 / 100.0;
            double c = rand()%100 / 100.0;
            glColor3d(a, b, c);
            tri = (CTriange * ) mpTriList->GetNext(pos);

            node0=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L1-1));
            drawMFCPoint(node0.x, node0.y);
            node0=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L2-1));
            drawMFCPoint(node0.x, node0.y);
            node0=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L3-1));
            drawMFCPoint(node0.x, node0.y);
        }
        glEnd();
    }
#pragma endregion draw
	
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

	//MessageBox("鼠标左键选择控制点位置\r\n右键生成画bezier曲线\r\n双击左键清空控制点");

    int button_width=85;
    int button_height=25;
    CSize buttonSize(button_width, button_height);
    int initialButtonX = 50;
    int initialButtonY = 5;
    int jianxi = 10;//间隙
    //CRect( POINT point, SIZE size );

    CRect rect_button_OuterEnd(CPoint(initialButtonX, initialButtonY), buttonSize);   //控制按钮大小、位置
    CRect rect_button_InnerEnd(CPoint(initialButtonX + button_width*1+jianxi, initialButtonY), buttonSize); 
    CRect rect_button_PolygonOK(CPoint(initialButtonX + button_width*2+jianxi, initialButtonY), buttonSize); 
    CRect rect_button_Begin(CPoint(initialButtonX + button_width*3+jianxi, initialButtonY), buttonSize); 
    CRect rect_button_Redraw(CPoint(initialButtonX + button_width*4+jianxi, initialButtonY), buttonSize); 

    
    m_button_OuterEnd.Create("OuterEnd",WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button_OuterEnd,this,IDC_OuterEnd);
    m_button_OuterEnd.ShowWindow(SW_SHOWNORMAL);
    
    m_button_InnerEnd.Create("InnerEnd",WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button_InnerEnd,this,IDC_InnerEnd);
    m_button_InnerEnd.ShowWindow(SW_SHOWNORMAL);
    
    m_button_PolygonOK.Create("PolygonOK",WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button_PolygonOK,this,IDC_PolygonOK);
    m_button_PolygonOK.ShowWindow(SW_SHOWNORMAL);

    m_button_Begin.Create("Begin",WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button_Begin,this,IDC_Begin);
    m_button_Begin.ShowWindow(SW_SHOWNORMAL);

    m_button_Redraw.Create("Redraw",WS_CHILD|WS_VISIBLE|WS_BORDER,rect_button_Redraw,this,IDC_Redraw);
    m_button_Redraw.ShowWindow(SW_SHOWNORMAL);


    k=0;
    currentnode=0;
    waihuan=neihuan=0;
    curhuan=0;
    currentedge=0;
    end_in=0;
    wai=true;
    mpNodeList=new CObList(sizeof(CNode));
   
    mpTriList=new CObList(sizeof(CTriange));
    mpEdgeList=new CObList(sizeof(CEdge));

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

	//glOrtho(-cx/n, cx/n, -cy/n, cy/n, -d, d);
    gluOrtho2D(0,cx,0,cy);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, cx, cy);
	wglMakeCurrent(NULL, NULL);

}


BOOL COpenGLDemoView::OnEraseBkgnd(CDC* pDC)
{

	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}

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
	//cordinate mfc --> opengl	
	/*
    {
		CRect rect;
		GetClientRect(&rect);
		int x = point.x;
		int y = rect.Height() - point.y;
        point = CPoint(x, y);
	}
	*/

    if (TRUE==bcandraw)
    {
        CNode temp,beginnode,endnode,temp2;
        CEdge edge;
        CString str;

        POSITION pos;

        if (0!=mpEdgeList->GetCount())
        {
            if (true==wai || 0!=end_in)
            {
                temp=*(CNode*)mpNodeList->GetTail();
                temp2.x=point.x;
                temp2.y=point.y;
                CNode *a,*b;
                for (pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
                {
                    edge=*(CEdge*)mpEdgeList->GetNext(pos);
                    findnode(&edge,a,b);
                    if (Utils::intersect(&temp,&temp2,a,b))
                    {
                        MessageBox("边不能相交");
                        return;
                    }
                }
            }

        }
 

        currentnode++;

        if (true==wai)
        {
            waihuan++;
            temp.index=waihuan;
            temp.x=point.x;
            temp.y=point.y;
            temp.out=TRUE;
            
            if (1==waihuan)
            {
                temp.head=TRUE;
            }

            if (!mpNodeList->IsEmpty())
            {
                endnode=*(CNode*)mpNodeList->GetTail();
                ++currentedge;
                edge.index=currentedge;
                edge.L1=endnode.index;
                edge.L2=temp.index;
                mpEdgeList->AddTail(new CEdge(edge));

            }
        }
        else
        {
            neihuan++;
            temp.index=waihuan+neihuan;
            temp.x=point.x;
            temp.y=point.y;
            temp.out=FALSE;
            temp.NO_in=curhuan;
            if (TRUE==in_start)
            {
                temp.head=true;
                in_start=false;
            }

            if(end_in!=0)
            {
                pos=mpNodeList->GetTailPosition();
                endnode=*(CNode*)mpNodeList->GetAt(pos);

                ++currentedge;
                edge.index=currentedge;
                edge.L1=endnode.index;
                edge.L2=temp.index;
                mpEdgeList->AddTail(new CEdge(edge));

            }
            ++end_in; 
        }	
  
        mpNodeList->AddTail(new CNode(temp));
        Invalidate();
        CView::OnLButtonUp(nFlags, point);
    }
 }

void COpenGLDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/*
	MessageBox("清空控制点，重画");
	ctrlPoints.clear();
	isReady = false;
	Invalidate(FALSE);
	*/
	CView::OnLButtonDblClk(nFlags,point);
}
void COpenGLDemoView::OnBtnClickOuterEnd()
{
    if (waihuan<3)
    {
        MessageBox("请至少输入3个点!");
        return;
    }
    CNode headnode,beginnode,endnode,temp,temp2;
    POSITION pos;

    CEdge edge;

    if (0!=mpEdgeList->GetCount())
    {
        temp=*(CNode*)mpNodeList->GetTail();
        temp2=*(CNode*)mpNodeList->GetHead();
        CNode *a,*b;
        for (pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
        {
            edge=*(CEdge*)mpEdgeList->GetNext(pos);
            findnode(&edge,a,b);
            if (Utils::intersect(&temp,&temp2,a,b))
            {
                MessageBox("输入的边不能构成多边形，请重绘！");
                GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
                return;
            }
        }
    }

    pos=mpNodeList->GetHeadPosition();
    headnode=beginnode=*(CNode*)mpNodeList->GetHead();
    while (pos!=NULL)
    {
        endnode=*(CNode*)mpNodeList->GetNext(pos);
        beginnode=endnode;
    }
    beginnode.tail=true;
  
    ++currentedge;
    edge.index=currentedge;
    edge.L1=beginnode.index;
    edge.L2=headnode.index;
    mpEdgeList->AddTail(new CEdge(edge));


    wai=FALSE;
    curhuan++;
    GetDlgItem(IDC_OuterEnd)->EnableWindow(FALSE);
    GetDlgItem(IDC_InnerEnd)->EnableWindow(TRUE);
    GetDlgItem(IDC_PolygonOK)->EnableWindow(TRUE);
    
}
void COpenGLDemoView::OnBtnClickRedraw()
{

    GetDlgItem(IDC_OuterEnd)->EnableWindow(TRUE);
    GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
    GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
    GetDlgItem(IDC_Begin)->EnableWindow(FALSE);

    bcandraw=true;
    k=0;
    currentnode=0;
    waihuan=neihuan=0;
    curhuan=0;
    currentedge=0;
    end_in=0;
    wai=true;
    in_start=false;
    mpNodeList->RemoveAll();

    mpTriList->RemoveAll();
    mpEdgeList->RemoveAll();
    this->Invalidate();
}
void COpenGLDemoView::OnBtnClickBegin()
{
    TriangulationDelaunay tri = TriangulationDelaunay(mpNodeList,  mpEdgeList, currentedge);

    mpTriList = tri.triangulate();

    hasGetResult = true;


#ifdef _DEBUG
    POSITION pos;
    for (pos=mpTriList->GetHeadPosition();pos!=NULL;)
    {
        afxDump<<"三角剖分结果:"<<mpTriList->GetNext(pos)<<"\n";
    }
#endif
    Invalidate();
    GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
    GetDlgItem(IDC_Begin)->EnableWindow(FALSE);
}
void COpenGLDemoView::OnBtnClickPolygonOK()
{
    bcandraw=false;
    GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
    GetDlgItem(IDC_OuterEnd)->EnableWindow(FALSE);
    GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
    GetDlgItem(IDC_Begin)->EnableWindow(TRUE);
}
void COpenGLDemoView::OnBtnClickInnerEnd()
{
    if (end_in >= 0 && end_in < 3)
    {
        MessageBox("请至少输入3个点!");
        return;
    }
    CNode headnode,beginnode,endnode;
    CEdge edge;
    POSITION pos;
  
    pos=mpNodeList->FindIndex( waihuan+neihuan-end_in);
    headnode=*(CNode*)mpNodeList->GetAt(pos);
    endnode=*(CNode*)mpNodeList->GetTail();
    if (0!=mpEdgeList->GetCount())
    {
        CNode *a,*b;
        for (pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
        {
            edge=*(CEdge*)mpEdgeList->GetNext(pos);
            findnode(&edge,a,b);
            if (Utils::intersect(&endnode,&headnode,a,b))
            {
                MessageBox("输入的边不能构成多边形，请重绘！");
                GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
                return;
            }
        }
    }

   
    beginnode.tail=true;

    ++currentedge;
    edge.index=currentedge;
    edge.L1=endnode.index;
    edge.L2=headnode.index;
    mpEdgeList->AddTail(new CEdge(edge));


    end_in=0;
    ++curhuan;
    in_start=TRUE;

    GetDlgItem(IDC_PolygonOK)->EnableWindow(TRUE);
    GetDlgItem(IDC_InnerEnd)->EnableWindow(TRUE);
}
