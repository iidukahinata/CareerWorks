/**
* @file	   ComponentDetails.h
* @brief
*
* @date	   2022/10/23 2022�N�x����
*/
#pragma once


#include "DetailsObject.h"

class IComponent;

class ComponentDetails : public DetailsObject
{
	COMPLETED_DEVELOPMENT()
public:

	ComponentDetails(DetailsWidget* detailsWidget);
	virtual ~ComponentDetails();

protected:

	/** �R���|�[�l���g�p�w�b�_�[ */
	bool ShowComponentHeader(IComponent * component) noexcept;
};