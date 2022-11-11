/**
* @file    LoadingThread.cpp
* @brief
*
* @date	   2022/11/01 2022年度初版
*/


#include "LoadingThread.h"
#include "../ThreadManager.h"

std::mutex LoadingCommandList::m_mutex;
Queue<LoadingCommandList::Command> LoadingCommandList::m_commandList;
Queue<LoadingCommandList::Command> LoadingCommandList::m_taskList;

static std::thread::id m_lodingThreadID = {};

bool LoadingCommandList::HasTask() noexcept
{
	return m_taskList.size() != 0;
}

void LoadingCommandList::SwapCommand() noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_commandList.swap(m_taskList);
}

void LoadingCommandList::CommandProcess() noexcept
{
	while (!m_commandList.empty())
	{
		auto&& command = std::move(m_commandList.front());

		// command の実行
		command();

		m_commandList.pop();
	}
}

void LoadingThread::Start() noexcept
{
	// register
	ThreadManager::Get().CreateThread<LoadingThread>();
}

void LoadingThread::Stop() noexcept
{
	// unregister
	ThreadManager::Get().QuitThread(LoadingThread::TypeData.Name);
}

void LoadingThread::Run()
{
	m_lodingThreadID = std::this_thread::get_id();

	while (!IsRequestQuit())
	{
		m_commandList.SwapCommand();

		m_commandList.CommandProcess();

		// タスクが既に登録されていれば直ぐに処理する
		if (m_commandList.HasTask())
		{
			continue;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}

bool LoadingThread::IsRealTimeThread() const
{
	return true;
}

bool IsInLoadingThread() noexcept
{
	return m_lodingThreadID == std::this_thread::get_id();
}