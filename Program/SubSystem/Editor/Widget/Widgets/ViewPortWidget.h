/**
* @file	   ViewPortWidget.h
* @brief
*
* @date	   2022/10/31 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class IRenderer;
class ScriptEngine;

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

	/** Scene View 操作 */
	void InputHandle() noexcept;

	/** Editor Mode 時のみギズモ表示 */
	void Show3DGuizmo(const ImVec2& cursorPos, float imageWidth, float imageHeight) noexcept;

	/** 各 State Button が使用されているかの判別がしやすくする処理 */
	bool ShowStateButton(bool state, std::function<bool()> func) noexcept;

	void CancelEvents() noexcept;

private:

	World* m_world = nullptr;

	IRenderer* m_renderer = nullptr;

	ScriptEngine* m_scriptEngine = nullptr;

	// * ゲーム実行中かを保持
	bool m_isPlay = false;

	// * 現在設定がポーズ中かを表す
	bool m_isPouse = false;

	enum class GuizmoMode
	{
		Translate,
		Rotate,
		Scale,
	};

	GuizmoMode m_guizmoMode = GuizmoMode::Translate;
};