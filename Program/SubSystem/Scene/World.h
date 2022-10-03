/**
* @file    World.h
* @brief
*
* @date	   2022/10/02 2022�N�x����
*/
#pragma once


#include "GameObject.h"
#include "SubSystem/Core/ISubsystem.h"

class Scene;

enum SceneState
{
	Stop,
	Active,
	Change,
};

class World : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(World)
public:

	bool Initialize() override;
	void Shutdown() override;

	void Update() noexcept;

public:

	/** 
	* �w�肳�ꂽ�V�[�����\�[�X�̃��[�h�������s���s���B
	* ���[�h�������� LoadSceneCompleteEvent �����s����A�ʒm�����B
	*/
	void LoadScene(StringView name) noexcept;

	/** �����I�ɂ� Unload ����Ȃ����ߎg�p�҂��Ǘ��o���邽�߂ɍ쐬�B*/
	void UnloadScene(StringView name) noexcept;

	/** Scene �̕ύX���s���C�x���g���s���s���B*/
	void ChangeScene(StringView name) noexcept;

public:

	/** CurrentScene �܂��͎w�肳�ꂽ�V�[����� GameObject �𐶐��B*/
	GameObject* CreateGameObject(Scene* scene = nullptr) noexcept;

	/** �e�N���X�� GameObject �����̒ʒB���s���B*/
	void DestroyGameObject(GameObject* gameObject) noexcept;

	/** ���[�h���ꂽ�S�ẴV�[���փA�N�Z�X���錟������(����) */
	GameObject* GetGameObjectByName(StringView name) const noexcept;

public:

	/** �A�N�Z�X */
	void SetCurrentScene(Scene* scene) noexcept;
	Scene* GetCurrentScene() const noexcept;
	Scene* GetScene(StringView name) noexcept;
	const Unordered_Map<String, Scene*>& GetSceneList() const noexcept;

private:

	void SetUpListenerObjects() noexcept;

	void AddScene(StringView name, Scene* scene) noexcept;
	void RemoveScene(Scene* scene) noexcept;
	void RemoveScene(StringView name) noexcept;

private:

	// * Update �p
	Job m_job;

	// * Listener Objects
	EventListener m_changeSceneListener;
	EventListener m_loadSceneCompleteListener;

	// * �g�p���V�[���I�u�W�F�N�g
	Scene* m_currentScene = nullptr;

	// * ���[�h�ς݃V�[�����X�g
	Unordered_Map<String, Scene*> m_sceneList;

	// * �����V�[���̓����ǂݍ��݂ɑΉ������邽��
	Unordered_Map<String, ResourceHandle*> m_resourceHandles;

	// * Load / Unload ���s�p
	ResourceManager* m_resourceManager;
};