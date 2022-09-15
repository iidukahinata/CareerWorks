/**
* @file	   CameraDetails.h
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class Camera;
class IComponent;

class CameraDetails : public DetailsObject
{
public:

	CameraDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Camera* m_camera;
};