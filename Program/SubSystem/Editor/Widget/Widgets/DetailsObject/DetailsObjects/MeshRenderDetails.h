/**
* @file	   MeshRenderDetails.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class IMeshRender;

class MeshRenderDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	MeshRenderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	GameObject* m_gameObject = nullptr;

	IMeshRender* m_meshRender = nullptr;
};