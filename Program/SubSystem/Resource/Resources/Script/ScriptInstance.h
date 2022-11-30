/**
* @file    ScriptInstance.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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

	OnCollisionEnter,

	OnCollisionStay,

	OnCollisionExit,

	OnTriggerEnter,

	OnTriggerStay,

	OnTriggerExit,

	MaxFunc,
};

enum ScriptEventFuncType
{
	OnKeyPressed,

	OnKeyReleased,

	OnChangeScene,

	OnLoadSceneComplete,

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

class IScript;

class ScriptInstance : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptInstance, IResource)
public:

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s���B*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

	void Reload() noexcept;

public:

	void RegisterScript(IScript* script) noexcept;
	void UnRegisterScript(IScript* script) noexcept;

	/** �w��֐����o�^����Ă���ꍇ�̂ݎ��s����B*/
	template<typename... Args>
	void CallFunction(IScript* script, ScriptFuncType type, Args... args) noexcept;

	bool HasTickFunction() const noexcept;

	void RegisterEventFunctionList() noexcept;
	void UnRegisterEventFunctionList() noexcept;
	bool IsRegisterEventFunction() const noexcept;

public:

	/** �X�N���v�g�C���X�^���X�̎擾 */
	boost::python::object GetInstance(IScript* script) const noexcept;

private:

	/**
	* @param name	   [����] boost python �̎d�l�ɂ�� const char* �Ȃǂ̌^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	* @param classInfo [����] �N���X���W���[���� __dict__ ���琶������ object_attribute ���w�肷��K�v������B
	*/
	template<typename T>
	void InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo);

	/**
	* @param name	   [����] boost python �̎d�l�ɂ�� const char* �Ȃǂ̌^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	* @param classInfo [����] �N���X���W���[���� __dict__ ���琶������ object_attribute ���w�肷��K�v������B
	*/
	template<typename T>
	void InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo);

	template<typename Func, typename Type>
	void SetupEventFunction(ScriptEventFuncType type);

private:

	/** �X�N���v�g�̔��ˏ�񂩂�֐��̐ݒ���s���B*/
	void PostInit() noexcept;

	/** �X�N���v�g�̃R���p�C�����s���B*/
	boost::python::object CompileScript(StringView name) noexcept;

	void CreateClassInstance(IScript* script) noexcept;

	void ErrorHandle() noexcept;

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

	Vector<IScript*> m_scripts;

	bool m_registerEventListener = false;

	Map<ScriptEventFuncType, EventListener> m_eventListenerMap;

	struct InstanceData
	{
		boost::python::object m_instance;

		Map<uint32_t, boost::python::api::object> m_functionMap;

		Map<uint32_t, boost::python::api::object> m_eventFunctionMap;
	};

	// * �g�p����Ă���X�N���v�g�N���X�̃C���X�^���X��ێ�
	Map<IScript*, InstanceData> m_classInstanceDataMap;
};

template<typename ...Args>
FORCEINLINE void ScriptInstance::CallFunction(IScript* script, ScriptFuncType type, Args ...args) noexcept
{
	if (!m_hasFunctionList[type])
	{
		return;
	}

	try
	{
		auto& instance = m_classInstanceDataMap[script];
		auto& function = instance.m_functionMap[type];

		function(args...);
	}
	catch (const boost::python::error_already_set&)
	{
		ErrorHandle();
	}
}

template<typename T>
FORCEINLINE void ScriptInstance::InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo)
{
	if (classInfo.contains(name))
	{
		m_hasFunctionList[type] = true;
		m_functionNameList[type] = name;
	}
}

template<typename T>
FORCEINLINE void ScriptInstance::InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo)
{
	if (classInfo.contains(name))
	{
		m_hasEventFunctionList[type] = true;
		m_eventFunctionNameList[type] = name;
	}
}

template<typename Func, typename Type>
FORCEINLINE void ScriptInstance::SetupEventFunction(ScriptEventFuncType type)
{
	if (!m_hasEventFunctionList[type])
	{
		return;
	}

	m_eventListenerMap[type].SetFunction([this, type](std::any data) {

		try
		{
			for (auto& instanceData : m_classInstanceDataMap)
			{
				instanceData.second.m_eventFunctionMap[type](std::any_cast<Type>(data));
			}
		}
		catch (const boost::python::error_already_set&)
		{
			ErrorHandle();
		}

	});

	m_eventListenerMap[type].RegisterToEventManager<Func>();
}