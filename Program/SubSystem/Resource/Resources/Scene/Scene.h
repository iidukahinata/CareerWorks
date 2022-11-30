/**
* @file    Scene.h
* @brief
*
* @date	   2022/11/28 2022年度初版
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

	/** asset ファイルと独自データの作成を行う。*/
	static Scene* Create(StringView name) noexcept;

	/** Scene データを */
	void Save(StringView path) noexcept;

	/** Save As... など別名として保存する場合に使用 */
	void SaveAs(StringView name) noexcept;

	/** 独自 Scene データの読み込みを行う。*/
	bool Load(StringView path) override;

	/** 各データの更新処理を行う。*/
	void Update() override;

public:

	/** World 適用時に呼び出され、ゲームオブジェクトの登録を行う。*/
	void AddToWorld() noexcept;

	/** World 取消時に呼び出され、ゲームオブジェクトの登録解除を行う。*/
	void RemoveFromWorld() noexcept;

public:

	/** GameObject メソッド */
	void AddGameObject(GameObject* gameObject) noexcept;
	GameObject* GetGameObjectByID(uint32_t id) const noexcept;
	GameObject* GetGameObjectByName(StringView name) const noexcept;
	const Vector<UniquePtr<GameObject>>& GetAllGameObjects() const noexcept;
	void GetAllRootGameObjects(Vector<GameObject*>& gameObjects) const noexcept;
	void RemoveGameObject(GameObject* gameObject) noexcept;
	void ClearGameObjects() noexcept;

private:

	/** 独自 Scene データを構築しファイル更新を行う。*/
	void UpdateProprietaryDataFile(StringView path = String()) noexcept;

	/** ResourceData データを構築しファイル更新を行う。*/
	void UpdateResourceDataFile() noexcept;

private:

	// * 所属ワールドクラスを保持
	World* m_world = nullptr;

	// * 管理中ゲームオブジェクトリスト
	Vector<UniquePtr<GameObject>> m_gameObjects;
};