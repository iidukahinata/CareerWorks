/**
* @file    Script.h
* @brief
*
* @date	   2022/11/15 2022年度初版
*/
#pragma once

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/object.hpp>
#include "../IComponent.h"

class Script : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Script)
private:

	enum FuncType
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

	void SetScript(StringView name) noexcept;

private:

	/** コンパイルが失敗した場合は null オブジェクトを返す。*/
	boost::python::object CompileScript(StringView name) noexcept;

	/** スクリプトのコンパイル及び、関数の設定を行う。*/
	void SetUpFunctionList() noexcept;

	/** ScriptFile から指定関数の取得を行う。*/
	bool GetFunction(StringView funcName, FuncType type, const boost::python::object& scriptFile) noexcept;

	/** 指定関数が登録されている場合実行する。*/
	void CallFunction(FuncType type) noexcept;

	/** Script エラー処理を出力する。*/
	void ErrorHandle() noexcept;

private:

	String m_scriptName;

	// * 関数の設定の有無を保持
	Array<bool, FuncType::Max> m_hasFunctionList;

	// * 関数オブジェクトリスト
	Array<boost::python::object, FuncType::Max> m_functionList;
};