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

	/** コマンド発行を内部で行っているため呼び出し側での処理は不要 */
	static void SelectGameObject(GameObject* gameObject) noexcept;
	static void SelectResource(ResourceData* resourceData) noexcept;

	/** 空選択やシーン切り替え時に使用するため。*/
	static void ClearSelectObject() noexcept;

private:

	/** Select 関数の呼び出し側でコマンド設定する手間をなくすため */
	static void SelectGameObjectInternal(GameObject* gameObject) noexcept;
	static void SelectResourceInternal(ResourceData* resourceData) noexcept;

private:

	World* m_world;

	ResourceManager* m_resourceManager;

	static GameObject* m_selectGameObject;

	static ResourceData* m_selectResourceData;

	// * this object pointor
	static DetailsWidget* m_detailsWidget;

	// * 選択オブジェクトの詳細情報
	static Vector<UniquePtr<DetailsObject>> m_detailsObjects;
};