/**
* @file	   MeshRenderDetails.h
* @brief
*
* @date	   2022/11/04 2022îNìxèâî≈
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

	GameObject* m_gameObject = nullptr;

	MeshRender* m_meshRender = nullptr;
};