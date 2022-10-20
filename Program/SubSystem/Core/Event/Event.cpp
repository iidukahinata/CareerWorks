/**
* @file		Event.cpp
* @brief
*
* @date		2022/10/03 2022�N�x����
*/


#include "Event.h"
#include "EventManager.h"

void NotifyEvent(UniquePtr<IEvent> eventBase)
{
	EventManager::Get().AddToQueue(std::move(eventBase));
}

void CancelEvent(const EventType& type, bool isAll)
{
	EventManager::Get().RemoveFromQueue(type, isAll);
}