/**
 * @file	PhysicsEvents.cpp
 * @brief
 *
 * @date	2022/11/09 2022�N�x����
 */


#include "PhysicsEvents.h"

CollisionEnterEvent::CollisionEnterEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionEnterEvent::GetData()
{
	return m_hitPair;
}

CollisionStayEvent::CollisionStayEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionStayEvent::GetData()
{
	return m_hitPair;
}

CollisionExitEvent::CollisionExitEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any CollisionExitEvent::GetData()
{
	return m_hitPair;
}

TriggerEnterEvent::TriggerEnterEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerEnterEvent::GetData()
{
	return m_hitPair;
}

TriggerStayEvent::TriggerStayEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerStayEvent::GetData()
{
	return m_hitPair;
}

TriggerExitEvent::TriggerExitEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB) : m_hitPair(rigidBodyA, rigidBodyB)
{
}

std::any TriggerExitEvent::GetData()
{
	return m_hitPair;
}