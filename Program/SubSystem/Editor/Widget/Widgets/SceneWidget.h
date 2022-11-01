/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/10/31 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class GameObject;

class SceneWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

private:

	/** �e�q�֌W���l������ GameObject �̕\�� */
	void AddGameObjectToTree(GameObject* gameObject) noexcept;

	/** GameObject �����Ȃǂ̃��\�b�h���s���B*/
	void ShowGameObjectHelper() noexcept;

	/** GameObject �������̐ݒ�Ȃǂ̎w����s���B*/
	void ShowCreateWindow() noexcept;

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand(GameObject* gameObject) noexcept;

	GameObject* CatchDragObject() const noexcept;

	void ShowDragDropHelper() const noexcept;

private:

	World* m_world = nullptr;

	bool m_selectGameObject = false;

	GameObject* m_gameObject = nullptr;
};