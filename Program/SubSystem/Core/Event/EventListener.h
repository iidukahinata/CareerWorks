/**
* @file    EventListener.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


using EventType = ClassTypeData;

class EventListener
{
	COMPLETED_DEVELOPMENT()
	CLASS_DATA(EventListener)

	typedef std::function<void(std::any)> Func;
public:

	EventListener() = default;
	EventListener(Func&& func);
	~EventListener();

public:

	/** Event �擾�֐��̎��s���s���B*/
	void Action(std::any data) noexcept;

	/** EventManager �Ŏ��s�����֐� function ��ݒ� */
	void SetFunction(Func&& func) noexcept;

public:

	/** Event���󂯎���悤�ɂ��邽�� */
	template<class T>
	void RegisterToEventManager();
	void RegisterToEventManager(const EventType& type) noexcept;

	/** �o�^���Ă���ꍇ�͌Ăяo���K�v������܂��B*/
	void UnRegisterFromEventManager() noexcept;

private:

	// * UnRegister ���g�p
	String m_eventName;

	// * Listener �֐�
	Func m_function;
};

template<class T>
FORCEINLINE void EventListener::RegisterToEventManager()
{
	RegisterToEventManager(T::TypeData);
}