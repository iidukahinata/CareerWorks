/**
 * @file	IPhysics.cpp
 * @brief
 *
 * @date	2022/11/28 2022îNìxèâî≈
 */


#include "IPhysics.h"

void IPhysics::RegisterRigidBody(IRigidBody* rigidBody)
{
	m_rigidBodys.emplace_back(rigidBody);
}

void IPhysics::UnRegisterRigidBody(IRigidBody* rigidBody)
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