/**
* @file    TickManager.cpp
* @brief
*
* @date	   2022/10/03 2022年度初版
*/


#include "TickManager.h"
#include "TickFunction.h"

void TickManager::Initialize() noexcept
{
	m_hEvent = CreateEvent(nullptr, true, false, nullptr);
	ASSERT(m_hEvent);

	m_job.SetFunction([this](double deltaTime) { Tick(deltaTime); }, FunctionType::Update);
	m_job.RegisterToJobSystem();

	// オーバーヘッド時間の減少のため Task 統一化
	m_anyThreadTask.SetFunction([this]()
	{
		for (auto task : m_anyThreadTaskList)
		{
			task->Tick(task->GetDeletaTime());
		}

		SetEvent(m_hEvent);
	});
}

void TickManager::Exit() noexcept
{
	m_job.UnRegisterFromJobSystem();

	CloseHandle(m_hEvent);
}

void TickManager::Tick(double deltaTime) noexcept
{
	CreateTaskList(deltaTime);

	ResetEvent(m_hEvent);

	// execution task
	{
		m_anyThreadTask.RegisterToJobSystem();

		for (auto task : m_gameThreadTaskList)
		{
			task->Tick(task->GetDeletaTime());
		}
	}

	WaitForTask();
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
		const auto priority = function->GetPriority();
		m_tickContainers[priority].insert(function);
	}
}

void TickManager::RemoveTickFunction(TickFunction* function) noexcept
{
	if (function->GetEnable())
	{
		const auto priority = function->GetPriority();
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
		// 毎フレーム作成データ消去
		m_anyThreadTaskList.clear();
		m_gameThreadTaskList.clear();
	}
}

void TickManager::CreateTaskList(double deltaTime) noexcept
{
	// 前提条件を考慮した処理順を作成するために必要
	Set<TickFunction*> registeredList;

	// 深さ優先探索で前提条件を追加
	for (const auto& container : m_tickContainers)
	{
		for (const auto function : container.second)
		{
			// 前提条件を考慮した際に既に登録されている場合があるため
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

	// 前提条件順に処理を追加するため DFS 方式を使用
	for (auto& prerequisites = function->GetPrerequisites(); auto prerequisite : prerequisites)
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