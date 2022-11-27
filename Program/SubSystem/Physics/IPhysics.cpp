/**
 * @file	Physics.cpp
 * @brief
 *
 * @date	2022/11/06 2022îNìxèâî≈
 */


#include "IPhysics.h"

void IPhysics::RegisterRigidBody(RigidBody* rigidBody)
{
	m_rigidBodys.emplace_back(rigidBody);
}

void IPhysics::UnRegisterRigidBody(RigidBody* rigidBody)
{
	std::erase(m_rigidBodys, rigidBody);
}

void IPhysics::SetGravity(const Math::Vector3& gravity)
{
	m_gravity = gravity;
}

const Math::Vector3& IPhysics::GetGravity() const
{
	return m_gravity;
}