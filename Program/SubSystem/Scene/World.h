/**
* @file    World.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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
	WAIT_FOR_DEVELOPMENT("現在シーンの再ロードの申請時の処理はまだない && Play Stop 後のリソースアクセス等は仕様が決まり次第作成")
	SUB_CLASS(World, ISubsystem)
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

	/** CurrentScene または指定された Scene 上で GameObject を生成。*/
	GameObject* CreateGameObject(Scene* scene = nullptr) noexcept;

	/** 各クラスに GameObject 消去の通達を行う。*/
	void DestroyGameObject(GameObject* gameObject) const noexcept;

	/** ロードされた全てのシーンへアクセスする検索処理(高価) */
	GameObject* GetGameObjectByName(StringView name) const noexcept;

public:

	/** アクセス */
	void SetCurrentScene(Scene* scene) noexcept;
	Scene* GetCurrentScene() const noexcept;
	Scene* GetScene(StringView name) noexcept;
	const Unordered_Map<String, Scene*>& GetSceneList() const noexcept;

	AutoDestroySystem& GetAutoDestroySystem() noexcept;

private:

	/** Scene メソッド */
	void AddScene(StringView name, Scene* scene) noexcept;
	void RemoveScene(Scene* scene) noexcept;
	void RemoveScene(StringView name) noexcept;

public:

	/** Editor 起動時は実行中のみ true になる。*/
	bool IsGameMode() const noexcept;

	/** Editor 起動時は実行中でも true になる。*/
	bool IsEditorMode() const noexcept;

private:

	/** Event Listener オブジェクト初期化 */
	void StartupListenerObjects() noexcept;

	/** Editor 時の State 変更時のシーン処理を記述 */
	void ChangeWorldType(WorldType type) noexcept;

private:

	// * Scene Load / Unload 発行用
	ResourceManager* m_resourceManager = nullptr;

	// * Update 用
	Job m_job;

	// * Listener Objects
	EventListener m_changeSceneListener;
	EventListener m_loadSceneCompleteListener;

#ifdef IS_EDITOR
	EventListener m_changeEditorStateListener;
#endif // IS_EDITOR


	// * 使用中シーンオブジェクト
	Scene* m_currentScene = nullptr;

	// * ロード済みシーンリスト
	Unordered_Map<String, Scene*> m_sceneList;

	// * 複数シーンの同時読み込みに対応させるため
	Unordered_Map<String, ResourceHandle*> m_resourceHandles;

	// * Game Mode 実行時の時は常に WorldType::Game になる
	WorldType m_worldType = WorldType::None;

	AutoDestroySystem m_autoDestroySystem;
};