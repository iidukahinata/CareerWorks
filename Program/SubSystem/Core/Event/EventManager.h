/**
* @file    EventManager.h
* @brief
*
* @date	   2022/10/03 2022年度初版
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

	/** 前フレームで発生したイベントの処理。*/
	void Tick() noexcept;

public:

	/** イベントを次フレーム処理用キューに追加 */
	bool AddToQueue(UniquePtr<IEvent> eventBase) noexcept;

	/**
	* 先頭にある指定イベントタイプの消去
	* isAll 指定で先頭を含めた全ての指定イベントタイプを消去
	*/
	void RemoveFromQueue(const EventType& eventType, bool isAll) noexcept;

	/** @param eventType [入力] 通知対応させるイベントタイプを指定。*/
	bool AddEventLisener(EventListener* eventListener, const EventType& eventType) noexcept;
	bool RemoveEventLisener(EventListener* eventListener, const EventType& eventType) noexcept;

private:

	/* Event Update の登録用 */
	Job m_job;

	std::mutex m_mutex;

	// * 現在使用中キュー番号
	uint32_t m_numActiveQueue = 0;

	// * キューが一つの場合イベント呼び出し処理が終わらない可能性があるため複数用意する
	Array<List<UniquePtr<IEvent>>, 2> m_eventQueues;

	// * -> ハッシュ値 : リスナーポインタ配列
	Map<uint32_t, Set<EventListener*>> m_eventListeners;
};