/**
* @file    ScriptInstance.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	/** asset ファイルと独自データの作成を行う。*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

	void Reload() noexcept;

public:

	void RegisterScript(IScript* script) noexcept;
	void UnRegisterScript(IScript* script) noexcept;

	/** 指定関数が登録されている場合のみ実行する。*/
	template<typename... Args>
	void CallFunction(IScript* script, ScriptFuncType type, Args... args) noexcept;

	bool HasTickFunction() const noexcept;

	void RegisterEventFunctionList() noexcept;
	void UnRegisterEventFunctionList() noexcept;
	bool IsRegisterEventFunction() const noexcept;

public:

	/** スクリプトインスタンスの取得 */
	boost::python::object GetInstance(IScript* script) const noexcept;

private:

	/**
	* @param name	   [入力] boost python の仕様により const char* などの型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	* @param classInfo [入力] クラスモジュールの __dict__ から生成した object_attribute を指定する必要あある。
	*/
	template<typename T>
	void InitFunction(const T& name, ScriptFuncType type, const boost::python::object& classInfo);

	/**
	* @param name	   [入力] boost python の仕様により const char* などの型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	* @param classInfo [入力] クラスモジュールの __dict__ から生成した object_attribute を指定する必要あある。
	*/
	template<typename T>
	void InitEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& classInfo);

	template<typename Func, typename Type>
	void SetupEventFunction(ScriptEventFuncType type);

private:

	/** スクリプトの反射情報から関数の設定を行う。*/
	void PostInit() noexcept;

	/** スクリプトのコンパイルを行う。*/
	boost::python::object CompileScript(StringView name) noexcept;

	void CreateClassInstance(IScript* script) noexcept;

	void ErrorHandle() noexcept;

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

	Vector<IScript*> m_scripts;

	bool m_registerEventListener = false;

	Map<ScriptEventFuncType, EventListener> m_eventListenerMap;

	struct InstanceData
	{
		boost::python::object m_instance;

		Map<uint32_t, boost::python::api::object> m_functionMap;

		Map<uint32_t, boost::python::api::object> m_eventFunctionMap;
	};

	// * 使用されているスクリプトクラスのインスタンスを保持
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