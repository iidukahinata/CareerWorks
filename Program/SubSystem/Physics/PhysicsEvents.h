/**
* @file    PhysicsEvents.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


class IRigidBody;

struct HitPairInfo
{
	IRigidBody* rigidBodyA;
	IRigidBody* rigidBodyB;

	HitPairInfo() = default;
	HitPairInfo(IRigidBody* bodyA, IRigidBody* bodyB) : rigidBodyA(bodyA), rigidBodyB(bodyB)
	{

	}
};

class CollisionEnterEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionEnterEvent, IEvent)
public:

	CollisionEnterEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};

class CollisionStayEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionStayEvent, IEvent)
public:

	CollisionStayEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};

class CollisionExitEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionExitEvent, IEvent)
public:

	CollisionExitEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};

class TriggerEnterEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerEnterEvent, IEvent)
public:

	TriggerEnterEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};

class TriggerStayEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerStayEvent, IEvent)
public:

	TriggerStayEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};

class TriggerExitEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerExitEvent, IEvent)
public:

	TriggerExitEvent(IRigidBody* rigidBodyA, IRigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * �q�b�g�I�u�W�F�N�g
	HitPairInfo m_hitPair;
};