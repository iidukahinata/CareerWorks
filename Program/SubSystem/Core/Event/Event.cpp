/**
* @file		Event.cpp
* @brief
*
* @date		2022/09/01 2022îNìxèâî≈
*/


#include "Event.h"
#include "EventManager.h"

void NotifyEvent(UniquePtr<IEvent> eventBase)
{
	EventManager::Get().AddToQueue(UniquePtr<IEvent>(eventBase.release()));
}

void CancelEvent(const EventType& type, bool isAll)
{
	EventManager::Get().RemoveFromQueue(type, isAll);
}