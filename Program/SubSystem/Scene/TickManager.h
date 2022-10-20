/**
* @file    TickManager.h
* @brief
*
* @date	   2022/10/03 2022年度初版
*/
#pragma once


class TickFunction;

/**
* 各 Tick 処理を各クラスが扱いやすいように前提条件などの処理順など
* 使用出来るように簡略化させるためのクラス
*/
class TickManager
{
	COMPLETED_DEVELOPMENT()
	TickManager() = default;
	COPY_PROHIBITED(TickManager)

	typedef Set<TickFunction*> TickContainer;
public:

	static TickManager& Get()
	{
		static TickManager instance;
		return instance;
	}

	void Initialize() noexcept;
	void Exit() noexcept;

	/** タスクリストの実行開始 */
	void Tick(double deltaTime) noexcept;

public:

	/** 更新処理を行うために、呼び出す必要があります。*/
	void RegisterTickFunction(TickFunction* function) noexcept;
	void UnRegisterTickFunction(TickFunction* function) noexcept;

	/** TickFunction の設定が変更された時に呼び出す。*/
	void AddTickFunction(TickFunction* function) noexcept;

	/** TickFunction の設定が変更する前に時に呼び出す必要があります。*/
	void RemoveTickFunction(TickFunction* function) noexcept;

	/** 更新処理のために登録されているかを判別。*/
	bool HasTickFunction(TickFunction* function) noexcept;

private:

	/** Tick 開始時に登録された全タスクが終了したことを保証する */
	void WaitForTask() noexcept;

	/** Function 設定を元に処理順を決めタスクリストを作成 */
	void CreateTaskList(double deltaTime) noexcept;

	/** 再帰を使用して前提条件を考慮して登録する */
	void RegisterToTaskList(TickFunction* function, double deltaTime, Set<TickFunction*>& registeredList) noexcept;

private:

	HANDLE m_hEvent;

	// * 更新関数セット用
	Job m_job;

	// * 分散での更新関数を保持用
	AsyncJob m_anyThreadTask;

	// * 通常動作(main thread)で更新させるタスク配列。値は毎フレーム更新
	Vector<TickFunction*> m_gameThreadTaskList;

	// * main thread 以外に分散させて更新させるタスク配列。値は毎フレーム更新
	Vector<TickFunction*> m_anyThreadTaskList;

	// * enable 設定問わず更新処理を行う可能性がある TickFunction を保持。
	Set<TickFunction*> m_allTickFunctions;

	// * -> 優先度 : コンテナ (enable 設定されている TickFunction のみ保持)
	Map<uint32_t, TickContainer> m_tickContainers;
};