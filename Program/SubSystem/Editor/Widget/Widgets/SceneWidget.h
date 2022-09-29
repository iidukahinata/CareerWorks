/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/09/13 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class GameObject;

class SceneWidget : public Widget
{
	IN_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** �e�q�֌W���l������ GameObject �̕\�� */
	void AddGameObjectToTree(GameObject* gameObject) noexcept;

	/** �e�q�֌W�̕ύX�A�ڍ׏��̕\���ȂǍs�� */
	void SelectGameObject(GameObject* gameObject) noexcept;

	/** GameObject �����Ȃǂ̃��\�b�h���s���B*/
	void ShowGameObjectHelper() noexcept;

	/** GameObject �������̐ݒ�Ȃǂ̎w����s���B*/
	void ShowGameObjectCreateWindow();

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand() noexcept;

private:

	World* m_world;
};