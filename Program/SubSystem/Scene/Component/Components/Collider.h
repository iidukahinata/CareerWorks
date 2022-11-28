/**
* @file    Collider.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../ICollider.h"

class PhysX;

namespace physx
{
	class PxShape;
	class PxMaterial;
}

class Collider : public ICollider
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Collider, ICollider)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnStart() override;
	void OnStop() override;

	void SetActive(bool active) override;

public:

	/** shape */
	void SetShapeType(ShapeType type) override;
	ShapeType GetShapeType() const override;
	void SetTrigger(bool trigger) override;
	bool GetTrigger() const override;

	/** scale */
	void SetScale(const Math::Vector3& scale) override;
	const Math::Vector3& GetScale() const override;
	float GetRadius() const override;
	float GetHight() const override;

	/** offset */
	void SetRestOffset(float restOffset) override;
	float GetRestOffset() const override;
	void SetContactOffset(float contactOffset) override;
	float GetContactOffset() const override;

	/** material */
	void SetStaticFriction(float friction) override;
	float GetStaticFriction() const override;
	void SetDynamicFriction(float friction) override;
	float GetDynamicFriction() const override;
	void SetRestitution(float restitution) override;
	float GetRestitution() const override;

	/** アクセス */
	physx::PxShape* GetShape() noexcept;

private:

	physx::PxShape* CreateShape() const noexcept;

	void UpdateGeometry() const noexcept;

	void AttachToRegidBody() noexcept;
	void DetachFromRegidBody() noexcept;

private:

	PhysX* m_physics = nullptr;

	// shape data
	ShapeType m_shapeType = ShapeType::Box;
	bool m_isTrigger	  = false;
	Math::Vector3 m_scale = Math::Vector3::One;
	float m_restOffset	  = 0.0f;
	float m_contactOffset = 0.02f;

	// * material prams
	float m_staticFriction	= 0.0f;
	float m_dynamicFriction = 0.0f;
	float m_restitution		= 0.0f;

	physx::PxShape* m_shape		  = nullptr;
	physx::PxMaterial* m_material = nullptr;
};