/**
 * @file	Physics.h
 * @brief
 *
 * @date	2022/11/04 2022�N�x����
 */
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class RigidBody;

class IPhysics : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IPhysics)
public:

	virtual ~IPhysics() = default;

	/** RigidBody ���\�b�h */
	virtual void RegisterRigidBody(RigidBody* rigidBody);
	virtual void UnRegisterRigidBody(RigidBody* rigidBody);

	/** Gravity ���\�b�h */
	virtual void SetGravity(const Math::Vector3& gravity);
	virtual const Math::Vector3& GetGravity() const;

protected:

	Vector<RigidBody*> m_rigidBodys;

	Math::Vector3 m_gravity = Math::Vector3(0.0f, -9.81f, 0.0f);
};