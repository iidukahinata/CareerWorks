/**
* @file    World.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "GameObject.h"
#include "AutoDestroySystem.h"
#include "SubSystem/Core/ISubsystem.h"

enum class WorldType
{
	Game,

	Editor,

	PlayEditor,

	None,
};

class Scene;

class World : public ISubsystem
{
	WAIT_FOR_DEVELOPMENT("���݃V�[���̍ă��[�h�̐\�����̏����͂܂��Ȃ� && Play Stop ��̃��\�[�X�A�N�Z�X���͎d�l�����܂莟��쐬")
	SUB_CLASS(World, ISubsystem)
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

	/** CurrentScene �܂��͎w�肳�ꂽ Scene ��� GameObject �𐶐��B*/
	GameObject* CreateGameObject(Scene* scene = nullptr) noexcept;

	/** �e�N���X�� GameObject �����̒ʒB���s���B*/
	void DestroyGameObject(GameObject* gameObject) const noexcept;

	/** ���[�h���ꂽ�S�ẴV�[���փA�N�Z�X���錟������(����) */
	GameObject* GetGameObjectByName(StringView name) const noexcept;

public:

	/** �A�N�Z�X */
	void SetCurrentScene(Scene* scene) noexcept;
	Scene* GetCurrentScene() const noexcept;
	Scene* GetScene(StringView name) noexcept;
	const Unordered_Map<String, Scene*>& GetSceneList() const noexcept;

	AutoDestroySystem& GetAutoDestroySystem() noexcept;

private:

	/** Scene ���\�b�h */
	void AddScene(StringView name, Scene* scene) noexcept;
	void RemoveScene(Scene* scene) noexcept;
	void RemoveScene(StringView name) noexcept;

public:

	/** Editor �N�����͎��s���̂� true �ɂȂ�B*/
	bool IsGameMode() const noexcept;

	/** Editor �N�����͎��s���ł� true �ɂȂ�B*/
	bool IsEditorMode() const noexcept;

private:

	/** Event Listener �I�u�W�F�N�g������ */
	void StartupListenerObjects() noexcept;

	/** Editor ���� State �ύX���̃V�[���������L�q */
	void ChangeWorldType(WorldType type) noexcept;

private:

	// * Scene Load / Unload ���s�p
	ResourceManager* m_resourceManager = nullptr;

	// * Update �p
	Job m_job;

	// * Listener Objects
	EventListener m_changeSceneListener;
	EventListener m_loadSceneCompleteListener;

#ifdef IS_EDITOR
	EventListener m_changeEditorStateListener;
#endif // IS_EDITOR


	// * �g�p���V�[���I�u�W�F�N�g
	Scene* m_currentScene = nullptr;

	// * ���[�h�ς݃V�[�����X�g
	Unordered_Map<String, Scene*> m_sceneList;

	// * �����V�[���̓����ǂݍ��݂ɑΉ������邽��
	Unordered_Map<String, ResourceHandle*> m_resourceHandles;

	// * Game Mode ���s���̎��͏�� WorldType::Game �ɂȂ�
	WorldType m_worldType = WorldType::None;

	AutoDestroySystem m_autoDestroySystem;
};