#include "stdafx.h"
#include "CP_Topology.h"

/*
	使用方法：将Validate.h考到项目文件夹内，然后在项目的头文件内添加Validate.h，
		在需要验证Brep正确性的文件中#include "Validate.h"，
		通过调用validate_body(CP_Body& b)来验证Brep是否正确。
	例如：
		cf_bodyCylinderCreate(b, 0.0, 0.0, -h/2, h, 2);
		validate_body(b);
	注意：因为该函数使用了assert()函数，所以该函数只有在Debug模式下才有效。
*/

// 验证body的拓扑是否正确
bool validate_body(CP_Body& b)
{
    bool no_error = true;
    for (int face_index = 0; face_index < b.mf_getFaceNumber(); face_index++)
    {
        CP_Face *face = b.mf_getFace(face_index);
        // 每个面的外环都不为空
        assert(face->mf_getLoopNumber() >= 1);
        for (int loop_index = 0; loop_index < face->mf_getLoopNumber(); loop_index++)
        {
            CP_Loop *loop = face->mf_getLoop(loop_index);
            // 每个环至少有一条coedge
            assert(loop->mf_getCoedgeNumber() >= 1);
            for (int coedge_index = 0; coedge_index < loop->mf_getCoedgeNumber(); coedge_index++)
            {
                CP_Coedge *current_coedge = loop->mf_getCoedge(coedge_index);
                // 每一条edge至少要有两条coedge
                assert(current_coedge->m_edge->mf_getCoedgeNumber() >= 2);
                int next_coedge_index = (coedge_index + 1) % loop->mf_getCoedgeNumber();
                CP_Coedge *next_coedge = loop->mf_getCoedge(next_coedge_index);

                CP_Vertex *current_coedge_end_vertex = current_coedge->m_sameDirection ? current_coedge->m_edge->m_endingVertex : current_coedge->m_edge->m_startingVertex;
                CP_Vertex *next_coedge_start_vertex = next_coedge->m_sameDirection ? next_coedge->m_edge->m_startingVertex : next_coedge->m_edge->m_endingVertex;
                // 每一个loop的coedge要成环
                assert(current_coedge_end_vertex == next_coedge_start_vertex);
                assert((current_coedge->m_curve2D->mf_getEndingPoint() - next_coedge->m_curve2D->mf_getStartingPoint()).mf_getLength() == 0);
            }
        }
    }
    return no_error;
}