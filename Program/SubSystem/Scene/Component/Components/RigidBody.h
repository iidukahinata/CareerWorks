/**
* @file    RigidBody.h
* @brief
*
* @date	   2022/11/06 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class PhysX;
class Collider;

namespace physx
{
	class PxRigidActor;
}

enum BodyType
{
	Dynamic,

	Static,
};

enum ForceMode
{
	Force,

	Impulse,
};

class RigidBody : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(RigidBody)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnStart() override;
	void OnStop() override;

	void Update() noexcept;

	void SetActive(bool active) override;

public:

	void SetBodyType(BodyType type) noexcept;
	const BodyType& GetBodyType() const noexcept;
	void SetCollider(Collider* collider) noexcept;

	/** force */
	void AddForce(const Math::Vector3& force, ForceMode mode = ForceMode::Force) noexcept;
	void AddTorque(const Math::Vector3& torque, ForceMode mode = ForceMode::Force) noexcept;

	/** velocity */
	void SetVelocity(const Math::Vector3& velocity) noexcept;
	void AddVelocity(const Math::Vector3& addVelocity) noexcept;
	Math::Vector3 GetVelocity() const noexcept;

	/** angular velocity */
	void SetAngularVelocity(const Math::Vector3& velocity) noexcept;
	void AddAngularVelocity(const Math::Vector3& addVelocity) noexcept;
	Math::Vector3 GetAngularVelocity() const noexcept;

	/** mass */
	void SetMass(float mass) noexcept;
	float GetMass() const noexcept;

	/** flags */
	void SetUseGravity(bool useGravity) noexcept;
	bool UseGravity() const noexcept;
	void SetKinematic(bool kinematic) noexcept;
	bool IsKinematic() const noexcept;
	bool IsDynamic() const noexcept;

	/** lock */
	void SetPositionLock(const Math::Vector3& positionLock) noexcept;
	const Math::Vector3& GetPositionLock() const noexcept;
	void SetRotationLock(const Math::Vector3& rotationLock) noexcept;
	const Math::Vector3& GetRotationLock() const noexcept;

	/** transform */
	void SetPosition(const Math::Vector3& pos) const noexcept;
	void SetRotation(const Math::Vector3& rot) const noexcept;

	/** アクセス */
	physx::PxRigidActor* GetRigidActor() noexcept;

private:

	void RegisterToPhysics() noexcept;
	void UnRegisterFromPhysics() noexcept;

private:

	PhysX* m_physics = nullptr;
	Collider* m_collider = nullptr;

	bool m_isRegister = false;

	physx::PxRigidActor* m_actor = nullptr;

	BodyType m_bodyType = BodyType::Dynamic;
	float m_mass		= 1.0f;
	bool m_isKinematic	= false;
	bool m_useGravity	= true;
	Math::Vector3 m_positionLock = Math::Vector3::Zero;
	Math::Vector3 m_rotationLock = Math::Vector3::Zero;

	Math::Vector3 m_velocity;
	Math::Vector3 m_angukarVelocity;
};