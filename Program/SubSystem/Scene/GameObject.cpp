/**
* @file    GameObject.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include "GameObject.h"
#include "World.h"
#include "Factory/ComponentFactory.h"

void GameObject::Serialized(FileStream* file) const
{
	file->Write(m_path);
	m_transform.Serialized(file);

	size_t numComponent = m_components.size();
	file->Write(numComponent);

	for (const auto& componentPair : m_components)
	{
		const auto& component = componentPair.second;
		auto className = component->GetTypeData().Name;

		file->Write(className);
		component->Serialized(file);
	}
}

void GameObject::Deserialization(FileStream* file)
{
	file->Read(&m_path);
	m_transform.Deserialization(file);

	size_t numComponent = 0;
	file->Read(&numComponent);

	for (int i = 0; i < numComponent; ++i)
	{
		String className;
		file->Read(&className);

		auto component = AddComponent(className);
		component->Deserialization(file);
	}
}

void GameObject::StartAllComponents() noexcept
{
	for (const auto& component : m_components)
	{
		component.second->OnStart();
	}
}

void GameObject::StopAllComponents() noexcept
{
	for (const auto& component : m_components)
	{
		component.second->OnStop();
	}
}

IComponent* GameObject::AddComponent(StringView name) noexcept
{
	IComponent* result = nullptr;

	if (auto component = ComponentFactory::Create(this, name))
	{
		result = component.get();
		AddComponent(component.release());
	}

	return result;
}

void GameObject::AddComponent(IComponent* component) noexcept
{
	const auto hash = component->GetTypeData().Hash;

	if (m_components.contains(hash))
	{
		LOG_ERROR("既に同じHash値のコンポーネントが存在しています。");
	}
	else
	{
		component->OnInitialize();
		m_components.emplace(hash, component);
	}
}

void GameObject::RemoveComponent(IComponent* component) noexcept
{
	const auto hash = component->GetTypeData().Hash;
	if (m_components.contains(hash))
	{
		component->OnRemove();
		m_components.erase(hash);
	}
}

const Map<uint32_t, UniquePtr<IComponent>>& GameObject::GetAllComponent() const noexcept
{
	return m_components;
}

IComponent* GameObject::FindComponent(StringView name) noexcept
{
	const ComponentType type(name);
	if (m_components.contains(type.Hash))
	{
		return m_components[type.Hash].get();
	}
	return nullptr;
}

void GameObject::SetActive(bool active) noexcept
{
	for (const auto& component : m_components)
	{
		component.second->SetActive(active);
	}

	m_active = active;
}

bool GameObject::GetActive() const noexcept
{
	return m_active;
}

uint32_t GameObject::GetID() const noexcept
{
	return m_id;
}

void GameObject::SetID(uint32_t id) noexcept
{
	m_id = id;
}

void GameObject::SetName(StringView name) noexcept
{
	m_path = name;
}

const String& GameObject::GetName() const noexcept
{
	return m_path;
}

Transform& GameObject::GetTransform() noexcept
{
	return m_transform;
}

Scene* GameObject::GetOwner() const noexcept
{
	return m_owner;
}

World* GameObject::GetWorld() const noexcept
{
	return m_world;
}

Context* GameObject::GetContext() const noexcept
{
	ASSERT(m_world);
	return m_world->GetContext();
}