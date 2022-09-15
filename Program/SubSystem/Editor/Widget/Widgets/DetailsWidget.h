/**
* @file	   DetailsWidget.h
* @brief
*
* @date	   2022/09/09 2022�N�x����
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
	WAIT_FOR_DEVELOPMENT("ResourceWidget �����")
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

	// * �I���I�u�W�F�N�g�̏ڍ׏��
	static Vector<UniquePtr<DetailsObject>> m_detailsObjects;
};