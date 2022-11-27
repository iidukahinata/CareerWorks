/**
* @file		Job.cpp
* @brief
*
* @date		2022/09/06 2022年度初版
*/


#include "Job.h"
#include "JobSystem.h"

Job::~Job()
{
	// クライアント側で Remove されていなかった場合
	UnRegisterFromJobSystem();
}

Job::Job(Task&& task, FunctionType type) noexcept
{
	SetFunction(std::forward<Task>(task), type);
}

void Job::Execute(double deletaTime) noexcept
{
	m_task(deletaTime);
}

void Job::SetFunction(Task&& task, FunctionType fincType) noexcept
{
	m_funcType = fincType;
	m_task = std::forward<Task>(task);
}

void Job::RegisterToJobSystem() noexcept
{
	ASSERT(m_task);

	if (!m_isRegistered)
	{
		m_isRegistered = true;
		JobSystem::Get().RegisterJob(this);
	}
}

void Job::UnRegisterFromJobSystem() noexcept
{
	if (m_isRegistered)
	{
		m_isRegistered = false;
		JobSystem::Get().RemoveJob(this);
	}
}