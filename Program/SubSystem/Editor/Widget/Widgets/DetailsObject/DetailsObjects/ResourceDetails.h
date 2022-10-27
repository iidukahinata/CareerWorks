/**
* @file	   ResourceDetails.h
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/
#pragma once


#include "../DetailsObject.h"

class ResourceData;

class ResourceDetails : public DetailsObject
{
	COMPLETED_DEVELOPMENT()
public:

	ResourceDetails(DetailsWidget* detailsWidget, ResourceData* resourceData);
	void Draw() override;

private:

	ResourceData* m_resourceData;
};