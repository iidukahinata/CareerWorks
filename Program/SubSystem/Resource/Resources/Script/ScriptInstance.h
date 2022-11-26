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

	Max,
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

	/** �w��֐����o�^����Ă���ꍇ���s����B*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

	const String& GetScriptName() const noexcept;

private:

	/** �R���p�C�������s�����ꍇ�� null �I�u�W�F�N�g��Ԃ��B*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** �X�N���v�g�̃R���p�C���y�сA�֐��̐ݒ���s���B*/
	void SetUpFunctionList() noexcept;

	/** �X�N���v�g�C�x���g�֐��̐ݒ���s���B*/
	void SetUpEventFunctionLisyt() noexcept;

	/**
	* ScriptFile ����w��֐��̎擾���s���B
	* @param name [����] boost python �̎d�l�ɂ�� const char* �Ȃǌ^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	*/
	template<typename T>
	bool SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept;

	/**
	* ScriptFile ����w��֐��̎擾���s���B
	* @param name [����] boost python �̎d�l�ɂ�� const char* �Ȃǌ^�̓G���[�������B�G���[��������邽�߂ɂ� char[5] �Ȃǂ̌^�w����s���K�v������B
	*/
	template<typename T>
	bool SetEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& scriptFile) noexcept;

	template<typename Func, typename Type>
	void SetUpEventFunction(ScriptEventFuncType type);

	/** Script �G���[�������o�͂���B*/
	void ErrorHandle() noexcept;

private:

	// * �X�N���v�g�����s����e�X�N���v�g�R���|�[�l���g
	Vector<Script*> m_scripts;

	String m_scriptName;

	boost::python::object m_scriptFile;

	// * �֐��̐ݒ�̗L����ێ�
	Array<bool, ScriptFuncType::Max> m_hasFunctionList;

	// * �֐��I�u�W�F�N�g���X�g
	Array<boost::python::object, ScriptFuncType::Max> m_functionList;

	// * �X�N���v�g�C�x���g�֐���ݒ肵�����X�i�[�I�u�W�F�N�g
	Map<ScriptEventFuncType, EventListener> m_eventListenerList;

	// * �C�x���g�֐��I�u�W�F�N�g���X�g
	Map<ScriptEventFuncType, boost::python::object> m_eventFuncList;
};

template<typename T>
FORCEINLINE bool ScriptInstance::SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept
{
	if (!scriptFile.contains(name))
	{
		return false;
	}

	m_hasFunctionList[type] = true;
	m_functionList[type] = scriptFile[name];

	return true;
}

template<typename T>
FORCEINLINE bool ScriptInstance::SetEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& scriptFile) noexcept
{
	if (!scriptFile.contains(name))
	{
		return false;
	}

	m_eventFuncList[type] = scriptFile[name];
	m_eventListenerList[type] = EventListener();

	return true;
}

template<typename Func, typename Type>
FORCEINLINE void ScriptInstance::SetUpEventFunction(ScriptEventFuncType type)
{
	if (m_eventFuncList.contains(type))
	{
		auto& eventFunc		  = m_eventFuncList[type];
		auto& m_eventListener = m_eventListenerList[type];

		m_eventListener.SetFunction([this, type](std::any data) {

			for (auto script : m_scripts)
			{
				m_eventFuncList[type](boost::python::ptr(script), std::any_cast<Type>(data));
			}

		});

		m_eventListener.RegisterToEventManager<Func>();
	}
}