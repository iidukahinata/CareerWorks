/**
* @file	   ColliderDetails.h
* @brief
*
* @date	   2022/11/08 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class Collider;

class ColliderDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	ColliderDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Collider* m_collider = nullptr;
};