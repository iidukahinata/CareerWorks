/**
* @file	   MeshRenderDetails.h
* @brief
*
* @date	   2022/11/04 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class MeshRender;

class MeshRenderDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	MeshRenderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	MeshRender* m_meshRender = nullptr;
};