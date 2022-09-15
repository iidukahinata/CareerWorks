/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/
#pragma once


#include "../Widget.h"

class World;
class GameObject;

class SceneWidget : public Widget
{
	IN_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	void AddGameObjectToTree(GameObject* gameObject) noexcept;

	void SelectGameObject(GameObject* gameObject) noexcept;

private:

	World* m_world;
};