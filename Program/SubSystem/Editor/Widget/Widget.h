/**
* @file		Widget.h
* @brief
*
* @date		2022/10/27 2022îNìxèâî≈
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

	virtual void Serialized(FileStream* file) const {}
	virtual  void Deserialized(FileStream* file) {}

	Context* GetContext() noexcept;
};