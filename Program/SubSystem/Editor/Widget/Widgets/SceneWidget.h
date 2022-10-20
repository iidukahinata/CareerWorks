/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
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
	void AddGameObjectToTree(GameObject* gameObject) const noexcept;

	/** GameObject �����Ȃǂ̃��\�b�h���s���B*/
	void ShowGameObjectHelper() const noexcept;

	/** GameObject �������̐ݒ�Ȃǂ̎w����s���B*/
	void ShowGameObjectCreateWindow() noexcept;

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand(GameObject* gameObject) const noexcept;

	GameObject* CatchDragObject() const noexcept;

private:

	World* m_world;
};