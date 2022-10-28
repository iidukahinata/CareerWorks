/**
* @file	   GameObjectDetails.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../DetailsObject.h"

class GameObject;
class IComponent;

class GameObjectDetails : public DetailsObject
{
	COMPLETED_DEVELOPMENT()
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

	// * AddComponent �ɕ\������R���|�[�l���g�ꗗ
	Vector<UniquePtr<IComponent>> m_components;

	// * AddComponent �p�t�B���^�[
	ImGuiTextFilter m_filter;

	GameObject* m_gameObject = nullptr;
};