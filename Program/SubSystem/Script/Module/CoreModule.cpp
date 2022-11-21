/**
* @file    CoreModule.cpp
* @brief
*
* @date	   2022/11/22 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Window/Window.h"

void DebugLog(String text)
{
	LOG(text);
};

void DebugLogError(String text)
{
	LOG_ERROR(text);
};

int ScreenWidth()
{
	return Window::Get().GetWindowWidth();
}

int ScreenHeight()
{
	return Window::Get().GetWindowHeight();
}

void SetUpCoreModule()
{
	PY_DEF(DebugLog);
	PY_DEF(DebugLogError);
	
	PY_DEF(ScreenWidth);
	PY_DEF(ScreenHeight);
}