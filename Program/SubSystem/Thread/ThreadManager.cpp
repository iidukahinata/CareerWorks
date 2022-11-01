/**
* @file    ThreadManager.cpp
* @brief
*
* @date	   2022/11/01 2022年度初版
*/


#include "ThreadManager.h"

void ThreadManager::Tick() noexcept
{
	for (const auto& thread : m_threads)
	{
		if (thread.second->IsRealTimeThread())
			continue;

		m_threadJob[thread.first].RegisterToJobSystem();
	}
}

void ThreadManager::QuitThread(StringView threadName) noexcept
{
	const uint32_t threadId = GetHashFromCRC(threadName);

	if (!m_threads.contains(threadId))
	{
		return;
	}

	// 安全な解放を行うために Thread Loop 終了まで待機
	if (m_threads[threadId]->IsRealTimeThread())
	{
		// 終了申請を行う
		m_threads[threadId]->RequestQuit();

		while (!m_threadJob[threadId].IsTaskFinish())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(8));
		}
	}

	// 解放処理
	UnRegisterThread(threadId);
	RemoveAsyncJob(threadId);
}

void ThreadManager::RegisterThread(IThread* ptr, const uint32_t hash) noexcept
{
	ASSERT(!m_threads.contains(hash));
	m_threads[hash].reset(ptr);
}

void ThreadManager::UnRegisterThread(const uint32_t hash) noexcept
{
	ASSERT(m_threads.contains(hash));
	m_threads.erase(hash);
}

void ThreadManager::RemoveAsyncJob(const uint32_t hash) noexcept
{
	if (m_threadJob.contains(hash))
	{
		m_threadJob.erase(hash);
	}
}