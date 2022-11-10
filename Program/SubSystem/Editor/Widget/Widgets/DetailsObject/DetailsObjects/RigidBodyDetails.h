/**
* @file	   RigidBodyDetails.h
* @brief
*
* @date	   2022/11/08 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class RigidBody;

class RigidBodyDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	RigidBodyDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	RigidBody* m_rigidBody = nullptr;
};