/**
* @file    PhysicsEvents.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


class RigidBody;

struct HitPairInfo
{
	RigidBody* rigidBodyA;
	RigidBody* rigidBodyB;

	HitPairInfo() = default;
	HitPairInfo(RigidBody* bodyA, RigidBody* bodyB) : rigidBodyA(bodyA), rigidBodyB(bodyB)
	{

	}
};

class CollisionEnterEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionEnterEvent, IEvent)
public:

	CollisionEnterEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};

class CollisionStayEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionStayEvent, IEvent)
public:

	CollisionStayEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};

class CollisionExitEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(CollisionExitEvent, IEvent)
public:

	CollisionExitEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};

class TriggerEnterEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerEnterEvent, IEvent)
public:

	TriggerEnterEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};

class TriggerStayEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerStayEvent, IEvent)
public:

	TriggerStayEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};

class TriggerExitEvent : public IEvent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(TriggerExitEvent, IEvent)
public:

	TriggerExitEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};