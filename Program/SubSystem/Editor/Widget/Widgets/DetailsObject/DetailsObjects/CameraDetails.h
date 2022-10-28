/**
* @file	   CameraDetails.h
* @brief
*
* @date	   2022/10/27 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class Camera;

class CameraDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	CameraDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Camera* m_camera = nullptr;
};