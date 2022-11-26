/**
* @file    ScriptInstance.h
* @brief
*
* @date	   2022/11/20 2022年度初版
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

	/** asset ファイルと独自データの作成を行う。*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

public:

	void RegisterScript(Script* script) noexcept;
	void UnRegisterScript(Script* script) noexcept;

	/** 指定関数が登録されている場合のみ実行する。*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

	const String& GetScriptName() const noexcept;

private:

	boost::python::object CompileScript(StringView name) noexcept;

	void ErrorHandle() noexcept;

private:

	/**
	* @param name	   [入力] boost python の仕様により const char* などの型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	* @param classInfo [入力] クラスモジュールの __dict__ から生成した object_attribute を指定する必要あある。
	*/
	template<typename T>
	void InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo) noexcept;

	/**
	* @param name	   [入力] boost python の仕様により const char* などの型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	* @param classInfo [入力] クラスモジュールの __dict__ から生成した object_attribute を指定する必要あある。
	*/
	template<typename T>
	void InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo) noexcept;

private:

	/** スクリプトのコンパイル及び、関数の設定を行う。*/
	void PostInit() noexcept;

	void CreateClassInstance(Script* script) noexcept;

	/** スクリプトイベント関数の設定を行う。*/
	void SetUpEventFunctionLisyt() noexcept;

	template<typename Func, typename Type>
	void SetUpEventFunction(ScriptEventFuncType type);

private:

	// * スクリプト名はとクラス名は同等である必要がある。
	String m_scriptName;

	// * スクリプトクラスモジュールを保持
	boost::python::object m_scriptClass;

	// * 関数設定を保持
	Array<bool, ScriptFuncType::MaxFunc> m_hasFunctionList;
	Array<String, ScriptFuncType::MaxFunc> m_functionNameList;

	// * イベント関数設定を保持
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

	// * 使用されているスクリプトクラスのインスタンスを保持
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