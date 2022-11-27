/**
* @file		Job.cpp
* @brief
*
* @date		2022/09/06 2022�N�x����
*/


#include "Job.h"
#include "JobSystem.h"

Job::~Job()
{
	// �N���C�A���g���� Remove ����Ă��Ȃ������ꍇ
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