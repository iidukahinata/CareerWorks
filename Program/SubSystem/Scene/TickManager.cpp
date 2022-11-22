/**
* @file    TickManager.cpp
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/


#include "TickManager.h"
#include "TickFunction.h"

void TickManager::Initialize() noexcept
{
	m_hEvent = CreateEvent(nullptr, false, false, "End Tick");
	ASSERT(m_hEvent);

	m_job.SetFunction([this](double deltaTime) { Tick(deltaTime); }, FunctionType::PrePhysics);

#if !IS_EDITOR
	m_job.RegisterToJobSystem();
#endif // IS_EDITOR

	// �I�[�o�[�w�b�h���Ԃ̌����̂��� Task ���ꉻ
	m_anyThreadTask.SetFunction([this]()
	{
#ifdef IS_EDITOR
			if (ImTimeLine::ShowTimeLine()) {
				TIME_LINE_WATCH_START(TaskThread, "Component Update");
			}
#endif // IS_EDITOR

		for (const auto& task : m_anyThreadTaskList)
		{
			task->Tick(task->GetDeletaTime());
		}

		SetEvent(m_hEvent);

#ifdef IS_EDITOR
		if (ImTimeLine::ShowTimeLine()) {
			TIME_LINE_WATCH_END(TaskThread);
		}
#endif // IS_EDITOR
	});
}

void TickManager::Exit() noexcept
{
	m_job.UnRegisterFromJobSystem();

	CloseHandle(m_hEvent);
}

void TickManager::Start() noexcept
{
	m_job.RegisterToJobSystem();
}

void TickManager::Stop() noexcept
{
	m_job.UnRegisterFromJobSystem();
}

void TickManager::Tick(double deltaTime) noexcept
{
#ifdef IS_EDITOR
	if (ImTimeLine::ShowTimeLine()) {
		TIME_LINE_WATCH_START(MainThread, "Component Update");
	}
#endif // IS_EDITOR

	CreateTaskList(deltaTime);

	ResetEvent(m_hEvent);

	// execution task
	{
		m_anyThreadTask.RegisterToJobSystem();

		for (const auto& task : m_gameThreadTaskList)
		{
			task->Tick(task->GetDeletaTime());
		}
	}

	WaitForTask();

#ifdef IS_EDITOR
	if (ImTimeLine::ShowTimeLine()) {
		TIME_LINE_WATCH_END(MainThread);
	}
#endif // IS_EDITOR
}

void TickManager::RegisterTickFunction(TickFunction* function) noexcept
{
	ASSERT(function);
	ASSERT(!m_allTickFunctions.contains(function));

	AddTickFunction(function);
	m_allTickFunctions.insert(function);
}

void TickManager::UnRegisterTickFunction(TickFunction* function) noexcept
{
	ASSERT(function);
	ASSERT(m_allTickFunctions.contains(function));

	RemoveTickFunction(function);
	m_allTickFunctions.erase(function);
}

void TickManager::AddTickFunction(TickFunction* function) noexcept
{
	ASSERT(function);

	if (function->GetEnable())
	{
		auto priority = function->GetPriority();
		m_tickContainers[priority].insert(function);
	}
}

void TickManager::RemoveTickFunction(TickFunction* function) noexcept
{
	if (function->GetEnable())
	{
		auto priority = function->GetPriority();
		if (m_tickContainers.contains(priority))
		{
			m_tickContainers[priority].erase(function);
		}
	}
}

bool TickManager::HasTickFunction(TickFunction* function) noexcept
{
	return m_allTickFunctions.contains(function);
}

void TickManager::WaitForTask() noexcept
{
	if (WaitForSingleObject(m_hEvent, INFINITE) == WAIT_OBJECT_0)
	{
		// ���t���[���쐬�f�[�^����
		m_anyThreadTaskList.clear();
		m_gameThreadTaskList.clear();
	}
}

void TickManager::CreateTaskList(double deltaTime) noexcept
{
	// �O��������l���������������쐬���邽�߂ɕK�v
	Set<TickFunction*> registeredList;

	// �[���D��T���őO�������ǉ�
	for (const auto& container : m_tickContainers)
	{
		for (const auto& function : container.second)
		{
			// �O��������l�������ۂɊ��ɓo�^����Ă���ꍇ�����邽��
			if (registeredList.contains(function))
			{
				continue;
			}

			RegisterToTaskList(function, deltaTime, registeredList);
		}
	}
}

void TickManager::RegisterToTaskList(TickFunction* function, double deltaTime, Set<TickFunction*>& registeredList) noexcept
{
	ASSERT(function);

	registeredList.insert(function);

	// �O��������ɏ�����ǉ����邽�� DFS �������g�p
	for (const auto& prerequisite : function->GetPrerequisites())
	{
		if (!registeredList.contains(prerequisite))
		{
			RegisterToTaskList(prerequisite, deltaTime, registeredList);
		}
	}

	// register list
	if (function->IsThreadSafe())
	{
		function->SetDeletaTime(deltaTime);
		m_anyThreadTaskList.emplace_back(function);
	}
	else
	{
		function->SetDeletaTime(deltaTime);
		m_gameThreadTaskList.emplace_back(function);
	}
}