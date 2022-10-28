/**
* @file	   MainMenuBarWidget.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;

class MainMenuBarWidget : public Widget
{
	WAIT_FOR_DEVELOPMENT("View && Assets �Ȃǖ��쐬")
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** �V���� Scene �̐����Ɩ��O�w�� */
	void ShowNewSceneModal() noexcept;

	/** �Z�[�u�� Scene ���̎擾���s���B*/
	void ShowSaveAsModal() noexcept;

	/** �t���X�N���[������ ImGui �� WindowMenu ��\�� */
	void ShowWindowMenu() noexcept;

private:

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;
};