/**
* @file	   GameObjectDetails.h
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/
#pragma once


#include "../DetailsObject.h"

class GameObject;
class IComponent;

class GameObjectDetails : public DetailsObject
{
public:

	GameObjectDetails(DetailsWidget* detailsWidget, GameObject* gameObject);
	void Draw() override;

private:

	void ShowAddComponentWindow() noexcept;

	template<class T>
	void RegisterComponent()
	{
		m_components.emplace_back(std::make_unique<T>());
	}

private:

	Vector<UniquePtr<IComponent>> m_components;

	ImGuiTextFilter m_filter;

	GameObject* m_gameObject;
};