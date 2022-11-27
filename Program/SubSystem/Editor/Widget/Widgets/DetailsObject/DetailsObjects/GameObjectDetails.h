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

private:

	// * AddComponent �p�t�B���^�[
	ImGuiTextFilter m_filter;

	GameObject* m_gameObject = nullptr;
};