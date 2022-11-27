/**
* @file	   GameObjectDetails.h
* @brief
*
* @date	   2022/10/27 2022年度初版
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

	/** コンポーネント追加用 window の表示を行う。*/
	void ShowAddComponentWindow() noexcept;

private:

	// * AddComponent 用フィルター
	ImGuiTextFilter m_filter;

	GameObject* m_gameObject = nullptr;
};