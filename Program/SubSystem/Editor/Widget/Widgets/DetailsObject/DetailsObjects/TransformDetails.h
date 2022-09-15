/**
* @file	   TransformDetails.h
* @brief
*
* @date	   2022/09/13 2022”N“x‰”Å
*/
#pragma once


#include "../DetailsObject.h"

class Transform;

class TransformDetails : public DetailsObject
{
public:

	TransformDetails(DetailsWidget* detailsWidget, Transform* transform);
	void Draw() override;

private:

	Transform* m_transform;
};