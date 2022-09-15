/**
* @file    MyGui.h
* @brief   gui管理クラス
*
* @date	   2022/09/11 2022年度初版
*/
#pragma once


#include "EditorHelper.h"
#include "Widget/Widget.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12DescriptorHeap.h"

class Widget;
class EditerHelper;

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

	bool Initialize() noexcept;

	bool PostInitialize() noexcept;

	void Shutdown() noexcept;

	void Render() noexcept;

private:

	/** imgui で使用される各データの初期化 */
	bool SetUpImguiObjects() noexcept;

	/** 使用 Widget の登録を行う。*/
	void RegisterWidgetsToContainer() noexcept;

private:

	Job m_job;

	// * Gui 用描画クラス配列
	Vector<UniquePtr<Widget>> m_widgets;

	// * imgui 描画用
	D3D12DescriptorHeap m_descriptHeap;
};