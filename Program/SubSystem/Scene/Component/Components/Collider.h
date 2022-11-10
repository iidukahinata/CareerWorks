/**
* @file    Collider.h
* @brief
*
* @date	   2022/11/07 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class PhysX;
class RigidBody;

namespace physx
{
	class PxShape;
	class PxMaterial;
}

enum ShapeType
{
	Box,

	Sphere,

	Capsule,

	//Mesh,

	//ConvexMesh,
};

class Collider : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Collider)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnStart() override;
	void OnStop() override;

	void SetActive(bool active) override;

public:

	/** shape */
	void SetShapeType(ShapeType type) noexcept;
	const ShapeType& GetShapeType() const noexcept;
	void SetTrigger(bool trigger) noexcept;
	bool IsTrigger() const noexcept;

	/** scale */
	void SetScale(const Math::Vector3& scale) noexcept;
	const Math::Vector3& GetScale() const noexcept;
	float GetRadius() const noexcept;
	float GetHight() const noexcept;

	/** offset */
	void SetRestOffset(float restOffset) noexcept;
	float GetRestOffset() const noexcept;
	void SetContactOffset(float contactOffset) noexcept;
	float GetContactOffset() const noexcept;

	/** material */
	void SetStaticFriction(float friction) noexcept;
	float GetStaticFriction() const noexcept;
	void SetDynamicFriction(float friction) noexcept;
	float GetDynamicFriction() const noexcept;
	void SetRestitution(float restitution) noexcept;
	float GetRestitution() const noexcept;

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