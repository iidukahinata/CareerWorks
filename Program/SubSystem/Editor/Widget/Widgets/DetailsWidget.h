/**
* @file	   DetailsWidget.h
* @brief
*
* @date	   2022/10/27 2022年度初版
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
	COMPLETED_DEVELOPMENT()
public:

	~DetailsWidget();

	void PostInitialize() override;
	void Draw() override;

	/** 表示 DetailsObject の追加などを行う場合に使用 */
	void RequestUpdate() noexcept;

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

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	// * 表示中オブジェクトの更新要請のため
	static bool m_requestUpdate;

	// * select objects
	static GameObject* m_selectGameObject;
	static ResourceData* m_selectResourceData;

	// * this object pointor
	static DetailsWidget* m_detailsWidget;

	// * 選択オブジェクトの詳細情報
	static Vector<UniquePtr<DetailsObject>> m_detailsObjects;
};