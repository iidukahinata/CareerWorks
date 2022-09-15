/**
* @file    MyGui.h
* @brief   gui�Ǘ��N���X
*
* @date	   2022/09/11 2022�N�x����
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

	/** Window �������̏������̂ݍs���B*/
	bool Initialize() noexcept;

	/** API �N���X�Z�b�g�A�b�v������Ɋe Widget �Z�b�g�A�b�v���s���B*/
	bool PostInitialize() noexcept;

	/** API�AJobSystem �N���X����ɏ�������K�v�����邽�� */
	void Shutdown() noexcept;

	/** Renderer ���� RendererTarget ���Z�b�g����Ă���K�v������܂��B*/
	void Render() noexcept;

private:

	/** imgui �Ŏg�p�����e�f�[�^�̏����� */
	bool SetUpImguiObjects() noexcept;

	/** �g�p Widget �̓o�^���s���B*/
	void RegisterWidgetsToContainer() noexcept;

	/** Undo Redo �h���b�O�A���h�h���b�v�ȂǂɎg�p */
	void ChackClickedCommand() noexcept;

private:

	Job m_job;

	// * Gui �p�`��N���X�z��
	Vector<UniquePtr<Widget>> m_widgets;

	// * imgui �`��p
	D3D12DescriptorHeap m_descriptHeap;
};