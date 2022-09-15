/**
* @file		AsyncJob.h
* @brief
*
* @date		2022/09/06 2022年度初版
*/
#pragma once


class AsyncJob
{
	COMPLETED_DEVELOPMENT()

	typedef std::function<void()> Task;
public:

	AsyncJob() = default;
	AsyncJob(Task&& task) noexcept;

	AsyncJob(const AsyncJob&);
	AsyncJob& operator=(const AsyncJob&);

	void Execute() noexcept;

	/** 他スレッドでタスクが実行->終了するまで待機。*/
	void WaitForFinish() noexcept;

	/** JobSystem で実行される関数 function を設定。*/
	void SetFunction(Task&& task) noexcept;

	/** JobSystem で実行されるメンバ関数 function を設定。*/
	template<class T, class Func>
	void SetFunction(T* ptr, Func task) noexcept;

	/** 実行する処理をタスクに追加。*/
	void RegisterToJobSystem();

public:

	/** 他スレッドでの処理が終了している保証 */
	bool IsTaskFinish() const noexcept;

private:

	// * 非同期実行させるタスク処理。
	Task m_task;

	// * タスク処理が完了したかを保持する。
	std::atomic_bool m_finish = true;
};

template<class T, class Func>
FORCEINLINE void AsyncJob::SetFunction(T* ptr, Func task) noexcept
{
	m_task = std::bind(&task, ptr);
}