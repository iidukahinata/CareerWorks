/**
* @file    World.h
* @brief
*
* @date	   2022/10/02 2022年度初版
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
	* 指定されたシーンリソースのロード処理発行を行う。
	* ロード完了時は LoadSceneCompleteEvent が発行され、通知される。
	*/
	void LoadScene(StringView name) noexcept;

	/** 自動的には Unload されないため使用者が管理出来るために作成。*/
	void UnloadScene(StringView name) noexcept;

	/** Scene の変更を行うイベント発行を行う。*/
	void ChangeScene(StringView name) noexcept;

public:

	/** CurrentScene または指定されたシーン上で GameObject を生成。*/
	GameObject* CreateGameObject(Scene* scene = nullptr) noexcept;

	/** 各クラスに GameObject 消去の通達を行う。*/
	void DestroyGameObject(GameObject* gameObject) noexcept;

	/** ロードされた全てのシーンへアクセスする検索処理(高価) */
	GameObject* GetGameObjectByName(StringView name) const noexcept;

public:

	/** アクセス */
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

	// * Update 用
	Job m_job;

	// * Listener Objects
	EventListener m_changeSceneListener;
	EventListener m_loadSceneCompleteListener;

	// * 使用中シーンオブジェクト
	Scene* m_currentScene = nullptr;

	// * ロード済みシーンリスト
	Unordered_Map<String, Scene*> m_sceneList;

	// * 複数シーンの同時読み込みに対応させるため
	Unordered_Map<String, ResourceHandle*> m_resourceHandles;

	// * Load / Unload 発行用
	ResourceManager* m_resourceManager;
};