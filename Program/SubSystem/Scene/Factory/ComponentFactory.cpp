/**
* @file    ComponentFactory.cpp
* @brief
*
* @date	   2022/11/27 2022îNìxèâî≈
*/


#include "ComponentFactory.h"
#include "../Component/ComponentCollection.h"

UniquePtr<IComponent> ComponentFactory::Create(GameObject* gameObject, StringView name) noexcept
{
    UniquePtr<IComponent> component = ComponentCollection::CreateComponent(name);

    if (component)
    {
        component->m_owner = gameObject;
    }

    return component;
}