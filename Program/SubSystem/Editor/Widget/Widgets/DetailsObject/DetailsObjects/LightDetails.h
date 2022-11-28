/**
* @file	   LightDetails.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../ComponentDetails.h"

class ILight;

class LightDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	LightDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	ILight* m_light = nullptr;
};