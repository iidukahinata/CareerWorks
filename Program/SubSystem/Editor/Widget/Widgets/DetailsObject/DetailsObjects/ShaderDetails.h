/**
* @file	   ShaderDetails.h
* @brief
*
* @date	   2022/11/24 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class Script;

class ShaderDetails : public DetailsObject
{
	WAIT_FOR_DEVELOPMENT()
public:

	ShaderDetails(DetailsWidget* detailsWidget, ResourceData* resourceData);

	void Draw() override;

private:

	ResourceData* m_resourceData;
};