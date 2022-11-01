/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/10/31 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class GameObject;

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

	/** 入力値から Window の切り替えなどを行うため。*/
	void ChackClickedCommand(GameObject* gameObject) noexcept;

	GameObject* CatchDragObject() const noexcept;

	void ShowDragDropHelper() const noexcept;

private:

	World* m_world = nullptr;

	bool m_selectGameObject = false;

	GameObject* m_gameObject = nullptr;
};