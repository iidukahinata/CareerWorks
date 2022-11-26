/**
* @file    Script.h
* @brief
*
* @date	   2022/11/20 2022�N�x����
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

	// * �g�p����Ă���X�N���v�g�N���X��ێ�
	boost::python::object m_classInstance;

	// * �N���X�p�����[�^�[��ێ�
	Unordered_Map<String, boost::python::api::object_item> m_items;
};