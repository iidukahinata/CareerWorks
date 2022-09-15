/**
* @file	   LightDetails.h
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class Light;
class IComponent;

class LightDetails : public DetailsObject
{
public:

	LightDetails(DetailsWidget* detailsWidget, IComponent* component);
	void Draw() override;

private:

	Light* m_light;
};