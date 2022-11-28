/**
* @file    ScriptEngine.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class ScriptEngine : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptEngine, ISubsystem)
public:

	bool Initialize();

	void RebuildAllScript() noexcept;

private:

	ResourceManager* m_resourceManager = nullptr;
};