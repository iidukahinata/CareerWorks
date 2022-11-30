/**
* @file	   SceneWidget.h
* @brief
*
* @date	   2022/10/31 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;
class GameObject;
class Model;

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

	/** �I�𒆃I�u�W�F�N�g UI ��\�� */
	void ShowSelectUI() const noexcept;

	/** ���͒l���� Window �̐؂�ւ��Ȃǂ��s�����߁B*/
	void ChackClickedCommand(GameObject* gameObject) noexcept;

	/** ���f���f�[�^����Q�[���I�u�W�F�N�g���\�z */
	void CreateGameObjectFromModel(Model* model) noexcept;

private:

	/** Drag Drop ���ꂽ�I�u�W�F�N�g�擾�� Helper �֐� */
	template<class T>
	T* CatchDragObject() const noexcept;
	GameObject* CatchDragGameObject() const noexcept;
	ResourceData* CatchDragResourceData() const noexcept;
	IResource* CatchDragResourceObject(uint32_t selectType) const noexcept;

private:

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	bool m_selectGameObject = false;

	GameObject* m_gameObject = nullptr;

	Vector<GameObject*> m_allRootGameObjects;

	EventListener m_eventListener;
};

template<class T>
FORCEINLINE T* SceneWidget::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragResourceObject(T::TypeData.Hash));
}