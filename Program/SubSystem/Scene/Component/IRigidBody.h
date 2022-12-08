/**
* @file    IRigidBody.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "IComponent.h"

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

class ICollider;

class IRigidBody : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IRigidBody, IComponent)
public:

	virtual void PreUpdate()
	{
		ASSERT(0);
	}

	virtual void Update()
	{
		ASSERT(0);
	}

public:

	virtual void SetCollider(ICollider* collider)
	{
		ASSERT(0);
	}

	/** body type */
	virtual void SetBodyType(BodyType type)
	{
		ASSERT(0);
	}
	virtual BodyType GetBodyType() const
	{
		ASSERT(0); return {};
	}

	/** force */
	virtual void AddForce(const Math::Vector3& force, ForceMode mode = ForceMode::Force)
	{
		ASSERT(0);
	}
	virtual void AddTorque(const Math::Vector3& torque, ForceMode mode = ForceMode::Force)
	{
		ASSERT(0);
	}

	/** velocity */
	virtual void SetVelocity(const Math::Vector3& velocity)
	{
		ASSERT(0);
	}
	virtual void AddVelocity(const Math::Vector3& addVelocity)
	{
		ASSERT(0);
	}
	virtual Math::Vector3 GetVelocity() const
	{
		ASSERT(0); return {};
	}

	/** angular velocity */
	virtual void SetAngularVelocity(const Math::Vector3& velocity)
	{
		ASSERT(0);
	}
	virtual void AddAngularVelocity(const Math::Vector3& addVelocity)
	{
		ASSERT(0);
	}
	virtual Math::Vector3 GetAngularVelocity() const
	{
		ASSERT(0); return {};
	}

	/** mass */
	virtual void SetMass(float mass)
	{
		ASSERT(0);
	}
	virtual float GetMass() const
	{
		ASSERT(0); return {};
	}

	/** flags */
	virtual void SetUseGravity(bool useGravity)
	{
		ASSERT(0);
	}
	virtual bool UseGravity() const
	{
		ASSERT(0); return {};
	}
	virtual void SetKinematic(bool kinematic)
	{
		ASSERT(0);
	}
	virtual bool IsKinematic() const
	{
		ASSERT(0); return {};
	}
	virtual bool IsDynamic() const
	{
		ASSERT(0); return {};
	}

	/** lock */
	virtual void SetPositionLock(const Math::Vector3& positionLock)
	{
		ASSERT(0);
	}
	virtual const Math::Vector3& GetPositionLock() const
	{
		ASSERT(0); return {};
	}
	virtual void SetRotationLock(const Math::Vector3& rotationLock)
	{
		ASSERT(0);
	}
	virtual const Math::Vector3& GetRotationLock() const
	{
		ASSERT(0); return {};
	}

	/** transform */
	virtual void SetPosition(const Math::Vector3& pos) const
	{
		ASSERT(0);
	}
	virtual void SetRotation(const Math::Quaternion& rot) const
	{
		ASSERT(0);
	}
};