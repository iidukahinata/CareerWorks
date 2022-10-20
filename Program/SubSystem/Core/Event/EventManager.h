/**
* @file    EventManager.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


using EventType = ClassTypeData;

class IEvent;
class EventListener;

class EventManager
{
	COMPLETED_DEVELOPMENT()
	EventManager() = default;
	COPY_PROHIBITED(EventManager)
public:

	static EventManager& Get() noexcept
	{
		static EventManager instance;
		return instance;
	}

	void Initialize() noexcept;
	void Exit() noexcept;

	/** �O�t���[���Ŕ��������C�x���g�̏����B*/
	void Tick() noexcept;

public:

	/** �C�x���g�����t���[�������p�L���[�ɒǉ� */
	bool AddToQueue(UniquePtr<IEvent> eventBase) noexcept;

	/**
	* �擪�ɂ���w��C�x���g�^�C�v�̏���
	* isAll �w��Ő擪���܂߂��S�Ă̎w��C�x���g�^�C�v������
	*/
	void RemoveFromQueue(const EventType& eventType, bool isAll) noexcept;

	/** @param eventType [����] �ʒm�Ή�������C�x���g�^�C�v���w��B*/
	bool AddEventLisener(EventListener* eventListener, const EventType& eventType) noexcept;
	bool RemoveEventLisener(EventListener* eventListener, const EventType& eventType) noexcept;

private:

	/* Event Update �̓o�^�p */
	Job m_job;

	std::mutex m_mutex;

	// * ���ݎg�p���L���[�ԍ�
	uint32_t m_numActiveQueue = 0;

	// * �L���[����̏ꍇ�C�x���g�Ăяo���������I���Ȃ��\�������邽�ߕ����p�ӂ���
	Array<List<UniquePtr<IEvent>>, 2> m_eventQueues;

	// * -> �n�b�V���l : ���X�i�[�|�C���^�z��
	Map<uint32_t, Set<EventListener*>> m_eventListeners;
};