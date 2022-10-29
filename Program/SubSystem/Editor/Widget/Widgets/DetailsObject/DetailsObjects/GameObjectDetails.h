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

	/** �R���|�[�l���g�ǉ��p window �̕\�����s���B*/
	void ShowAddComponentWindow() noexcept;

	template<class T>
	void RegisterComponent();

private:

	// * AddComponent �ɕ\������R���|�[�l���g�ꗗ
	Vector<UniquePtr<IComponent>> m_components;

	// * AddComponent �p�t�B���^�[
	ImGuiTextFilter m_filter;

	GameObject* m_gameObject = nullptr;
};

template<class T>
FORCEINLINE void GameObjectDetails::RegisterComponent()
{
	m_components.emplace_back(std::make_unique<T>());
}