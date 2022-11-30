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

	String componentName(name);
	if (m_componentFactorys.contains(componentName))
	{
		component = m_componentFactorys[componentName]();
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