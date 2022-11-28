/**
* @file    Script.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object_items.hpp>
#include "../IScript.h"

class Script : public IScript
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Script, IScript)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnRegister() override;
	void OnUnRegister() override;
	void OnStart() override;
	void OnStop() override;
	void OnRemove() override;

	void Tick(double deltaTime) override;

public:

	void SetScript(ScriptInstance* scriptInstance) override;
	ScriptInstance* GetScript() const override;

private:

	void CallInitFunctions();

private:

	ScriptInstance* m_scriptInstance = nullptr;
};