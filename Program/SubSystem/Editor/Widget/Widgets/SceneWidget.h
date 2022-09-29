/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/09/13 2022年度初版
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

	/** 親子関係を考慮した GameObject の表示 */
	void AddGameObjectToTree(GameObject* gameObject) noexcept;

	/** 親子関係の変更、詳細情報の表示など行う */
	void SelectGameObject(GameObject* gameObject) noexcept;

	/** GameObject 生成などのメソッドを行う。*/
	void ShowGameObjectHelper() noexcept;

	/** GameObject 生成時の設定などの指定を行う。*/
	void ShowGameObjectCreateWindow();

	/** 入力値から Window の切り替えなどを行うため。*/
	void ChackClickedCommand() noexcept;

private:

	World* m_world;
};