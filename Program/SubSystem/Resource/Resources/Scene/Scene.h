/**
* @file    Scene.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include "../IResource.h"
#include "SubSystem/Scene/GameObject.h"

class Scene : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Scene)
public:

	~Scene();

	/** asset ファイルと独自データの作成を行う */
	static Scene* Create(StringView name) noexcept;

	bool Load(StringView path) override;

	void Update() override;

	void AddToWorld() noexcept;
	void RemoveFromWorld() noexcept;

public:

	/** GameObject メソッド */
	void AddGameObject(GameObject* gameObject) noexcept;
	GameObject* GetGameObjectByID(uint32_t id) const noexcept;
	GameObject* GetGameObjectByName(StringView name) const noexcept;
	void RemoveGameObject(GameObject* gameObject) noexcept;
	void ClearGameObjects() noexcept;

	const Vector<UniquePtr<GameObject>>& GetAllGameObjects() const noexcept;

private:

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	Vector<UniquePtr<GameObject>> m_gameObjects;
};