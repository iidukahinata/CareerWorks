/**
* @file    EditorSystem.h
* @brief
*
* @date	   2022/10/21 2022�N�x����
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

	/** Window �������̏������̂ݍs���B*/
	bool Initialize() noexcept;

	/** API �N���X�Z�b�g�A�b�v������Ɋe Widget �Z�b�g�A�b�v���s���B*/
	bool PostInitialize(void* shaderResourceView) noexcept;

	/** API�AJobSystem �N���X����ɏ�������K�v�����邽�� */
	void Shutdown() noexcept;

	/** Renderer ���� RendererTarget ���Z�b�g����Ă���K�v������܂��B*/
	void Render() noexcept;

private:

	/** imgui �Ŏg�p�����e�f�[�^�̏����� */
	bool SetUpImGuiObjects(void* finalFrameSRV) noexcept;
	void SetUpGuiStyle() noexcept;
	void AddFonts() noexcept;

	/** �g�p Widget �̓o�^���s���B*/
	void RegisterWidgetsToContainer() noexcept;

	/** Window �̃h�b�L���O���s���E�B���h�E�𐶐� */
	void ShowDockingWindow() const noexcept;

	/** Undo Redo �h���b�O�A���h�h���b�v�ȂǂɎg�p */
	void ChackClickedCommand() noexcept;

private:

	Job m_job;

	// * �R�}���h���͎��g�p
	Stopwatch m_stopwatch;

	// * Gui �p�`��N���X�z��
	Vector<UniquePtr<Widget>> m_widgets;

	// * font �p�q�[�v
	D3D12DescriptorHeap m_descriptHeap;
};