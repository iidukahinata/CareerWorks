/**
* @file	   EditerHelper.h
* @brief
*
* @date	   2022/10/21 2022�N�x����
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
	* �\������ Texture DescriptorHeap ���Ǘ����邽�߂ɍ쐬�B
	* ���̊֐��𖳎����� ImGui::Image ���g�p����Ɖ�ʕ\�����s���Ȃ��B
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
	* �\������ Texture DescriptorHeap ���Ǘ����邽�߂ɍ쐬�B
	* ���̊֐��𖳎����� ImGui::ImageButton ���g�p����Ɖ�ʕ\�����s���Ȃ��B
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

	// * Undo & Redo �p�� Editor �R�}���h���X�g
	CommandList m_commandList;

	// * GUI �Ŏg�p����� icon Texture
	Vector<Texture*> m_iconTextures;

	// * Image �\���p
	UINT m_numImage = 1;
	D3D12DescriptorHeap* m_descriptorHeap;

	// * Last Frame �f�[�^�ێ�
	void* m_finalFrameSRV = nullptr;
};

/** Editor �R�}���h���s�̊ȗ����̂��߂� Register �֐� */
template<class Lambda, class T>
void RegisterEditorCommand(Lambda&& func, const T& next, const T& pre)
{
	EditorHelper::Get().AddEditorCommand(std::make_unique<EditorCommand<T>>(func, next, pre));
}