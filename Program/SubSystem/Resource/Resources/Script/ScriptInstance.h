/**
* @file    ScriptInstance.h
* @brief
*
* @date	   2022/11/20 2022�N�x����
*/
#pragma once


#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object.hpp>
#include "../IResource.h"

enum ScriptFuncType
{
	Init,

	Register,

	UnRegister,

	Start,

	Stop,

	Remove,

	Update,

	MaxFunc,
};

enum ScriptEventFuncType
{
	OnKeyPressed,

	OnKeyReleased,

	OnChangeScene,

	OnLoadSceneComplete,

	OnCollisionEnter,

	OnCollisionStay,

	OnCollisionExit,

	OnTriggerEnter,

	OnTriggerStay,

	OnTriggerExit,

	MaxEventFunc,
};

enum ScriptParamType
{
	Int,

	Float,

	Complex,

	Str,

	Boolean,

	Object,
};

class Script;

class ScriptInstance : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptInstance)
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s���B*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

public:

	void RegisterScript(Script* script) noexcept;
	void UnRegisterScript(Script* script) noexcept;

	/** �w��֐����o�^����Ă���ꍇ�̂ݎ��s����B*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

	const String& GetScriptName() const noexcept;

private:

	boost::python::object CompileScript(StringView name) noexcept;

	void ErrorHandle() noexcept;

private:

	/**
	* @param name	   [����] boost python �̎d�l�ɂ�� const char* �Ȃǂ̌^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	* @param classInfo [����] �N���X���W���[���� __dict__ ���琶������ object_attribute ���w�肷��K�v������B
	*/
	template<typename T>
	void InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo) noexcept;

	/**
	* @param name	   [����] boost python �̎d�l�ɂ�� const char* �Ȃǂ̌^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	* @param classInfo [����] �N���X���W���[���� __dict__ ���琶������ object_attribute ���w�肷��K�v������B
	*/
	template<typename T>
	void InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo) noexcept;

private:

	/** �X�N���v�g�̃R���p�C���y�сA�֐��̐ݒ���s���B*/
	void PostInit() noexcept;

	void CreateClassInstance(Script* script) noexcept;

	/** �X�N���v�g�C�x���g�֐��̐ݒ���s���B*/
	void SetUpEventFunctionLisyt() noexcept;

	template<typename Func, typename Type>
	void SetUpEventFunction(ScriptEventFuncType type);

private:

	// * �X�N���v�g���͂ƃN���X���͓����ł���K�v������B
	String m_scriptName;

	// * �X�N���v�g�N���X���W���[����ێ�
	boost::python::object m_scriptClass;

	// * �֐��ݒ��ێ�
	Array<bool, ScriptFuncType::MaxFunc> m_hasFunctionList;
	Array<String, ScriptFuncType::MaxFunc> m_functionNameList;

	// * �C�x���g�֐��ݒ��ێ�
	Array<bool, ScriptEventFuncType::MaxEventFunc> m_hasEventFunctionList;
	Array<String, ScriptEventFuncType::MaxEventFunc> m_eventFunctionNameList;

private:

	Vector<Script*> m_scripts;

	Map<ScriptEventFuncType, EventListener> m_eventListenerMap;

	struct InstanceData
	{
		boost::python::object m_instance;

		Map<uint32_t, boost::python::api::object> m_functionMap;

		Map<uint32_t, boost::python::api::object> m_eventFunctionMap;
	};

	// * �g�p����Ă���X�N���v�g�N���X�̃C���X�^���X��ێ�
	Map<Script*, InstanceData> m_classInstanceDataMap;
};

template<typename T>
FORCEINLINE void ScriptInstance::InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo) noexcept
{
	if (classInfo.contains(name))
	{
		m_hasFunctionList[type] = true;
		m_functionNameList[type] = name;
	}
}

template<typename T>
FORCEINLINE void ScriptInstance::InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo) noexcept
{
	if (classInfo.contains(name))
	{
		m_hasEventFunctionList[type] = true;
		m_eventFunctionNameList[type] = name;
	}
}

template<typename Func, typename Type>
FORCEINLINE void ScriptInstance::SetUpEventFunction(ScriptEventFuncType type)
{
	if (!m_hasEventFunctionList[type])
	{
		return;
	}

	m_eventListenerMap[type].SetFunction([this, type](std::any data) {

		for (auto& instanceData : m_classInstanceDataMap)
		{
			instanceData.second.m_eventFunctionMap[type](std::any_cast<Type>(data));
		}

	});

	m_eventListenerMap[type].RegisterToEventManager<Func>();
}