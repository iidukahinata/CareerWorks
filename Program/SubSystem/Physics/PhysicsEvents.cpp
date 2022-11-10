/**
 * @file	PhysicsEvents.cpp
 * @brief
 *
 * @date	2022/11/09 2022îNìxèâî≈
 */


#include "PhysicsEvents.h"

CollisionEnterEvent::CollisionEnterEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionEnterEvent::GetData()
{
	return m_hitPair;
}

CollisionStayEvent::CollisionStayEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionStayEvent::GetData()
{
	return m_hitPair;
}

CollisionExitEvent::CollisionExitEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionExitEvent::GetData()
{
	return m_hitPair;
}

TriggerEnterEvent::TriggerEnterEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerEnterEvent::GetData()
{
	return m_hitPair;
}

TriggerStayEvent::TriggerStayEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerStayEvent::GetData()
{
	return m_hitPair;
}

TriggerExitEvent::TriggerExitEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerExitEvent::GetData()
{
	return m_hitPair;
}