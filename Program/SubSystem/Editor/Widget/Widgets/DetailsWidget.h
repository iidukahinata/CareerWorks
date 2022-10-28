/**
* @file	   DetailsWidget.h
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/
#pragma once


#include "../Widget.h"

class GameObject;
class ResourceData;
class DetailsObject;

class World;
class ResourceManager;

class DetailsWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	~DetailsWidget();

	void PostInitialize() override;
	void Draw() override;

	/** �\�� DetailsObject �̒ǉ��Ȃǂ��s���ꍇ�Ɏg�p */
	void RequestUpdate() noexcept;

	World* GetWorld() const noexcept;
	ResourceManager* GetResourceManager() const noexcept;

public:

	/** �R�}���h���s������ōs���Ă��邽�ߌĂяo�����ł̏����͕s�v */
	static void SelectGameObject(GameObject* gameObject) noexcept;
	static void SelectResource(ResourceData* resourceData) noexcept;

	/** ��I����V�[���؂�ւ����Ɏg�p���邽�߁B*/
	static void ClearSelectObject() noexcept;

private:

	/** Select �֐��̌Ăяo�����ŃR�}���h�ݒ肷���Ԃ��Ȃ������� */
	static void SelectGameObjectInternal(GameObject* gameObject) noexcept;
	static void SelectResourceInternal(ResourceData* resourceData) noexcept;

private:

	World* m_world = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	// * �\�����I�u�W�F�N�g�̍X�V�v���̂���
	static bool m_requestUpdate;

	// * select objects
	static GameObject* m_selectGameObject;
	static ResourceData* m_selectResourceData;

	// * this object pointor
	static DetailsWidget* m_detailsWidget;

	// * �I���I�u�W�F�N�g�̏ڍ׏��
	static Vector<UniquePtr<DetailsObject>> m_detailsObjects;
};