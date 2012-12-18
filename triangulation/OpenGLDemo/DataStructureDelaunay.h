#pragma once

//点链表的结点类
class CNode:public CObject
{
public:
	int index;//点索引号
	int x,y;//x，y坐标值
	//bool out;//内外环
	int NO_in;//内环编号，外环为0，内环从1开始
	bool head;//是否环头
	bool tail;//是否环尾
public:
	CNode()
	{
		index=0;
		x=0;
		y=0;
		//out=TRUE;
		NO_in=0;
		head=FALSE;
		tail=FALSE;
	}//零初始化
	CNode(CNode *temp)
	{
		index=temp->index;
		x=temp->x;
		y=temp->y;
		//out=temp->out;
		NO_in=temp->NO_in;
		head=temp->head;
		tail=temp->tail;
	}
	CNode(CNode &n)
	{
		index=n.index;
		x=n.x;
		y=n.y;
		//out=n.out;
		NO_in=n.NO_in;
		head=n.head;
		tail=n.tail;
	}
	CNode& operator =(CNode n)
	{
		index=n.index;
		x=n.x;
		y=n.y;
		//out=n.out;
		NO_in=n.NO_in;
		head=n.head;
		tail=n.tail;
		return *this;
	}
#ifdef _DEBUG
    void Dump( CDumpContext& dc ) const
    {
        //CObject::Dump( dc );
        dc << index;
    }
#endif
};

//三角形链表的结点类
class CTriange:public CObject
{
public:
	int index;//点索引号
	int L1,L2,L3;//三角形的三个点的索引号
public:
	CTriange()
	{
		index=0;
		L1=0;
		L2=0;
		L3=0;
	}//零初始化
	CTriange(CTriange *temp)
	{
		index=temp->index;
		L1=temp->L1;
		L2=temp->L2;
		L3=temp->L3;
	}
	CTriange(CTriange &t)
	{
		index=t.index;
		L1=t.L1;
		L2=t.L2;
		L3=t.L3;
	}
	CTriange& operator =(CTriange t)
	{
		index=t.index;
		L1=t.L1;
		L2=t.L1;
		L3=t.L3;
		return *this;
	}
#ifdef _DEBUG
    void Dump( CDumpContext& dc ) const
    {
        //CObject::Dump( dc );
        dc << index<<":"<<L1<<","<<L2<<","<<L3;
    }
#endif

};
//边链表的边类
class CEdge:public CObject
{
public:
	int index;//边索引号
	int L1,L2;//边的两个点的索引号
public:
	CEdge()
	{
		index=0;
		L1=0;
		L2=0;
	}//零初始化
	CEdge(CEdge *temp)
	{
		index=temp->index;
		L1=temp->L1;
		L2=temp->L2;
	}
	CEdge(CEdge &t)
	{
		index=t.index;
		L1=t.L1;
		L2=t.L2;
	}
	CEdge& operator =(CEdge t)
	{
		index=t.index;
		L1=t.L1;
		L2=t.L2;
		return *this;
	}
#ifdef _DEBUG
    void Dump( CDumpContext& dc ) const
    {
		//CObject::Dump( dc );
        dc << index<<":"<<L1<<","<<L2;
    }
#endif
};
