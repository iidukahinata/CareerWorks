/**
* @file		Widget.h
* @brief
*
* @date		2022/10/20 2022îNìxèâî≈
*/
#pragma once


#include "../EditorHelper.h"

class Widget
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(Widget);
public:

	Widget() = default;
	virtual ~Widget() = default;

public:

	virtual void PostInitialize() {}
	virtual void Draw() {}

	Context* GetContext() noexcept;
};