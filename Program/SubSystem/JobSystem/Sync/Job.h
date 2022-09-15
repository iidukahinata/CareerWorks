/**
* @file		Job.h
* @brief
*
* @date		2022/09/06 2022年度初版
*/
#pragma once


#include "JobHelper.h"

class Job
{
	COMPLETED_DEVELOPMENT()

	friend class JobSystem;

	typedef std::function<void(double)> Task;
public:

	Job() = default;
	Job(Task&& task, FunctionType type) noexcept;

	~Job();

	void Execute(double deletaTime) noexcept;

	/**
	* JobSystem で実行される関数 function を設定
	* 
	* @note 引数には double 型を受け取る必要があります。
	*/
	void SetFunction(Task&& task, FunctionType fincType) noexcept;

	/** 更新処理を行うためには、一度実行しなければならない。*/
	void RegisterToJobSystem() noexcept;

	/** JobSystem に登録した場合、呼び出す必要があります。*/
	void UnRegisterFromJobSystem() noexcept;

private:

	// * 重複チェック用
	bool m_isRegistered = false;

	// * JobSystem セット後実行させる関数
	Task m_task;

	// * 検索用に所属コンテナのポインタを保持します。
	FunctionType m_funcType = FunctionType::None;
};