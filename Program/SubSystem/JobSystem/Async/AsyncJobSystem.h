/**
* @file    AsyncJobSystem.h
* @brief
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


class AsyncJob;

class AsyncJobSystem
{
	COMPLETED_DEVELOPMENT()
	AsyncJobSystem() = default;
	COPY_PROHIBITED(AsyncJobSystem)
public:

	static AsyncJobSystem& Get() noexcept
	{
		static AsyncJobSystem instance;
		return instance;
	}

	~AsyncJobSystem();

	/** 指定個数 thread を処理初期化。*/
	bool Initialize(int threadCount) noexcept;

	/** 並列処理で実行される処理の追加。*/
	void AddTask(AsyncJob* job) noexcept;

	/** CPU最大使用可能スレッド数を返す。*/
	int GetMaxThreadCount() const noexcept;

private:

	/** インスタンスの解放時に実行される。*/
	void Stop() noexcept;

	/** 生成された各 thread が処理する Task 実行を行う関数。*/
	void ThreadLoop() noexcept;

private:

	// * 並列処理中の終了を管理する
	bool m_isRunning;

	// * タスク実行されていない thread 数を保持
	int m_numOfThreadsAvailable;

	// * 並列処理で順に実行される関数
	Queue<AsyncJob*> m_tasks;

	// * ユーザー指定数で thread 数を変更するために可変長配列で管理
	Vector<std::thread> m_threads;

	// * このクラス内での排他制御実現用
	std::mutex m_mutex;

	std::condition_variable m_conditionVar;
};