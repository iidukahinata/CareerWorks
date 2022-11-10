/**
 * @file	Physics.cpp
 * @brief
 *
 * @date	2022/11/06 2022îNìxèâî≈
 */


#include "Physics.h"

void Physics::RegisterRigidBody(RigidBody* rigidBody)
{
	m_rigidBodys.emplace_back(rigidBody);
}

void Physics::UnRegisterRigidBody(RigidBody* rigidBody)
{
	std::erase(m_rigidBodys, rigidBody);
}

void Physics::SetGravity(const Math::Vector3& gravity)
{
	m_gravity = gravity;
}

const Math::Vector3& Physics::GetGravity() const
{
	return m_gravity;
}