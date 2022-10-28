/**
* @file	   TransformDetails.h
* @brief
*
* @date	   2022/10/27 2022îNìxèâî≈
*/
#pragma once


#include "../DetailsObject.h"

class Transform;

class TransformDetails : public DetailsObject
{
	COMPLETED_DEVELOPMENT()
public:

	TransformDetails(DetailsWidget* detailsWidget, Transform* transform);
	void Draw() override;

private:

	Transform* m_transform = nullptr;
};