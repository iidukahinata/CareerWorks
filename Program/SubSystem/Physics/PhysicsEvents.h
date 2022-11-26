/**
* @file    PhysicsEvents.h
* @brief
*
* @date	   2022/11/09 2022年度初版
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
	SUB_CLASS(CollisionEnterEvent)
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
	SUB_CLASS(CollisionStayEvent)
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
	SUB_CLASS(CollisionExitEvent)
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
	SUB_CLASS(TriggerEnterEvent)
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
	SUB_CLASS(TriggerStayEvent)
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
	SUB_CLASS(TriggerExitEvent)
public:

	TriggerExitEvent(RigidBody* rigidBodyA, RigidBody* rigidBodyB);
	std::any GetData() override;

private:

	// * ヒットオブジェクト
	HitPairInfo m_hitPair;
};