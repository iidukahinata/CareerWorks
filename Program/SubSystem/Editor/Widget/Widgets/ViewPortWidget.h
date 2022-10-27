/**
* @file	   ViewPortWidget.h
* @brief
*
* @date	   2022/10/25 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;

class ViewPortWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** State 変更 Button などの表示を行う。*/
	void ShowToolBar() noexcept;

	/** シーン or ゲーム画面の表示を行う。*/
	void ShowViewPort() noexcept;

	/** 各 State Button が使用されているかの判別がしやすい為に関数化 */
	bool ShowStateButton(bool state, std::function<bool()> func) noexcept;

private:

	World* m_world = nullptr;

	// * ゲーム実行中かを保持
	bool m_isPlay = false;

	// * 現在設定がポーズ中かを表す
	bool m_isPouse = false;
};