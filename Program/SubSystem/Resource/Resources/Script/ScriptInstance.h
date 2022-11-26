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

	/** asset ファイルと独自データの作成を行う。*/
	static ScriptInstance* Create(StringView name) noexcept;

	bool Load(StringView path) override;
	void Update() override;

public:

	void RegisterScript(Script* script) noexcept;
	void UnRegisterScript(Script* script) noexcept;

	/** 指定関数が登録されている場合実行する。*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

	const String& GetScriptName() const noexcept;

private:

	/** コンパイルが失敗した場合は null オブジェクトを返す。*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** スクリプトのコンパイル及び、関数の設定を行う。*/
	void SetUpFunctionList() noexcept;

	/** スクリプトイベント関数の設定を行う。*/
	void SetUpEventFunctionLisyt() noexcept;

	/**
	* ScriptFile から指定関数の取得を行う。
	* @param name [入力] boost python の仕様により const char* など型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	*/
	template<typename T>
	bool SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept;

	/**
	* ScriptFile から指定関数の取得を行う。
	* @param name [入力] boost python の仕様により const char* など型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	*/
	template<typename T>
	bool SetEventFunction(const T& name, ScriptEventFuncType type, const boost::python::object& scriptFile) noexcept;

	template<typename Func, typename Type>
	void SetUpEventFunction(ScriptEventFuncType type);

	/** Script エラー処理を出力する。*/
	void ErrorHandle() noexcept;

private:

	// * スクリプトを実行する各スクリプトコンポーネント
	Vector<Script*> m_scripts;

	String m_scriptName;

	boost::python::object m_scriptFile;

	// * 関数の設定の有無を保持
	Array<bool, ScriptFuncType::Max> m_hasFunctionList;

	// * 関数オブジェクトリスト
	Array<boost::python::object, ScriptFuncType::Max> m_functionList;

	// * スクリプトイベント関数を設定したリスナーオブジェクト
	Map<ScriptEventFuncType, EventListener> m_eventListenerList;

	// * イベント関数オブジェクトリスト
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