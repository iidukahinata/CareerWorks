/**
* @file    IComponent.cpp
* @brief
*
* @date	   2022/10/25 2022”N“x‰”Å
*/


#include "IComponent.h"
#include "../GameObject.h"

void TickComponentFunction::Tick(double deltaTime)
{
	m_component->Tick(deltaTime);
}

IComponent::IComponent()
{
	m_tickFunction.m_component = this;
}

void IComponent::Serialized(FileStream* file) const
{
	file->Write(GetActive());
	file->Write(m_tickFunction.GetPriority());
}

void IComponent::Deserialized(FileStream* file)
{
	bool active;
	uint32_t priority;

	file->Read(&active);
	file->Read(&priority);

	SetActive(active);
	SetTickPriority(priority);
}

void IComponent::OnInitialize()
{

}

void IComponent::OnRegister()
{
	ASSERT(!m_registered);

	m_registered = true;
}

void IComponent::OnUnRegister()
{
	ASSERT(m_registered);

	m_registered = false;
}

void IComponent::OnStart()
{
	ASSERT(m_registered);
	ASSERT(!m_beginPlay);

	m_beginPlay = true;
}

void IComponent::OnStop()
{
	ASSERT(m_registered);
	ASSERT(m_beginPlay);

	m_beginPlay = false;
}

void IComponent::OnRemove()
{
	if (m_beginPlay)
	{
		OnStop();
	}

	if (m_registered)
	{
		OnUnRegister();
	}
}

void IComponent::Tick(double deltaTime)
{

}

void IComponent::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	m_active = active;
	SetTickEnable(active);
}

bool IComponent::GetActive() const noexcept
{
	return m_active;
}

void IComponent::SetTickEnable(bool enable) noexcept
{
	m_tickFunction.SetEnable(enable);
}

void IComponent::SetTickPriority(uint32_t priority) noexcept
{
	m_tickFunction.SetPriority(priority);
}

void IComponent::AddTickPrerequisite(TickFunction* prerequisite) noexcept
{
	m_tickFunction.AddPrerequisite(prerequisite);
}

bool IComponent::IsRegistered() const noexcept
{
	return m_registered;
}

bool IComponent::IsBeginPlay() const noexcept
{
	return m_beginPlay;
}

Transform& IComponent::GetTransform() const noexcept
{
	ASSERT(m_owner);
	return m_owner->GetTransform();
}

GameObject* IComponent::GetOwner() const noexcept
{
	return m_owner;
}

Scene* IComponent::GetScene() const noexcept
{
	ASSERT(m_owner);
	return m_owner->GetOwner();
}

World* IComponent::GetWorld() const noexcept
{
	ASSERT(m_owner);
	return m_owner->GetWorld();
}

Context* IComponent::GetContext() const noexcept
{
	ASSERT(m_owner);
	return m_owner->GetContext();
}