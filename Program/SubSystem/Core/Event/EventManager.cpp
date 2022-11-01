/**
* @file    EventManager.cpp
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/


#include "EventManager.h"
#include "Event.h"
#include "EventListener.h"

void EventManager::Initialize() noexcept
{
	// ���C���X���b�h�W���u�Ƃ��ăC�x���g���[�v��o�^���Ă��邪�A���X���b�h�ŏ������������S���Ă�������������Ȃ��B
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

	// ������ Event ��o�^���Ȃ�����
	if (eventType.Name.empty())
	{
		return false;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// �w��p�̃��X�i�[�R���e�i���Ȃ���Ώ���������̂��Ȃ��̂Œǉ����Ȃ��B
	if (m_eventListeners.contains(eventType.Hash))
	{
		m_eventQueues[m_numActiveQueue].emplace_back(std::move(eventBase));
		return true;
	}
	return false;
}

void EventManager::RemoveFromQueue(const EventType& eventType, bool isAll) noexcept
{
	// ������ Event �͓o�^����Ă��Ȃ�����
	if (eventType.Name.empty())
	{
		return;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// ���X�i�[�R���e�i����������Ă��Ȃ����͌��X�L���[�ɒǉ�����Ă��Ȃ�����
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
	// ������ Event ��o�^���Ȃ�����
	if (eventType.Name.empty())
	{
		return false;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	// ���X�i�[�̏d��������邽��
	if (m_eventListeners[eventType.Hash].contains(eventListener))
	{
		return false;
	}

	m_eventListeners[eventType.Hash].insert(eventListener);

	return true;
}

bool EventManager::RemoveEventLisener(EventListener* eventListener, const EventType& eventType) noexcept
{
	// ������ Event ���������Ȃ�����
	if (eventType.Name.empty())
	{
		return true;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	auto numErase = m_eventListeners[eventType.Hash].erase(eventListener);

	// �������ꂽ���𒲂ׂ邽��
	return numErase > 0;
}

void EventManager::Tick() noexcept
{
	// �L���[�̓���ւ�
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

		// �^�C���A�E�g�����͖��J���B
	}

	// �^�C���A�E�g�������N�����ꍇ�A�c�����^�X�N�����t���[���Ɏ����z���B
	//m_eventQueues[m_numActiveQueue].merge(m_eventQueues[numQueue]);
}