/**
* @file		AsyncJob.cpp
* @brief
*
* @date		2022/08/02 2022年度初版
*/


#include "AsyncJob.h"
#include "AsyncJobSystem.h"

AsyncJob::AsyncJob(Task&& task) noexcept
{
	SetFunction(std::forward<Task>(task));
}

AsyncJob::AsyncJob(const AsyncJob& asyncJob)
{
	m_task = asyncJob.m_task;
}

AsyncJob& AsyncJob::operator=(const AsyncJob& asyncJob)
{
	m_task = asyncJob.m_task;
	return *this;
}

void AsyncJob::Execute() noexcept
{
	// 関数が設定されていない Job が実行されている時、ここで止まります。
	ASSERT(m_task);

	m_task();
	m_finish = true;
}

void AsyncJob::WaitForFinish() noexcept
{
	while (!IsTaskFinish())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void AsyncJob::SetFunction(Task&& task) noexcept
{
	m_finish = true;
	m_task = std::forward<Task>(task);
}

bool AsyncJob::IsTaskFinish() const noexcept
{
	return m_finish.load();
}

void AsyncJob::RegisterToJobSystem()
{
	m_finish = false;
	AsyncJobSystem::Get().AddTask(this);
}