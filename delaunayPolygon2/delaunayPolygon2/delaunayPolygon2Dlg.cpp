
// delaunayPolygon2Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "delaunayPolygon2.h"
#include "delaunayPolygon2Dlg.h"
#include "afxdialogex.h"
#include "cmath"
#include "DataStructure.h"
#include "Triangulation.cpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BLACK 1;
#define RED 2;

bool bcandraw=true;//能否绘图
CObList *mpNodeList,*mpTriList,*mpCandidateNode,*mpCandidateNode2,*mpEdgeList;
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CdelaunayPolygon2Dlg 对话框




CdelaunayPolygon2Dlg::CdelaunayPolygon2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdelaunayPolygon2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdelaunayPolygon2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdelaunayPolygon2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OuterEnd, &CdelaunayPolygon2Dlg::OnBnClickedOuterend)
	ON_BN_CLICKED(IDC_InnerEnd, &CdelaunayPolygon2Dlg::OnBnClickedInnerend)
	ON_BN_CLICKED(IDC_PolygonOK, &CdelaunayPolygon2Dlg::OnBnClickedPolygonok)
	ON_BN_CLICKED(IDC_DELAUNAYBegin, &CdelaunayPolygon2Dlg::OnBnClickedDelaunaybegin)
	ON_BN_CLICKED(IDC_REDRAW, &CdelaunayPolygon2Dlg::OnBnClickedRedraw)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CdelaunayPolygon2Dlg 消息处理程序

BOOL CdelaunayPolygon2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    k=0;
    currentnode=0;
    waihuan=neihuan=0;
    curhuan=0;
    currentedge=0;
    end_in=0;
    wai=true;
    mpNodeList=new CObList(sizeof(CNode));
    mpCandidateNode=new CObList(sizeof(CNode));
    mpCandidateNode2=new CObList(sizeof(CNode));
    mpTriList=new CObList(sizeof(CTriange));
    mpEdgeList=new CObList(sizeof(CEdge));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CdelaunayPolygon2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdelaunayPolygon2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

        CClientDC dc(this);
        CBrush br;
        CPen pen;
        POSITION pos;
        CNode node, node0;
        CString str;
        int i=0;

        CDC *pDC=GetDC();
        CBitmap bmp;


        dc.SetBkMode(TRANSPARENT);
        br.CreateSolidBrush(RGB(0,0,0));
        dc.SelectObject(&br);


        for (pos=mpNodeList->GetHeadPosition();pos!=NULL;)
        {
            node=*(CNode*)mpNodeList->GetNext(pos);
            str.Format("%d",node.index);
            dc.Ellipse(node.x-3,node.y-3,node.x+3,node.y+3);
            dc.TextOut(node.x,node.y-20,str);
        }
        br.DeleteObject();

        if (hasGetResult) 
        {
            POSITION pos;
            CTriange * tri; 
            pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
            for (pos=mpTriList->GetHeadPosition();pos!=NULL;)
            {
                tri = (CTriange * ) mpTriList->GetNext(pos);
                dc.SelectObject(&pen);
                node0=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L1-1));
                dc.MoveTo(node0.x,node0.y);
                node=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L2-1));
                dc.LineTo(node.x,node.y);
                node=*(CNode*)mpNodeList->GetAt(mpNodeList->FindIndex(tri->L3-1));
                dc.LineTo(node.x,node.y);
                dc.LineTo(node0.x,node0.y);
                pen.DeleteObject();
            }
        }
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdelaunayPolygon2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CdelaunayPolygon2Dlg::OnBnClickedOuterend()
{
	if (waihuan<3)
	{
		AfxMessageBox("请至少输入3个点!");
		return;
	}
	CNode headnode,beginnode,endnode,temp,temp2;
	POSITION pos;
	CClientDC dc(this);
	CPen pen;
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
				AfxMessageBox("输入的边不能构成多边形，请重绘！");
				GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
				return;
			}
		}
	}
	dc.SetBkMode(TRANSPARENT);
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	dc.SelectObject(&pen);
	pos=mpNodeList->GetHeadPosition();
	headnode=beginnode=*(CNode*)mpNodeList->GetHead();
	while (pos!=NULL)
	{
		endnode=*(CNode*)mpNodeList->GetNext(pos);
		beginnode=endnode;
		dc.MoveTo(beginnode.x,beginnode.y);
	}
	beginnode.tail=true;
	dc.LineTo(headnode.x,headnode.y);
	++currentedge;
	edge.index=currentedge;
	edge.L1=beginnode.index;
	edge.L2=headnode.index;
	mpEdgeList->AddTail(new CEdge(edge));

	pen.DeleteObject();

	wai=FALSE;
	GetDlgItem(IDC_OuterEnd)->EnableWindow(FALSE);
	GetDlgItem(IDC_InnerEnd)->EnableWindow(TRUE);
    GetDlgItem(IDC_PolygonOK)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC)->SetWindowText("请按顺时针顺序输入内环结点");
}


void CdelaunayPolygon2Dlg::OnBnClickedInnerend()
{
	if (end_in >= 0 && end_in < 3)
	{
		AfxMessageBox("请至少输入3个点!");
		return;
	}
	CNode headnode,beginnode,endnode;
	CEdge edge;
	POSITION pos;
	CClientDC dc(this);
	CPen pen;

	dc.SetBkMode(TRANSPARENT);
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	dc.SelectObject(&pen);
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
				AfxMessageBox("输入的边不能构成多边形，请重绘！");
				GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
				return;
			}
		}
	}
	
	dc.MoveTo(endnode.x,endnode.y);

	beginnode.tail=true;
	dc.LineTo(headnode.x,headnode.y);
	++currentedge;
	edge.index=currentedge;
	edge.L1=endnode.index;
	edge.L2=headnode.index;
	mpEdgeList->AddTail(new CEdge(edge));


	pen.DeleteObject();
    end_in=0;
    ++curhuan;
    in_start=TRUE;

    GetDlgItem(IDC_PolygonOK)->EnableWindow(TRUE);
    GetDlgItem(IDC_InnerEnd)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC)->SetWindowText("请按顺时针顺序输入内环结点");
}


void CdelaunayPolygon2Dlg::OnBnClickedPolygonok()
{
	bcandraw=false;
	GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_OuterEnd)->EnableWindow(FALSE);
	GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
	GetDlgItem(IDC_DelaynayBegin)->EnableWindow(TRUE);
}


void CdelaunayPolygon2Dlg::OnBnClickedDelaunaybegin()
{

    Triangulation tri = Triangulation(mpNodeList,  mpEdgeList, currentedge);

    mpTriList = tri.triangulate();

    hasGetResult = true;

    this->Invalidate();

#ifdef _DEBUG
    POSITION pos;
    for (pos=mpTriList->GetHeadPosition();pos!=NULL;)
    {
        afxDump<<"三角剖分结果:"<<mpTriList->GetNext(pos)<<"\n";
    }
#endif

	GetDlgItem(IDC_STATIC)->SetWindowText("请按逆时针顺序输入外多边形结点");
	GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
	GetDlgItem(IDC_DelaynayBegin)->EnableWindow(FALSE);

}


void CdelaunayPolygon2Dlg::OnBnClickedRedraw()
{
	GetDlgItem(IDC_STATIC)->SetWindowText("请按逆时针顺序输入外多边形结点");
	GetDlgItem(IDC_OuterEnd)->EnableWindow(TRUE);
	GetDlgItem(IDC_InnerEnd)->EnableWindow(FALSE);
	GetDlgItem(IDC_PolygonOK)->EnableWindow(FALSE);
	GetDlgItem(IDC_DelaynayBegin)->EnableWindow(FALSE);

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
	mpCandidateNode->RemoveAll();
	mpCandidateNode2->RemoveAll();
	mpTriList->RemoveAll();
	mpEdgeList->RemoveAll();
	this->Invalidate();
}


void CdelaunayPolygon2Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (TRUE==bcandraw)
    {
        CNode temp,beginnode,endnode,temp2;
        CEdge edge;
        CString str;
        CClientDC dc(this);
        CBrush br;
        CPen pen;
        POSITION pos;

        dc.SetBkMode(TRANSPARENT);
        br.CreateSolidBrush(RGB(0,0,0));
        dc.SelectObject(&br);

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
                        AfxMessageBox("边不能相交");
                        return;
                    }
                }
            }

        }
        //画点
        dc.Ellipse(point.x-5,point.y-5,point.x+5,point.y+5);

        currentnode++;
        str.Format("%d",currentnode);

        pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
        dc.SelectObject(&pen);
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
                dc.MoveTo(endnode.x,endnode.y);
                dc.LineTo(point.x,point.y);
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

                dc.MoveTo(endnode.x,endnode.y);
                dc.LineTo(point.x,point.y);
                ++currentedge;
                edge.index=currentedge;
                edge.L1=endnode.index;
                edge.L2=temp.index;
                mpEdgeList->AddTail(new CEdge(edge));
           
            }
            ++end_in; 
        }	
        dc.TextOut(point.x,point.y-20,str);
        br.DeleteObject();
        pen.DeleteObject();
        mpNodeList->AddTail(new CNode(temp));
        ASSERT(&temp!=NULL);
        CDialog::OnLButtonDown(nFlags, point);
    }
}