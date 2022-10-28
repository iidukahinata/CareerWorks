/**
* @file	   LightDetails.h
* @brief
*
* @date	   2022/10/27 2022îNìxèâî≈
*/
#pragma once


#include "../ComponentDetails.h"

class Light;

class LightDetails : public ComponentDetails
{
	COMPLETED_DEVELOPMENT()
public:

	LightDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Light* m_light = nullptr;
};