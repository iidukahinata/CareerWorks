/**
* @file    ThreadManager.h
* @brief
*
* @date	   2022/11/01 2022年度初版
*/
#pragma once


#include "Thread.h"

/**
* IThread クラスは数個(多くて４個)しか派生しないと想定して実装しているため、
* Hash 値の被りを考慮した設計にせず名前変更を推奨している。
*/
class ThreadManager
{
	COMPLETED_DEVELOPMENT()
	ThreadManager() = default;
	COPY_PROHIBITED(ThreadManager)
public:

	static ThreadManager& Get() noexcept
	{
		static ThreadManager instance;
		return instance;
	}

	void Tick() noexcept;

	/** スレッドを作成し、他 Thread 上で Run関数を実行。*/
	template<class T>
	void CreateThread() noexcept;

	/** 指定スレッドの Run 関数が終了するまで待機が入る。*/
	void QuitThread(StringView threadName) noexcept;

private:

	/** thread class メソッド */
	void RegisterThread(IThread* ptr, const uint32_t hash) noexcept;
	void UnRegisterThread(const uint32_t hash) noexcept;

	/** async run job メソッド */
	template<typename Func>
	void AddAsyncJob(const uint32_t hash, Func&& func) noexcept;
	void RemoveAsyncJob(const uint32_t hash) noexcept;

private:

	Map<uint32_t, UniquePtr<IThread>> m_threads;
	Map<uint32_t, AsyncJob> m_threadJob;
};

template<class T>
FORCEINLINE void ThreadManager::CreateThread() noexcept
{
	const uint32_t threadId = T::TypeData.Hash;
	T* newThread = new T();

	RegisterThread(newThread, threadId);
	AddAsyncJob(threadId, [this, newThread]() { newThread->Run(); });

	if (newThread->IsRealTimeThread())
	{
		m_threadJob[threadId].RegisterToJobSystem();
	}
}

template<typename Func>
FORCEINLINE void ThreadManager::AddAsyncJob(const uint32_t hash, Func&& func) noexcept
{
	ASSERT(!m_threadJob.contains(hash));
	m_threadJob.emplace(std::pair<uint32_t, AsyncJob>(hash, func));
}