/**
* @file    RenderingThread.h
* @brief
*
* @date	   2022/11/01 2022年度初版
*/
#pragma once


#include "../Thread.h"
#include "RenderCommandFance.h"

/**
* RenderingThread で順次処理される CommandList と CommandList 追加待ち TaskList を管理
* データ競合阻止のため Command の追加時の MainThread でのアクセスブロックによる待ち時間の発生等の問題より二つのキューを用意し回避している。
*/
class RenderCommandList
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

	// * RenderingThread で順次処理される CommandList
	static Queue<Command> m_commandList;

	// * CommandList 追加待ちの TaskList
	static Queue<Command> m_taskList;
};

/**
* 使用者側で RenderingCommand の追加を行うためのインターフェース関数。
*/
template<class Func>
void RegisterRenderCommand(Func&& task) noexcept
{
	RenderCommandList::AddTask(std::forward<Func>(task));
}

class RenderingThread : public IThread
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(RenderingThread)
public:

	void Run() override;

public:

	/** RenderingThread の立ち上げ処理を行う。*/
	static void Start() noexcept;

	/** RenderingThread の終了処理を行う。*/
	static void Stop() noexcept;

	/**
	* GemaThread 開始時に呼び出される。
	* RenderingThread での開始処理のコマンドセットを行う。
	*/
	static void BegineFrame() noexcept;

	/**
	* GemaThread 終了時に呼び出される。
	* RenderingThread での終了処理のコマンドセットを行う。
	*/
	static void EndFrame() noexcept;

	/** RenderingThread で処理されるコマンド処理が終了するまで待機する。*/
	static void WiatForRenderCommand() noexcept;

private:

	RenderCommandList m_renderCommandList;

	static RenderCommandFance m_renderCommandFance;
};

bool IsRenderingThread() noexcept;