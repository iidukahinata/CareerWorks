/**
* @file    ComponentFactory.cpp
* @brief
*
* @date	   2022/11/27 2022îNìxèâî≈
*/


#include "ComponentFactory.h"

Unordered_Map<String, std::function<UniquePtr<IComponent>()>> ComponentFactory::m_componentFactorys;

UniquePtr<IComponent> ComponentFactory::Create(GameObject* gameObject, StringView name) noexcept
{
	UniquePtr<IComponent> component;

	if (m_componentFactorys.contains(name))
	{
		component = m_componentFactorys[name]();
	}

    if (component)
    {
        component->m_owner = gameObject;
    }

    return component;
}

Vector<String> ComponentFactory::GetAllComponentType()
{
	Vector<String> componentTypes;

	for (const auto& componentInfo : m_componentFactorys)
	{
		componentTypes.emplace_back(componentInfo.first);
	}

	return componentTypes;
}