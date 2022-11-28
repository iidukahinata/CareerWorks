/**
* @file	   ColliderDetails.h
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class ICollider;

class ColliderDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	ColliderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	ICollider* m_collider = nullptr;
};