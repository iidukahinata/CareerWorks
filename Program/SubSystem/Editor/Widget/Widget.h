/**
* @file		Widget.h
* @brief
*
* @date		2022/09/07 2022îNìxèâî≈
*/
#pragma once


#include "../EditorHelper.h"

class Widget
{
	COMPLETED_DEVELOPMENT()
public:

	virtual ~Widget() {}

	virtual void PostInitialize() {}
	virtual void Draw() {}

	Context* GetContext() noexcept;

private:

	bool m_isRegistered = false;
};