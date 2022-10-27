/**
* @file    EditorSystem.h
* @brief
*
* @date	   2022/10/21 2022年度初版
*/
#pragma once


#include "EditorHelper.h"
#include "Widget/Widget.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12DescriptorHeap.h"

class Widget;
class EditorHelper;

class EditorSystem
{
	COMPLETED_DEVELOPMENT()
	EditorSystem() = default;
	COPY_PROHIBITED(EditorSystem);
public:

	static EditorSystem& Get() noexcept
	{
		static EditorSystem instance;
		return instance;
	}

public:

	/** Window 生成時の初期化のみ行う。*/
	bool Initialize() noexcept;

	/** API クラスセットアップ完了後に各 Widget セットアップを行う。*/
	bool PostInitialize(void* shaderResourceView) noexcept;

	/** API、JobSystem クラスより先に消去する必要があるため */
	void Shutdown() noexcept;

	/** Renderer 側で RendererTarget がセットされている必要があります。*/
	void Render() noexcept;

private:

	/** imgui で使用される各データの初期化 */
	bool SetUpImGuiObjects(void* finalFrameSRV) noexcept;
	void SetUpGuiStyle() noexcept;
	void AddFonts() noexcept;

	/** 使用 Widget の登録を行う。*/
	void RegisterWidgetsToContainer() noexcept;

	/** Window のドッキングを行うウィンドウを生成 */
	void ShowDockingWindow() const noexcept;

	/** Undo Redo ドラッグアンドドロップなどに使用 */
	void ChackClickedCommand() noexcept;

private:

	Job m_job;

	// * コマンド入力時使用
	Stopwatch m_stopwatch;

	// * Gui 用描画クラス配列
	Vector<UniquePtr<Widget>> m_widgets;

	// * font 用ヒープ
	D3D12DescriptorHeap m_descriptHeap;
};