/**
* @file    IScript.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

class ScriptInstance;

class IScript : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IScript, IComponent)
public:

	virtual void SetScript(ScriptInstance* scriptInstance)
	{
		ASSERT(0);
	}
	virtual ScriptInstance* GetScript() const
	{
		ASSERT(0); return {};
	}
};