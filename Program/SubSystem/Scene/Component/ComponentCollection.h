/**
* @file    ComponentCollection.h
* @brief
*
* @date	   2022/11/27 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

class ComponentCollection
{
public:

	template<class Key, class T>
	static void Register();

	template<class Key>
	static void UnRegister();

public:

	static UniquePtr<IComponent> CreateComponent(StringView name);

	static Vector<StringView> GetAllComponentType();

private:

	static Unordered_Map<String, std::function<UniquePtr<IComponent>()>> m_componentFactorys;
};

template<class Key, class T>
FORCEINLINE void ComponentCollection::Register()
{
	auto name = Key::TypeData.Name;
	if (!m_componentFactorys.contains(name.data()))
	{
		m_componentFactorys[name.data()] = []() { return std::make_unique<T>(); };
	}
}

template<class Key>
FORCEINLINE void ComponentCollection::UnRegister()
{
	auto name = Key::TypeData.Name;
	if (m_componentFactorys.contains(name))
	{
		m_componentFactorys.erase(name);
	}
}