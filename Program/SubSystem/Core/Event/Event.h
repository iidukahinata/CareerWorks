/**
* @file		Event.h
* @brief
*
* @date		2022/09/06 2022�N�x����
*/
#pragma once


using EventType = ClassTypeData;

class IEvent
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(IEvent)
	SUPER_CLASS(IEvent)
public:

	IEvent() = default;
	virtual ~IEvent() = default;

	/** ���X�i�[�I�u�W�F�N�g�ɓn���������f�[�^ */
	virtual std::any GetData() = 0;
};

template<class T, class... Args>
void NotifyEvent(Args... args);
void NotifyEvent(UniquePtr<IEvent> eventBase);

template<class T>
void CancelEvent(bool isAll);
void CancelEvent(const EventType& type, bool isAll);

template<class T, class ...Args>
FORCEINLINE void NotifyEvent(Args ...args)
{
	NotifyEvent(std::make_unique<T>(args...));
}

template<class T>
FORCEINLINE void CancelEvent(bool isAll)
{
	CancelEvent(T::TypeData, isAll);
}