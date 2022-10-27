/**
* @file		EventListener.cpp
* @brief
*
* @date		2022/09/05 2022îNìxèâî≈
*/


#include "EventListener.h"
#include "EventManager.h"

EventListener::EventListener(Func&& func)
{
	SetFunction(std::forward<Func>(func));
}

EventListener::~EventListener()
{
	UnRegisterFromEventManager();
}

void EventListener::Action(std::any data) noexcept
{
	m_function(data);
}

void EventListener::SetFunction(Func&& func) noexcept
{
	m_function = std::forward<Func>(func);
}

void EventListener::RegisterToEventManager(const EventType& type) noexcept
{
	m_eventName = type.Name;
	EventManager::Get().AddEventLisener(this, type);
}

void EventListener::UnRegisterFromEventManager() noexcept
{
	if (m_eventName.empty())
	{
		return;
	}

	EventManager::Get().RemoveEventLisener(this, EventType(m_eventName));
	m_eventName = StringView();
}