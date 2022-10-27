/**
* @file	   ComponentDetails.h
* @brief
*
* @date	   2022/10/23 2022年度初版
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

	/** コンポーネント用ヘッダー */
	bool ShowComponentHeader(IComponent * component) noexcept;
};