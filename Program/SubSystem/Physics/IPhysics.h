/**
 * @file	IPhysics.h
 * @brief
 *
 * @date	2022/11/28 2022�N�x����
 */
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class IRigidBody;

class IPhysics : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IPhysics, ISubsystem)
public:

	virtual ~IPhysics() = default;

	/** RigidBody ���\�b�h */
	virtual void RegisterRigidBody(IRigidBody* rigidBody);
	virtual void UnRegisterRigidBody(IRigidBody* rigidBody);

	/** Gravity ���\�b�h */
	virtual void SetGravity(const Math::Vector3& gravity);
	virtual const Math::Vector3& GetGravity() const;

protected:

	Vector<IRigidBody*> m_rigidBodys;

	Math::Vector3 m_gravity = Math::Vector3(0.0f, -9.81f, 0.0f);
};