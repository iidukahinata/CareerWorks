/**
* @file    IComponent.h
* @brief
*
* @date	   2022/10/03 2022年度初版
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
	virtual void Deserialization(FileStream* file);

public:

	/** シーンロード中の可能性があるため GameObject / Component の検索は行わない方が良い。*/
	virtual void OnInitialize() {}

	/** 所属 Scene が AddToWorld 適用時に呼び出される。*/
	virtual void OnStart() {}

	/** 所属 Scene が RemoveFromWorld 適用時に呼び出される。*/
	virtual void OnStop() {}

	/** GameObject から解放されるタイミングで呼び出される。*/
	virtual void OnRemove() { OnStop(); }

	/** 派生先で使用する場合は、TickFunction をシステム側に登録する必要があります。*/
	virtual void Tick(double deltaTime) {}

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

	bool m_active;

	GameObject* m_owner = nullptr;
};

template<class T>
FORCEINLINE bool IComponent::IsSameClass() const noexcept
{
	return TypeInfo() == typeid(T);
}