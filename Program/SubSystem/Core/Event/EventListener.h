/**
* @file    EventListener.h
* @brief
*
* @date	   2022/10/03 2022年度初版
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

	/** Event 取得関数の実行を行う。*/
	void Action(std::any data) noexcept;

	/** EventManager で実行される関数 function を設定 */
	void SetFunction(Func&& func) noexcept;

public:

	/** Eventを受け取れるようにするため */
	template<class T>
	void RegisterToEventManager();
	void RegisterToEventManager(const EventType& type) noexcept;

	/** 登録している場合は呼び出す必要があります。*/
	void UnRegisterFromEventManager() noexcept;

private:

	// * UnRegister 時使用
	String m_eventName;

	// * Listener 関数
	Func m_function;
};

template<class T>
FORCEINLINE void EventListener::RegisterToEventManager()
{
	RegisterToEventManager(T::TypeData);
}