/**
* @file    Script.h
* @brief
*
* @date	   2022/11/20 2022年度初版
*/
#pragma once


#include "../IComponent.h"

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object_items.hpp>

class ScriptInstance;

class Script : public IComponent
{
	COMPLETED_DEVELOPMENT()
		SUB_CLASS(Script)
private:

	enum ParamType
	{
		Int,

		Float,

		Complex,

		Str,

		Boolean,

		Object,
	};

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

	void SetScript(ScriptInstance* scriptInstance) noexcept;
	ScriptInstance* GetScript() const noexcept;

	void CallInitFunctions();

private:

	ScriptInstance* m_scriptInstance = nullptr;

	// * 使用されているスクリプトクラスを保持
	boost::python::object m_classInstance;

	// * クラスパラメーターを保持
	Unordered_Map<String, boost::python::api::object_item> m_items;
};