/**
* @file	   EditerHelper.h
* @brief
*
* @date	   2022/10/21 2022年度初版
*/
#pragma once


#include "SubSystem/Core/Command/Command.h"
#include "SubSystem/Core/Command/CommandList.h"

class Texture;
class D3D12DescriptorHeap;

enum IconType
{
	Icon_Folder,
	Icon_Scene,
	Icon_Model,
	Icon_Mesh,
	Icon_Material,
	Icon_Texture,
	Icon_Shader,
	Icon_Audio,
	Icon_Script,
	Icon_None,
};

class EditorHelper
{
	COMPLETED_DEVELOPMENT()
	EditorHelper() = default;
	COPY_PROHIBITED(EditorHelper);
public:

	static EditorHelper& Get()
	{
		static EditorHelper instance;
		return instance;
	}

public:

	void Initialize(D3D12DescriptorHeap* descriptorHeap, void* finalFrameSRV) noexcept;
	void BegineRenderer() noexcept;
	void Shutdown() noexcept;

public:

	/**
	* 表示する Texture DescriptorHeap を管理するために作成。
	* この関数を無視して ImGui::Image を使用すると画面表示が行われない。
	*/
	void AddImage(
		void* shaderResourceView,
		const ImVec2& imageSize,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
		const ImVec4& border_col = ImVec4(0, 0, 0, 0)
	) noexcept;

	/**
	* 表示する Texture DescriptorHeap を管理するために作成。
	* この関数を無視して ImGui::ImageButton を使用すると画面表示が行われない。
	*/
	bool AddImageButton(
		void* shaderResourceView,
		const ImVec2& imageSize,
		const ImVec2& uv0 = ImVec2(0, 0),
		const ImVec2& uv1 = ImVec2(1, 1),
		const int frame_padding = -1,
		const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
		const ImVec4& tint_col = ImVec4(1, 1, 1, 1)
	) noexcept;

public:

	void* GetFinalFrameTexture() const noexcept;
	Texture* GetIconTexture(IconType type) const noexcept;
	IconType GetIconTypeFromResourceType(uint32_t type) const noexcept;
	uint32_t GetResourceTypeByIconType(IconType type) const noexcept;

private:

	void RegisterIconTexture() noexcept;

public:

	void UndoCommand() noexcept;
	void RedoCommand() noexcept;
	void FlushCommandList() noexcept;

	void AddEditorCommand(UniquePtr<ICommand> command) noexcept;

private:

	// * Undo & Redo 用の Editor コマンドリスト
	CommandList m_commandList;

	// * GUI で使用される icon Texture
	Vector<Texture*> m_iconTextures;

	// * Image 表示用
	UINT m_numImage = 1;
	D3D12DescriptorHeap* m_descriptorHeap;

	// * Last Frame データ保持
	void* m_finalFrameSRV = nullptr;
};

/** Editor コマンド発行の簡略化のための Register 関数 */
template<class Lambda, class T>
void RegisterEditorCommand(Lambda&& func, const T& next, const T& pre)
{
	EditorHelper::Get().AddEditorCommand(std::make_unique<EditorCommand<T>>(func, next, pre));
}