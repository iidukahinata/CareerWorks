/**
* @file    IComponent.h
* @brief
*
* @date	   2022/10/25 2022年度初版
*/
#pragma once


#include "Transform.h"
#include "../TickFunction.h"

class Context;
class World;
class Scene;
class GameObject;

using ComponentType = ClassTypeData;

class IComponent
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(IComponent)
	COPY_PROHIBITED(IComponent)

	friend struct ComponentFactory;
public:

	IComponent();

	virtual void Serialized(FileStream* file) const;
	virtual void Deserialized(FileStream* file);

public:

	/** シーンロード中の可能性があるため GameObject / Component の検索は行わない方が良い。*/
	virtual void OnInitialize();

	/** world 登録時に呼び出される。*/
	virtual void OnRegister();

	/** world から登録解除時に呼び出される。*/
	virtual void OnUnRegister();

	/** ゲームオブジェクトの再生時 or 既に再生されている時はコンポーネント生成時に呼び出される。*/
	virtual void OnStart();

	/** コンポーネントの再生終了時に呼び出される。*/
	virtual void OnStop();

	/** GameObject から解放されるタイミングで呼び出される。*/
	virtual void OnRemove();

	/** 派生先で使用する場合は、TickFunction をシステム側に登録する必要があります。*/
	virtual void Tick(double deltaTime);

	/** 
	* 別スレッドで操作されている時に Remove 申請があっても安全に機能するためのフラグ関数
	* Remove 時に true の場合はその場で放棄され、 false の場合は AutoDestroyManager へ管理権を渡す。
	*/
	virtual bool Erasable();

public:

	/** true の場合、更新関係にも影響する。*/
	virtual void SetActive(bool active);
	bool GetActive() const noexcept;

	/** true の場合、更新処理が有効になります。*/
	void SetTickEnable(bool enable) noexcept;

	/** 指定優先度を元に更新順序を変更します。最大値 : 0 */
	void SetTickPriority(uint32_t priority) noexcept;

	/** 更新処理の前提条件を設定します。*/
	void AddTickPrerequisite(TickFunction* prerequisite) noexcept;

public:

	bool IsRegistered() const noexcept;
	bool IsBeginPlay() const noexcept;

	/** テンプレート型が this と同じ型の時 true を返します。*/
	template<class T>
	bool IsSameClass() const noexcept;

	/** アクセス */
	Transform& GetTransform() const noexcept;
	GameObject* GetOwner() const noexcept;
	Scene* GetScene() const noexcept;
	World* GetWorld() const noexcept;
	Context* GetContext() const noexcept;

protected:

	// * コンポーネントの更新処理を管理
	TickComponentFunction m_tickFunction;

private:

	// * 所属ゲームオブジェクトクラスを保持
	GameObject* m_owner = nullptr;

	// * コンポーネントの有効性を保持
	bool m_active = true;

	// * world に登録されているかを保持
	bool m_registered = false;

	// * コンポーネントが再生中かどうかを保持
	bool m_beginPlay = false;
};

template<class T>
FORCEINLINE bool IComponent::IsSameClass() const noexcept
{
	return TypeInfo() == typeid(T);
}