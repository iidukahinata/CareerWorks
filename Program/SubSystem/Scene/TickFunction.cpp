/**
* @file    TickFunction.cpp
* @brief
*
* @date	   2022/09/01 2022”N“x‰”Å
*/


#include "TickFunction.h"
#include "TickManager.h"

void TickFunction::RegisterToTickManager() noexcept
{
	if (!IsRegistered())
	{
		m_isRegistered = true;
		TickManager::Get().RegisterTickFunction(this);
	}
}

void TickFunction::UnRegisterFromTickManager() noexcept
{
	if (IsRegistered())
	{
		m_isRegistered = false;
		TickManager::Get().UnRegisterTickFunction(this);
	}
}

bool TickFunction::IsRegistered() const noexcept
{
	return m_isRegistered;
}

void TickFunction::SetEnable(bool enable) noexcept
{
	if (m_isEnabled == enable)
		return;

	if (IsRegistered())
	{
		TickManager::Get().RemoveTickFunction(this);
		m_isEnabled = enable;
		TickManager::Get().AddTickFunction(this);
	}
	else
	{
		m_isEnabled = enable;
	}
}

bool TickFunction::GetEnable() noexcept
{
	return m_isEnabled;
}

void TickFunction::SetPriority(uint32_t priority) noexcept
{
	if (m_priority == priority)
		return;

	if (IsRegistered())
	{
		TickManager::Get().RemoveTickFunction(this);
		m_priority = priority;
		TickManager::Get().AddTickFunction(this);
	}
	else
	{
		m_priority = priority;
	}

	for (auto prerequisite : m_prerequisites)
	{
		prerequisite->SetPriority(priority);
	}
}

uint32_t TickFunction::GetPriority() const noexcept
{
	return m_priority;
}

void TickFunction::SetThreadSafe(bool isThreadSafe) noexcept
{
	m_isThreadSafe = isThreadSafe;
}

bool TickFunction::IsThreadSafe() const noexcept
{
	return m_isThreadSafe;
}

void TickFunction::SetDeletaTime(double deltaTime) noexcept
{
	m_deltaTime = deltaTime;
}

double TickFunction::GetDeletaTime() const noexcept
{
	return m_deltaTime;
}

void TickFunction::AddPrerequisite(TickFunction* tickFunction) noexcept
{
	if (tickFunction->IsRegistered())
	{
		m_prerequisites.emplace_back(tickFunction);
	}
}

void TickFunction::RemovePrerequisite(TickFunction* tickFunction) noexcept
{
	for (auto it = m_prerequisites.begin(); it != m_prerequisites.end(); it++)
	{
		if (*it._Ptr == tickFunction)
		{
			m_prerequisites.erase(it);
		}
	}
}

const Vector<TickFunction*>& TickFunction::GetPrerequisites() const noexcept
{
	return m_prerequisites;
}