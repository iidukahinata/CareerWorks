/**
* @file    LoadingThread.h
* @brief
*
* @date	   2022/11/01 2022年度初版
*/
#pragma once


#include "../Thread.h"

/**
* LoadingThread で順次処理される CommandList と CommandList 追加待ち TaskList を管理
* データ競合阻止のため Command の追加時の MainThread でのアクセスブロックによる待ち時間の発生等の問題より二つのキューを用意し回避している。
*/
class LoadingCommandList
{
	COMPLETED_DEVELOPMENT()

	typedef std::function<void()> Command;
public:

	/** GameThread 側からアクセスされる Task 追加関数 */
	static void AddTask(Command&& task) noexcept
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_taskList.push(std::forward<Command>(task));
	}

	/** 登録 Task の消去関数 */
	static void FlushTask() noexcept
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Queue<Command> clearData;
		m_taskList.swap(clearData);
	}

	bool HasTask() noexcept;

	/** 追加待ち TaskList から CommandList へ Command の入れ替え処理 */
	void SwapCommand() noexcept;

	/** 前フレームで追加されたコマンドを実行 */
	void CommandProcess() noexcept;

private:

	// * command の swap 時の排他制御で使用
	static std::mutex m_mutex;

	// * LoadingThread で順次処理される CommandList
	static Queue<Command> m_commandList;

	// * CommandList 追加待ちの TaskList
	static Queue<Command> m_taskList;
};

/**
* 使用者側で LoadingCommand の追加を行うためのインターフェース関数。
*/
template<class Func>
void RegisterLoadingCommand(Func&& task) noexcept
{
	LoadingCommandList::AddTask(std::forward<Func>(task));
}

class LoadingThread : public IThread
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(LoadingThread)
public:

	void Run() override;

	bool IsRealTimeThread() const override;

public:

	/** LoadingThread の立ち上げ処理を行う。*/
	static void Start() noexcept;

	/** LoadingThread の終了処理を行う。*/
	static void Stop() noexcept;

private:

	LoadingCommandList m_commandList;
};

bool IsInLoadingThread() noexcept;