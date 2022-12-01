/**
* @file    CoreModule.cpp
* @brief
*
* @date	   2022/11/22 2022îNìxèâî≈
*/


#include "ModuleHelper.h"
#include "Subsystem/Window/Window.h"
#include "Subsystem/Core/Context.h"
#include "Subsystem/Resource/ResourceManager.h"

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
	PY_CLASS_NOCOPY(Context)
		.def("GetResource", &Context::GetSubsystem<ResourceManager>, PY_RET_REF);

	PY_DEF(DebugLog);
	PY_DEF(DebugLogError);
	
	PY_DEF(ScreenWidth);
	PY_DEF(ScreenHeight);
}