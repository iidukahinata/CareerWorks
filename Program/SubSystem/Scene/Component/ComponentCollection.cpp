/**
* @file    ComponentCollection.cpp
* @brief
*
* @date	   2022/11/27 2022îNìxèâî≈
*/

#include "ComponentCollection.h"

Unordered_Map<String, std::function<UniquePtr<IComponent>()>> ComponentCollection::m_componentFactorys;

UniquePtr<IComponent> ComponentCollection::CreateComponent(StringView name)
{
	String componentName(name);

	if (!m_componentFactorys.contains(componentName))
	{
		return UniquePtr<IComponent>();
	}
	else
	{
		return m_componentFactorys[componentName]();
	}
}

Vector<StringView> ComponentCollection::GetAllComponentType()
{
	Vector<StringView> componentTypes;

	for (const auto& componentInfo : m_componentFactorys)
	{
		componentTypes.emplace_back(componentInfo.first);
	}

	return componentTypes;
}