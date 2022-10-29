/**
* @file    GameObject.cpp
* @brief
*
* @date	   2022/10/25 2022年度初版
*/


#include "GameObject.h"
#include "World.h"
#include "Factory/ComponentFactory.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"

GameObject::GameObject() : m_transform(this)
{

}

void GameObject::Serialized(FileStream* file) const noexcept
{
	file->Write(m_path);
	m_transform.Serialized(file);

	size_t numComponent = m_components.size();
	file->Write(numComponent);

	for (const auto& componentInfo : m_components)
	{
		const auto& component = componentInfo.second;
		auto componentName = component->GetTypeData().Name;

		file->Write(componentName);
		component->Serialized(file);
	}

	// children
	size_t numChild = m_transform.GetChildCount();
	file->Write(numChild);

	for (const auto& child : m_transform.GetChildren())
	{
		child->GetOwner()->Serialized(file);
	}
}

void GameObject::Deserialized(FileStream* file) noexcept
{
	file->Read(&m_path);
	m_transform.Deserialized(file);

	size_t numComponent;
	file->Read(&numComponent);

	for (int i = 0; i < numComponent; ++i)
	{
		String componentName;
		file->Read(&componentName);

		auto component = AddComponent(componentName);
		component->Deserialized(file);
	}

	// children
	size_t numChild;
	file->Read(&numChild);

	for (int i = 0; i < numChild; ++i)
	{
		auto child = GetWorld()->CreateGameObject(GetOwner());

		child->Deserialized(file);
		child->GetTransform().SetParent(&m_transform);
	}
}

void GameObject::RegisterAllComponents() noexcept
{
	ASSERT(!m_registered);
	m_registered = true;

	for (const auto& component : m_components)
	{
		if (!component.second->IsRegistered())
		{
			component.second->OnRegister();
		}
	}
}

void GameObject::UnRegisterAllComponents() noexcept
{
	ASSERT(m_registered);
	m_registered = false;

	for (const auto& component : m_components)
	{
		if (component.second->IsRegistered())
		{
			component.second->OnUnRegister();
		}
	}
}

void GameObject::BeginPlay() noexcept
{
	if (!m_isPlaying)
	{
		m_isPlaying = true;

		for (const auto& component : m_components)
		{
			if (!component.second->IsBeginPlay())
			{
				component.second->OnStart();
			}
		}
	}
}

void GameObject::EndPlay() noexcept
{
	if (m_isPlaying)
	{
		m_isPlaying = false;

		for (const auto& component : m_components)
		{
			if (component.second->IsBeginPlay())
			{
				component.second->OnStop();
			}
		}
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
	ASSERT(component);

	const auto hash = component->GetTypeData().Hash;
	if (!m_components.contains(hash))
	{
		component->OnInitialize();

		if (m_registered && !component->IsRegistered())
		{
			component->OnRegister();
		}

		if (m_isPlaying && !component->IsBeginPlay())
		{
			component->OnStart();
		}

		m_components.emplace(hash, component);
	}
	else
	{
		LOG_ERROR("既に同じHash値のコンポーネントが存在しています。");
	}
}

void GameObject::RemoveComponent(IComponent* component) noexcept
{
	ASSERT(component);

	const auto hash = component->GetTypeData().Hash;
	if (m_components.contains(hash))
	{
		component->OnRemove();

		m_components.erase(hash);
	}
}

void GameObject::GetAllComponent(Vector<IComponent*>& components) const noexcept
{
	components.resize(m_components.size());

	for (const auto& componentInfo : m_components)
	{
		components.push_back(componentInfo.second.get());
	}
}

const Map<uint32_t, UniquePtr<IComponent>>& GameObject::GetAllComponent() const noexcept
{
	return m_components;
}

IComponent* GameObject::FindComponent(StringView name) const noexcept
{
	const auto type = ComponentType(name);

	if (m_components.contains(type.Hash))
	{
		return m_components.at(type.Hash).get();
	}

	return nullptr;
}

void GameObject::ClearComponets() noexcept
{
	for (const auto& component : m_components)
	{
		component.second->OnRemove();
	}

	m_components.clear();
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