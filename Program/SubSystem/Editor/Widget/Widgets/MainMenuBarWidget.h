/**
* @file	   MainMenuBarWidget.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class Renderer;
class ResourceManager;
class Material;

class MainMenuBarWidget : public Widget
{
	WAIT_FOR_DEVELOPMENT("View && Assets �Ȃǖ��쐬")
public:

	void PostInitialize() override;
	void Draw() override;

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

private:

	/** �e�ݒ���s���� Window �̕\�����s���B*/
	void ShowSettingsWindow() noexcept;

	/** �V���� Scene �̐����Ɩ��O�w�� */
	void ShowNewSceneModal() noexcept;

	/** �Z�[�u�� Scene ���̎擾���s���B*/
	void ShowSaveAsModal() noexcept;

	/** �v���O�����̍ċN���m�F Window */
	void ShowRestartSystemModal() noexcept;

	/** �t���X�N���[������ ImGui �� WindowMenu ��\�� */
	void ShowWindowMenu() noexcept;

private:

	void ShowMaterial(StringView label, Material* material, std::function<void(Material*)> collBack) noexcept;

private:

	/** �h���b�O�A���h�h���b�v���� UI �֌W�̕\�� */
	template<class T>
	void ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;
	void ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;

	/** Drag Drop ���ꂽ�I�u�W�F�N�g�擾�� Helper �֐� */
	template<class T>
	T* CatchDragObject() const noexcept;
	IResource* CatchDragResourceObject(uint32_t selectType) const noexcept;

	/** ���\�[�X�����p Helper �֐� */
	template<class T>
	ResourceData* ShowSearchResourceHelper() noexcept;
	ResourceData* ShowSearchResourceHelper(uint32_t selectType) noexcept;
	bool OpenResourceHelper(StringView lavel) noexcept;

	template<class T>
	T* LoadResource(ResourceData* resourceData) const noexcept;
	IResource* LoadResource(ResourceData* resourceData) const noexcept;

private:

	World* m_world = nullptr;

	Renderer* m_renderer = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	bool m_openSettginsWindow = false;

	// * ���� helper �֐��p filter �����p
	ImGuiTextFilter m_searchResourceFilter;

	String m_searchResourceLavel;
};

template<class T>
FORCEINLINE void MainMenuBarWidget::ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ShowDragDropHelper(T::TypeData.Hash, hovered, r, g, b);
}

template<class T>
FORCEINLINE T* MainMenuBarWidget::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragResourceObject(T::TypeData.Hash));
}

template<class T>
FORCEINLINE ResourceData* MainMenuBarWidget::ShowSearchResourceHelper() noexcept
{
	return ShowSearchResourceHelper(T::TypeData.Hash);
}

template<class T>
FORCEINLINE T* MainMenuBarWidget::LoadResource(ResourceData* resourceData) const noexcept
{
	return dynamic_cast<T*>(LoadResource(resourceData));
}