/**
* @file    Thread.h
* @brief
*
* @date	   2022/11/01 2022年度初版
*/
#pragma once


class IThread
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(IThread)
	COPY_PROHIBITED(IThread)
public:

	IThread() = default;
	~IThread() {};

	virtual void Run() = 0;

	/** Run 関数の Loop 処理を終了させるために実装 */
	void RequestQuit() noexcept;

	bool IsRequestQuit() const noexcept;

	virtual bool IsRealTimeThread() const { return false; };

private:

	// * 派生先 Run 関数内で Loop 処理を記述した際に使用し、MainThreadとの連携をとる(主に終了時)
	std::atomic_bool m_isRequestQuit = false;
};