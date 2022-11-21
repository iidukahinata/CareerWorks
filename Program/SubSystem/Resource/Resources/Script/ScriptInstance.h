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

	/** 指定関数が登録されている場合実行する。*/
	void CallFunction(Script* script, ScriptFuncType type) noexcept;
	void CallTickFunction(Script* script, double deltaTime) noexcept;

	bool HasTickFunction() const noexcept;

private:

	/** コンパイルが失敗した場合は null オブジェクトを返す。*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** スクリプトのコンパイル及び、関数の設定を行う。*/
	void SetUpFunctionList() noexcept;

	/** 
	* ScriptFile から指定関数の取得を行う。 
	* @param name [入力] boost python の仕様により const char* など型はエラーが発生。エラーを回避するためには char[5] などの型指定を行う必要がある。
	*/
	template<typename T>
	bool SetFunction(const T& name, ScriptFuncType type, const boost::python::object& scriptFile) noexcept;

	/** Script エラー処理を出力する。*/
	void ErrorHandle() noexcept;

private:

	String m_scriptName;

	boost::python::object m_scriptFile;

	// * 関数の設定の有無を保持
	Array<bool, ScriptFuncType::Max> m_hasFunctionList;

	// * 関数オブジェクトリスト
	Array<boost::python::object, ScriptFuncType::Max> m_functionList;
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