#include "stdafx.h"
#ifndef UTILS
#define UTILS

#include "DataStructureDelaunay.h"
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

class Utils
{
public:
   // double waixin(CNode*,CNode*,CNode*);//求外接圆半径
    //int	Area2(CNode*,CNode*,CNode*);//由三点坐标求三角形面积
    //bool ToLeft(CNode*,CNode*,CNode*);//判断某点是否在一条直线的左侧

     /**
    *	Check if point x lies left to the directed ray rooted at a and passing b
    */
    static bool	ToLeft(CNode* pa,CNode*	pb,CNode* px)
    {
	    int r=Area2(pa, pb, px);
	    return(r<0);
    }

    static bool intersect(CNode* pP1,CNode* pP2,CNode* pP3,CNode* pP4)              //线段求交
    {
	
	    if(pP1->index==pP3->index||pP2->index==pP3->index||pP1->index==pP4->index||pP2->index==pP4->index)
		    return FALSE;
	    else
	    {
		    if((Area2(pP1,pP2,pP3)*Area2(pP1,pP2,pP4)<=0) && (Area2(pP3,pP4,pP1)*Area2(pP3,pP4,pP2)<=0))	
			    return TRUE;
		    else 
			    return FALSE;
	    }
    }

    /**
    *	Get twice the signed area of the triangle determined by a, b, c
    *	Positive if a, b, and c define a CCW turn, and
    *	Negative if a CW turn
    */
    static int	Area2(CNode* pa,CNode* pb,CNode* pc)
    {
	    double r=(
		    pa->x * pb->y - pa->y * pb->x
		    +	pb->x * pc->y - pb->y * pc->x
		    +	pc->x * pa->y - pc->y * pa->x);
	    if (r>0)
	    {
		    return 1;
	    }
	    else if (0==r)
	    {
		    return 0;
	    }
	    else 
	    {
		    return -1;
	    }
    }

    static double waixin(CNode* p1,CNode* p2,CNode* p3)                    //求外接圆半径
    {
        double a,b,c,s;
        a=sqrt((double)((p2->y-p1->y)*(p2->y-p1->y)+(p2->x-p1->x)*(p2->x-p1->x)));
        b=sqrt((double)((p3->y-p2->y)*(p3->y-p2->y)+(p3->x-p2->x)*(p3->x-p2->x)));
        c=sqrt((double)((p1->y-p3->y)*(p1->y-p3->y)+(p1->x-p3->x)*(p1->x-p3->x)));

        s=(a+b+c)/2;
        s=sqrt(s*(s-a)*(s-b)*(s-c));
        double circler=a*b*c/(4*s);
        return circler;
    }

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
	{ 
		std::stringstream ss(s); 
		std::string item; 
		while(std::getline(ss, item, delim)) 
		{ 
			elems.push_back(item); 
		} 
		return elems;
	} 
	static std::vector<std::string> split(const std::string &s, char delim) 
	{ 
		std::vector<std::string> elems; 
		return split(s, delim, elems); 
	} 

};
#endif