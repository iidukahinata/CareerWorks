/**
* @file    ScriptEngine.h
* @brief
*
* @date	   2022/11/28 2022îNìxèâî≈
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class ScriptEngine : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(ScriptEngine, ISubsystem)
private:

	enum PhysicsEventType
	{
		OnCollisionEnter,

		OnCollisionStay,

		OnCollisionExit,

		OnTriggerEnter,

		OnTriggerStay,

		OnTriggerExit,

		MaxEvent,
	};

public:

	bool Initialize() override;
	void Shutdown() override;

	void RebuildAllScript() noexcept;

private:

	void SetupEventLisneterList();

	template<typename Event, typename Func, typename Type>
	void SetupHitEventLisneter(PhysicsEventType type, Func notifyHit, Type hitType);

private:

	ResourceManager* m_resourceManager = nullptr;
	
	Array<EventListener, PhysicsEventType::MaxEvent> m_eventListenerList;
};

template<typename Event, typename Func, typename Type>
FORCEINLINE void ScriptEngine::SetupHitEventLisneter(ScriptEngine::PhysicsEventType type, Func notifyHit, Type hitType)
{
	m_eventListenerList[type].SetFunction([notifyHit, hitType](std::any data) {

		auto hitInfo = std::any_cast<HitPairInfo>(data);

		notifyHit(hitType, hitInfo.rigidBodyA, hitInfo.rigidBodyB);
		notifyHit(hitType, hitInfo.rigidBodyB, hitInfo.rigidBodyA);
	});

	m_eventListenerList[type].RegisterToEventManager<Event>();
}