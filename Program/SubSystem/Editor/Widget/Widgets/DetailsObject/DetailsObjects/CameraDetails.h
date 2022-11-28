/**
* @file	   CameraDetails.h
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/
#pragma once


#include "../ComponentDetails.h"

class ICamera;

class CameraDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	CameraDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	ICamera* m_camera = nullptr;
};