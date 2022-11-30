/**
* @file    Scene.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../IResource.h"
#include "SubSystem/Scene/GameObject.h"

class World;

class Scene : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Scene, IResource)
public:

	Scene();
	~Scene();

	/** asset �t�@�C���ƓƎ��f�[�^�̍쐬���s���B*/
	static Scene* Create(StringView name) noexcept;

	/** Scene �f�[�^�� */
	void Save(StringView path) noexcept;

	/** Save As... �ȂǕʖ��Ƃ��ĕۑ�����ꍇ�Ɏg�p */
	void SaveAs(StringView name) noexcept;

	/** �Ǝ� Scene �f�[�^�̓ǂݍ��݂��s���B*/
	bool Load(StringView path) override;

	/** �e�f�[�^�̍X�V�������s���B*/
	void Update() override;

public:

	/** World �K�p���ɌĂяo����A�Q�[���I�u�W�F�N�g�̓o�^���s���B*/
	void AddToWorld() noexcept;

	/** World ������ɌĂяo����A�Q�[���I�u�W�F�N�g�̓o�^�������s���B*/
	void RemoveFromWorld() noexcept;

public:

	/** GameObject ���\�b�h */
	void AddGameObject(GameObject* gameObject) noexcept;
	GameObject* GetGameObjectByID(uint32_t id) const noexcept;
	GameObject* GetGameObjectByName(StringView name) const noexcept;
	const Vector<UniquePtr<GameObject>>& GetAllGameObjects() const noexcept;
	void GetAllRootGameObjects(Vector<GameObject*>& gameObjects) const noexcept;
	void RemoveGameObject(GameObject* gameObject) noexcept;
	void ClearGameObjects() noexcept;

private:

	/** �Ǝ� Scene �f�[�^���\�z���t�@�C���X�V���s���B*/
	void UpdateProprietaryDataFile(StringView path = String()) noexcept;

	/** ResourceData �f�[�^���\�z���t�@�C���X�V���s���B*/
	void UpdateResourceDataFile() noexcept;

private:

	// * �������[���h�N���X��ێ�
	World* m_world = nullptr;

	// * �Ǘ����Q�[���I�u�W�F�N�g���X�g
	Vector<UniquePtr<GameObject>> m_gameObjects;
};