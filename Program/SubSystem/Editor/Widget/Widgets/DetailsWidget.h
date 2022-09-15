/**
* @file	   DetailsWidget.h
* @brief
*
* @date	   2022/09/09 2022年度初版
*/
#pragma once


#include "../Widget.h"

class GameObject;
class ResourceData;
class DetailsObject;

class World;
class ResourceManager;

class DetailsWidget : public Widget
{
	WAIT_FOR_DEVELOPMENT("ResourceWidget 制作後")
public:

	void PostInitialize() override;
	void Draw() override;

	World* GetWorld() const noexcept;
	ResourceManager* GetResourceManager() const noexcept;

public:

	static void SelectGameObject(GameObject* gameObject) noexcept;
	static void SelectResource(ResourceData* resourceData) noexcept;

private:

	World* m_world;
	ResourceManager* m_resourceManager;

	// * this object pointor
	static DetailsWidget* m_detailsWidget;

	// * 選択オブジェクトの詳細情報
	static Vector<UniquePtr<DetailsObject>> m_detailsObjects;
};