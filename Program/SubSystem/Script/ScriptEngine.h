/**
* @file    ScriptEngine.h
* @brief
*
* @date	   2022/11/13 2022�N�x����
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class ScriptEngine : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptEngine)
public:

	bool Initialize();
};