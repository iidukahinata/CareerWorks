/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/10/31 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;
class GameObject;
class Model;

class SceneWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

private:

	/** 親子関係を考慮した GameObject の表示 */
	void AddGameObjectToTree(GameObject* gameObject) noexcept;

	/** GameObject 生成などのメソッドを行う。*/
	void ShowGameObjectHelper() noexcept;

	/** GameObject 生成時の設定などの指定を行う。*/
	void ShowCreateWindow() noexcept;

	/** 選択中オブジェクト UI を表示 */
	void ShowSelectUI() const noexcept;

	/** 入力値から Window の切り替えなどを行うため。*/
	void ChackClickedCommand(GameObject* gameObject) noexcept;

	/** モデルデータからゲームオブジェクトを構築 */
	void CreateGameObjectFromModel(Model* model) noexcept;

private:

	/** Drag Drop されたオブジェクト取得の Helper 関数 */
	template<class T>
	T* CatchDragObject() const noexcept;
	GameObject* CatchDragGameObject() const noexcept;
	ResourceData* CatchDragResourceData() const noexcept;
	IResource* CatchDragResourceObject(uint32_t selectType) const noexcept;

private:

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	bool m_selectGameObject = false;

	GameObject* m_gameObject = nullptr;

	Vector<GameObject*> m_allRootGameObjects;

	EventListener m_eventListener;
};

template<class T>
FORCEINLINE T* SceneWidget::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragResourceObject(T::TypeData.Hash));
}