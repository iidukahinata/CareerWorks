/**
* @file	   RigidBodyDetails.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class IRigidBody;

class RigidBodyDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	RigidBodyDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	IRigidBody* m_rigidBody = nullptr;
};