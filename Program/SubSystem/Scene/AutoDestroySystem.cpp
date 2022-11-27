/**
* @file    AutoDestroySystem.cpp
* @brief
*
* @date	   2022/11/04 2022îNìxèâî≈
*/


#include "AutoDestroySystem.h"
#include "GameObject.h"

void AutoDestroySystem::AddAutoDestroy(GameObject* gameObject) noexcept
{
	if (!m_gameObjects.contains(gameObject))
	{
		m_gameObjects.emplace(gameObject);
	}
}

void AutoDestroySystem::RemoveAutoDestroy(GameObject* gameObject) noexcept
{
	if (m_gameObjects.contains(gameObject))
	{
		m_gameObjects.erase(gameObject);
	}
}

void AutoDestroySystem::Initialize() noexcept
{
	m_job.SetFunction([this](double) { Tick(); }, FunctionType::StartPhysics);
	m_job.RegisterToJobSystem();
}

void AutoDestroySystem::Tick() noexcept
{
	Vector<GameObject*> destroyObjectList;

	for (const auto& gameObject : m_gameObjects)
	{
		// check if an object can be erased
		if (AllComponentsCanErase(gameObject))
		{
			destroyObjectList.emplace_back(gameObject);
		}
	}

	// delete pointer & detroy check list
	for (const auto& gameObject : destroyObjectList)
	{
		m_gameObjects.erase(gameObject);
		delete gameObject;
	}
}

void AutoDestroySystem::Shutdown() noexcept
{
	m_job.UnRegisterFromJobSystem();
}

bool AutoDestroySystem::AllComponentsCanErase(GameObject* gameObject) const noexcept
{
	for (const auto& component : gameObject->GetAllComponent())
	{
		if (component.second && !component.second->Erasable())
		{
			return false;
		}
	}
	return true;
}