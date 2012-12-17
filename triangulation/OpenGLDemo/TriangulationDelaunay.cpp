#include "stdafx.h"

#include <cmath>
#include "DataStructureDelaunay.h"
#include "Utils.cpp"


class TriangulationDelaunay
{
public:

    CObList *mpNodeList,*mpTriList,*mpCandidateNode,*mpCandidateNode2,*mpEdgeList;
    //节点链，三角形链，候选节点链，新的候选节点链,边链

    int tri_index;
    int N;//当前多边形边数
    int k;//当前处理边数
    
    CNode *pL11,*pL12,*pLk1,*pLk2,*pLo1,*pLo2,*pLm1,*pLm2,*pLn1,*pLn2;//各边的始终点
    CEdge *pLk,*pL1,*pLo,*pLm,*pLn;//各边

    
    TriangulationDelaunay(CObList *p_mpNodeList, CObList *p_mpEdgeList, int n)
    {
        mpNodeList = p_mpNodeList;
        mpEdgeList = p_mpEdgeList;
        N = n;

        mpCandidateNode=new CObList(sizeof(CNode));
        mpCandidateNode2=new CObList(sizeof(CNode));
        mpTriList = new CObList(sizeof(CTriange));
        tri_index = 0;
    }

    /*
    bool isEdge(CNode*,CNode*,int&);//判断某两点是否多边形的边，如果是，是那条边 
    void findnode(CEdge*,CNode*&,CNode*&);//找到某边的始终点
    void findedge(int,CEdge*&);//有索引找到某条边
    */

    bool IsCanExistInCircle(CNode* pL11,CNode* pL12,CNode* pLo2)
    {
	    POSITION pos;
	    CNode *pLcan;
	    bool exist=FALSE;
	    mpCandidateNode2->RemoveAll();
   	    for (pos=mpCandidateNode->GetHeadPosition();pos!=NULL;)
	    {
		    pLcan=(CNode*)mpCandidateNode->GetNext(pos);
		    if (pLcan->index!=pLo2->index)
		    {   
			    if (Utils::ToLeft(pL11,pL12,pLcan)&&Utils::ToLeft(pL12,pLo2,pLcan)&&Utils::ToLeft(pLo2,pL11,pLcan))
			    {
				    mpCandidateNode2->AddTail(new CNode(*pLcan));
				    exist=true;
			    }
		    }
	    }
	    if (0==mpCandidateNode2->GetCount())
	    {
		    exist=false;
	    }
	    else 
		    exist=true;
	    return exist;
    }

    void findLo2(CNode* pL11,CNode* pL12,CNode*& pLo2,CObList *pCandidateList)
    {
	    POSITION pos;
	    CNode* pLcan;
	    double minR=65535;
	    double r=0;

	    for (pos=pCandidateList->GetHeadPosition();pos!=NULL; )
	    {
		    pLcan=(CNode*)pCandidateList->GetNext(pos);
		    findedge(1,pL1);
		    findnode(pL1,pL11,pL12);
		    r=Utils::waixin(pL11,pL12,pLcan);
		    if (r<minR)
		    {
			    minR=r;
			    pLo2=pLcan;
		    }
	    }
    }
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

    bool isEdge(CNode* pL1,CNode* L2,int &M)
    {
	    POSITION pos;
	    CEdge edge;
	    for (pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
	    {
		    edge=*(CEdge*)mpEdgeList->GetNext(pos);
		    if ((edge.L1==pL1->index) && (edge.L2==L2->index) ||(edge.L1==L2->index) && (edge.L2==pL1->index))
		    {
			    M=edge.index;
			    return TRUE;
		    }
	    }
	    return false;
	
    }
   
    void fun1()
    {
        if (3 == N)    
        {
            //CString str;
            //int tot = mpTriList->GetSize();
            //str.Format("剖分结束，共%d个三角形",tot);

            CEdge* pa;
            CTriange tri;
            tri.index=++tri_index;
            POSITION pos=mpEdgeList->GetHeadPosition();
            pa=(CEdge*)mpEdgeList->GetNext(pos);
            tri.L1 = (pa->L1);
            tri.L2 = pa->L2;
            pa=(CEdge*)mpEdgeList->GetNext(pos);
            tri.L3 = pa->L2;

            mpTriList->AddTail(new CTriange(tri));
            return;
        }
        else
        {
            k=1;
            mpCandidateNode->RemoveAll();
            mpCandidateNode2->RemoveAll();
            fun2(k);
        }
    }

    void fun2(int &k)
    {
        ++k;
        if(k>N)
        {
            fun5(k);
            return;
        }
        findedge(1,pL1);

        findnode(pL1,pL11,pL12);
        findedge(k,pLk);
        findnode(pLk,pLk1,pLk2);
        if (((k==N) && pLk2->index==pL11->index)  || k>N)
            fun5(k);  
        else if (Utils::ToLeft(pL11,pL12,pLk2))
            fun3(k);
        else
            fun2(k);
    }

    void fun3(int &k)
    {
        int t;
        CEdge *pLt;
        CNode *pLt1,*pLt2;

        findedge(1,pL1);
        findnode(pL1,pL11,pL12);
        findedge(k,pLk);
        findnode(pLk,pLk1,pLk2);

        for (t=1;t<=N;t++)                  //从第三条边开始检查，除去第1，2，N，k，k+1条边
        {
            if ((t!=k) && (t!=k+1))
            {
                findedge(t,pLt);
                findnode(pLt,pLt1,pLt2);
                if (Utils::intersect(pLt1,pLt2,pL11,pLk2) || Utils::intersect(pLt1,pLt2,pL12,pLk2) )
                {
                    fun2(k);
                    return;
                }
            }
        }
        if (t==N+1)
        {
            fun4(k);
        }
    }
    void fun4(int &k)
    {	
        findedge(k,pLk);
        findnode(pLk,pLk1,pLk2);
        mpCandidateNode->AddTail(new CNode(*pLk2));

        if (k==N)
        {
            fun5(k);
        }
        else 
            fun2(k);
    }
    void fun5(int &k)
    {
        double minR=65535;
        double r=0;
        CEdge *pLn,*pLm;
        CTriange tri;
        CPoint ptVertex[3];
        int M_index=0; 
        int N_index=0; 

        POSITION pos;
  
        findedge(1,pL1);
        findnode(pL1,pL11,pL12);
        findLo2(pL11,pL12,pLo2,mpCandidateNode);
		

        //用VC语言实现任意多边形的Delaunay完全三角剖分算法
		//不仅要选外接圆最小滴，而且还要选择不包含其他候选节点的
        CNode *pLcan;
        while (true==IsCanExistInCircle(pL11,pL12,pLo2))					 
        {
            mpCandidateNode->RemoveAll();
            for(pos=mpCandidateNode2->GetHeadPosition();pos!=NULL;)
            {
                //mpCandidateNode2 store 能够行成三角形的候选列表
                pLcan=(CNode*)mpCandidateNode2->GetNext(pos);
                mpCandidateNode->AddTail(new CNode(*pLcan));
            }
            findLo2(pL11,pL12,pLo2,mpCandidateNode);
        }
  
        tri.index=++tri_index;
        tri.L1=pL11->index;
        tri.L2=pL12->index;
        tri.L3=pLo2->index;
        mpTriList->AddTail(new CTriange(tri));

        if (!isEdge(pL11,pLo2,M_index) && !isEdge(pL12,pLo2,N_index))
        {
            POSITION pre;
            CEdge* pa;
            ++N;
            pLn=new CEdge();

            for(pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
            {
                pre=pos;
                pa=(CEdge*)mpEdgeList->GetNext(pos);
                if(pa->L2==pL1->L1)	break;
            }
            if(!pre) exit(0);

            for(;pos!=NULL;)
            {
                pa=(CEdge*)mpEdgeList->GetNext(pos);
                pa->index++;
            }
            pa=(CEdge*)mpEdgeList->GetAt(pre);
            pLn->index=pa->index+1;
            pLn->L1=pL11->index;
            pLn->L2=pLo2->index;
            mpEdgeList->InsertAfter(pre,new CEdge(*pLn));
            pL1->L1=pLo2->index;
        }
        else if ( isEdge(pL11,pLo2,M_index) && !isEdge(pL12,pLo2,N_index) )
        {
            CEdge* pa;
            int i=1;

            pos=mpEdgeList->FindIndex(M_index-1);
            mpEdgeList->RemoveAt(pos);
            pL1->L1=pLo2->index;

            for(pos=mpEdgeList->GetHeadPosition();pos!=NULL;i++)
            {
                pa=(CEdge*)mpEdgeList->GetNext(pos);
                if(pa->index!=i)	pa->index=i;
            }
            --N;
        }
        else if ( isEdge(pL12,pLo2,M_index) && !isEdge(pL11,pLo2,N_index) )
        {
            CEdge* pa;
            int i=1;

            pL1->L2=pLo2->index;
            findedge(M_index,pLm);
            findedge(N,pLn);
            pos=mpEdgeList->FindIndex(M_index-1);
            mpEdgeList->RemoveAt(pos);
            for(pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
            {
                pa=(CEdge*)mpEdgeList->GetNext(pos);
                pa->index=i++;
            }
            --N;
        }
        else if ( isEdge(pL12,pLo2,M_index) && isEdge(pL11,pLo2,N_index) )
        {
            CObject *pa;

            if(M_index>N_index)	{int mid=N_index;N_index=M_index;M_index=N_index;}

            pos=mpEdgeList->FindIndex(N_index-1);
            pa=mpEdgeList->GetAt(pos);
            mpEdgeList->RemoveAt(pos);
            delete pa;
            pos=mpEdgeList->FindIndex(M_index-1);
            pa=mpEdgeList->GetAt(pos);
            mpEdgeList->RemoveAt(pos);
            delete pa;
            pos=mpEdgeList->FindIndex(1-1);
            pa=mpEdgeList->GetAt(pos);
            mpEdgeList->RemoveAt(pos);
            delete pa;

            int i=1;
            for(pos=mpEdgeList->GetHeadPosition();pos!=NULL;)
                ((CEdge*)mpEdgeList->GetNext(pos))->index=i++;
            N-=3;
        }
        fun1();
    }
    CObList * triangulate()
    {
        //N=n;
        fun1();
        return mpTriList;
    }

	~TriangulationDelaunay()
	{
		/*//在View 中是局部变量，会析构，再在ondraw时，会木有，所以不能remove掉
		mpCandidateNode->RemoveAll();
		mpCandidateNode2->RemoveAll();
		mpEdgeList->RemoveAll();
		mpNodeList->RemoveAll();
		mpTriList->RemoveAll();

		delete mpCandidateNode;
		delete mpCandidateNode2;
		delete mpEdgeList;
		delete mpNodeList;
		delete mpTriList;*/
	}
};

