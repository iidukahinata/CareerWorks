/**
* @file    GameObject.h
* @brief
*
* @date	   2022/11/04 2022年度初版
*/
#pragma once


#include "Component/IComponent.h"
#include "Component/Transform.h"

class GameObject
{
	COMPLETED_DEVELOPMENT()
	CLASS_DATA(GameObject)
	COPY_PROHIBITED(GameObject)

	friend struct GameObjectFactory;
public:

	GameObject();

	void Serialized(FileStream* file) const noexcept;
	void Deserialized(FileStream* file) noexcept;

public:

	/** component の world 登録を行う。*/
	void RegisterAllComponents() noexcept;

	/** component の world 登録解除を行う。*/
	void UnRegisterAllComponents() noexcept;

	/** ゲームオブジェクトの再生時に呼び出される。*/
	void BeginPlay() noexcept;

	/** ゲームオブジェクトの終了時に呼び出される。*/
	void EndPlay() noexcept;

	/** ゲームオブジェクトの更新 */
	void Tick(double deltaTime) noexcept;

	/** 自身の消去を行う。*/
	void Destroy() noexcept;

public:

	/** 指定名コンポーネントの追加。指定名が存在しない等の場合、null を返す。*/
	IComponent* AddComponent(StringView name) noexcept;

private:

	void AddComponent(IComponent* component) noexcept;
	void PostAddComponent(IComponent* component);

public:

	/** 保持するコンポーネントを消去するため、そのアドレスを引数とする。*/
	void RemoveComponent(IComponent* component) noexcept;

	/** 型名からコンポーネントを検索。*/
	template<class T>
	T* GetComponent();
	void GetAllComponent(Vector<IComponent*>& components) const noexcept;
	const Map<uint32_t, UniquePtr<IComponent>>& GetAllComponent() const noexcept;

	/** 指定名コンポーネントを保持する場合、そのアドレスを返す。*/
	IComponent* FindComponent(StringView name) const noexcept;

	/** 全コンポーネントの消去を行う。*/
	void ClearComponets() noexcept;

public:

	void SetActive(bool active) noexcept;
	bool GetActive() const noexcept;

	/** 各GameObject識別用に使用。*/
	void SetName(StringView name) noexcept;
	const String& GetName() const noexcept;

	/** Worldクラスで以外で使用されると、場合によってはScene終了時までメモリが解放されず残り続けます。*/
	void SetID(uint32_t id) noexcept;
	uint32_t GetID() const noexcept;

	/** true 時は AutoDestroySystem への受託の申請要請 */
	bool RequestAutoDestroy() const noexcept;

	/** アクセス */
	Transform& GetTransform() noexcept;
	Scene* GetOwner() const noexcept;
	World* GetWorld() const noexcept;
	Context* GetContext() const noexcept;

protected:

	// * ゲームオブジェクトの更新処理を管理
	TickGameObjectFunction m_tickFunction;

private:

	// * 所属シーンクラスを保持
	Scene* m_owner = nullptr;

	// * 所属ワールドクラスを保持
	World* m_world = nullptr;

	// * WorldクラスでのID値
	uint32_t m_id = 0;

	// * Worldクラスで探索時等に使用
	String m_path = "GameObject";

	// * コンポーネントの有効性を保持
	bool m_active = true;

	// * world に登録されているかを保持
	bool m_registered = false;

	// * ゲームオブジェクトが再生中であるかを保持
	bool m_isPlaying = false;

	Transform m_transform;

	// * -> ハッシュ値 : 各コンポーネントオブジェクト
	Map<uint32_t, UniquePtr<IComponent>> m_components;
};

template<class T>
FORCEINLINE T* GameObject::GetComponent()
{
	return dynamic_cast<T*>(FindComponent(T::TypeData.Name));
}