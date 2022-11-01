/**
* @file    EventManager.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "EventManager.h"
#include "Event.h"
#include "EventListener.h"

void EventManager::Initialize() noexcept
{
	// メインスレッドジョブとしてイベントループを登録しているが、他スレッドで処理し処理分担してもいいかもしれない。
	m_job.SetFunction([this](double) { Tick(); }, FunctionType::Update);
	m_job.RegisterToJobSystem();
}

void EventManager::Exit() noexcept
{
	m_job.UnRegisterFromJobSystem();

	for (auto eventListeners : m_eventListeners)
	{
		for (auto listener : eventListeners.second)
		{
			listener->UnRegisterFromEventManager();
		}
	}

	m_eventListeners.clear();

	for (size_t i = 0; i < m_eventQueues.max_size(); ++i)
	{
		m_eventQueues[i].clear();
	}

	m_numActiveQueue = 0;
}

bool EventManager::AddToQueue(UniquePtr<IEvent> eventBase) noexcept
{
	auto& eventType = eventBase->GetTypeData();

	// 無効な Event を登録しないため
	if (eventType.Name.empty())
	{
		return false;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// 指定用のリスナーコンテナがなければ処理するものがないので追加しない。
	if (m_eventListeners.contains(eventType.Hash))
	{
		m_eventQueues[m_numActiveQueue].emplace_back(std::move(eventBase));
		return true;
	}
	return false;
}

void EventManager::RemoveFromQueue(const EventType& eventType, bool isAll) noexcept
{
	// 無効な Event は登録されていないため
	if (eventType.Name.empty())
	{
		return;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// リスナーコンテナが生成されていない時は元々キューに追加されていないため
	if (!m_eventListeners.contains(eventType.Hash))
	{
		return;
	}

	auto& eventQueue = m_eventQueues[m_numActiveQueue];
	for (auto it = eventQueue.begin(); it != eventQueue.end();)
	{
		if ((*it)->GetTypeData() == eventType)
		{
			it = eventQueue.erase(it);
			if (!isAll) break;
		}
		else
		{
			it++;
		}
	}
}

bool EventManager::AddEventLisener(EventListener* eventListener, const EventType& eventType) noexcept
{
	// 無効な Event を登録しないため
	if (eventType.Name.empty())
	{
		return false;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// リスナーの重複を避けるため
	if (m_eventListeners[eventType.Hash].contains(eventListener))
	{
		return false;
	}

	m_eventListeners[eventType.Hash].insert(eventListener);

	return true;
}

bool EventManager::RemoveEventLisener(EventListener* eventListener, const EventType& eventType) noexcept
{
	// 無効な Event を検索しないため
	if (eventType.Name.empty())
	{
		return true;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	auto numErase = m_eventListeners[eventType.Hash].erase(eventListener);

	// 消去されたかを調べるため
	return numErase > 0;
}

void EventManager::Tick() noexcept
{
	// キューの入れ替え
	auto numQueue = m_numActiveQueue;
	(++m_numActiveQueue) %= m_eventQueues.max_size();
	m_eventQueues[m_numActiveQueue].clear();

	while (m_eventQueues[numQueue].size() != 0)
	{
		auto& event = m_eventQueues[numQueue].front();

		auto listeners = m_eventListeners[event->GetTypeData().Hash];
		for (auto listener : listeners)
		{
			listener->Action(event->GetData());
		}

		m_eventQueues[numQueue].pop_front();

		// タイムアウト処理は未開発。
	}

	// タイムアウト処理が起きた場合、残ったタスクを次フレームに持ち越す。
	//m_eventQueues[m_numActiveQueue].merge(m_eventQueues[numQueue]);
}