/**
* @file    RigidBody.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IRigidBody.h"

class PhysX;
class Collider;

namespace physx
{
	class PxRigidActor;
}

class RigidBody : public IRigidBody
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(RigidBody, IRigidBody)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void OnInitialize() override;
	void OnStart() override;
	void OnStop() override;

	void SetActive(bool active) override;

	void PreUpdate() override;
	void Update() override;

public:

	void SetCollider(ICollider* collider) override;

	/** body type */
	void SetBodyType(BodyType type) override;
	BodyType GetBodyType() const override;

	/** force */
	void AddForce(const Math::Vector3& force, ForceMode mode = ForceMode::Force) override;
	void AddTorque(const Math::Vector3& torque, ForceMode mode = ForceMode::Force) override;

	/** velocity */
	void SetVelocity(const Math::Vector3& velocity) override;
	void AddVelocity(const Math::Vector3& addVelocity) override;
	Math::Vector3 GetVelocity() const override;

	/** angular velocity */
	void SetAngularVelocity(const Math::Vector3& velocity) override;
	void AddAngularVelocity(const Math::Vector3& addVelocity) override;
	Math::Vector3 GetAngularVelocity() const override;

	/** mass */
	void SetMass(float mass) override;
	float GetMass() const override;

	/** flags */
	void SetUseGravity(bool useGravity) override;
	bool UseGravity() const override;
	void SetKinematic(bool kinematic) override;
	bool IsKinematic() const override;
	bool IsDynamic() const override;

	/** lock */
	void SetPositionLock(const Math::Vector3& positionLock) override;
	const Math::Vector3& GetPositionLock() const override;
	void SetRotationLock(const Math::Vector3& rotationLock) override;
	const Math::Vector3& GetRotationLock() const override;

	/** transform */
	void SetPosition(const Math::Vector3& pos) const override;
	void SetRotation(const Math::Quaternion& rot) const override;

	/** アクセス */
	physx::PxRigidActor* GetRigidActor() noexcept;

private:

	void RegisterToPhysics() noexcept;
	void UnRegisterFromPhysics() noexcept;

private:

	PhysX* m_physics = nullptr;

	bool m_isRegister = false;

	Collider* m_collider = nullptr;

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