/**
* @file    GameObject.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "GameObject.h"
#include "World.h"
#include "TickFunction.h"
#include "Factory/ComponentFactory.h"
#include "SubSystem/Resource/Resources/Scene/Scene.h"

void TickGameObjectFunction::Tick(double deltaTime)
{
	m_gameObject->Tick(deltaTime);
}

GameObject::GameObject() : m_transform(this)
{
	m_tickFunction.m_gameObject = this;
}

void GameObject::Serialized(FileStream* file) const noexcept
{
	file->Write(m_path);
	m_transform.Serialized(file);

	// component data
	size_t numComponent = m_components.size();
	file->Write(numComponent);

	for (const auto& componentInfo : m_components)
	{
		const auto& component = componentInfo.second;

		auto componentName = component->GetSuperTypeData().Name;
		file->Write(componentName);

		component->Serialized(file);
	}

	// children data
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

	// component data
	size_t numComponent;
	file->Read(&numComponent);

	for (int i = 0; i < numComponent; ++i)
	{
		String componentName;
		file->Read(&componentName);

		if (auto component = AddComponent(componentName))
		{
			component->Deserialized(file);
		}
	}

	// children data
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

	for (const auto& componentInfo : m_components)
	{
		if (!componentInfo.second->IsRegistered())
		{
			componentInfo.second->OnRegister();
		}
	}

	m_tickFunction.RegisterToTickManager();
}

void GameObject::UnRegisterAllComponents() noexcept
{
	ASSERT(m_registered);
	m_registered = false;

	for (const auto& componentInfo : m_components)
	{
		if (componentInfo.second->IsRegistered())
		{
			componentInfo.second->OnUnRegister();
		}
	}

	m_tickFunction.UnRegisterFromTickManager();
}

void GameObject::BeginPlay() noexcept
{
	if (m_isPlaying)
	{
		return;
	}

	m_isPlaying = true;

	for (const auto& componentInfo : m_components)
	{
		if (!componentInfo.second->IsBeginPlay())
		{
			componentInfo.second->OnStart();
		}
	}
}

void GameObject::EndPlay() noexcept
{
	if (!m_isPlaying)
	{
		return;
	}

	m_isPlaying = false;

	for (const auto& componentInfo : m_components)
	{
		if (componentInfo.second->IsBeginPlay())
		{
			componentInfo.second->OnStop();
		}
	}
}

void GameObject::Tick(double deltaTime) noexcept
{
	bool finishTick = true;

	// safe component  erase process.
	// check component remove request processed.
	std::erase_if(m_components, [&finishTick](auto& component) {

		if (component.second->RequestRemove())
		{
			if (component.second->Erasable())
			{
				return true;
			}
			else
			{
				finishTick = false;
			}
		}

		return false;
	});

	if (finishTick)
	{
		m_tickFunction.SetEnable(false);
	}
}

void GameObject::Destroy() noexcept
{
	m_world->DestroyGameObject(this);
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
		LOG_ERROR("already has same component of the  hash value");
	}
	else
	{
		m_components.emplace(hash, component);
		PostAddComponent(component);
	}
}

void GameObject::PostAddComponent(IComponent* component)
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
}

void GameObject::RemoveComponent(IComponent* component) noexcept
{
	ASSERT(component);

	const auto hash = component->GetTypeData().Hash;

	if (m_components.contains(hash))
	{
		component->OnRemove();

		if (component->Erasable())
		{
			m_components.erase(hash);
		}
		else
		{
			m_tickFunction.SetEnable(true);
		}
	}
}

void GameObject::GetAllComponent(Vector<IComponent*>& components) const noexcept
{
	components.reserve(m_components.size());

	for (const auto& componentInfo : m_components)
	{
		if (componentInfo.second->RequestRemove())
		{
			continue;
		}

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
	if (m_registered)
	{
		m_tickFunction.UnRegisterFromTickManager();
	}

	for (const auto& componentInfo : m_components)
	{
		componentInfo.second->OnRemove();
	}

	std::erase_if(m_components, [](const auto& componentInfo) { return componentInfo.second->Erasable(); });
}

void GameObject::SetActive(bool active) noexcept
{
	for (const auto& componentInfo : m_components)
	{
		componentInfo.second->SetActive(active);
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

bool GameObject::RequestAutoDestroy() const noexcept
{
	if (m_components.size() == 0)
	{
		return false;
	}

	for (const auto& componentInfo : m_components)
	{
		if (!componentInfo.second->Erasable())
		{
			return true;
		}
	}
	return false;
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