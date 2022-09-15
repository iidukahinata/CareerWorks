/**
* @file		Widget.cpp
* @brief
*
* @date		2022/09/07 2022”N“x‰”Å
*/


#include "Widget.h"
#include "../EditorSystem.h"


extern Context* g_context;

Context* Widget::GetContext() noexcept
{
	return g_context;
}