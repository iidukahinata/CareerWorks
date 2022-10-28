/**
* @file	   MainMenuBarWidget.h
* @brief
*
* @date	   2022/10/27 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;

class MainMenuBarWidget : public Widget
{
	WAIT_FOR_DEVELOPMENT("View && Assets など未作成")
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** 新しい Scene の生成と名前指定 */
	void ShowNewSceneModal() noexcept;

	/** セーブ先 Scene 名の取得を行う。*/
	void ShowSaveAsModal() noexcept;

	/** フルスクリーン時に ImGui で WindowMenu を表示 */
	void ShowWindowMenu() noexcept;

private:

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;
};