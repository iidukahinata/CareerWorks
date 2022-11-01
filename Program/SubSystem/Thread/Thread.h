/**
* @file    Thread.h
* @brief
*
* @date	   2022/11/01 2022�N�x����
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

	/** Run �֐��� Loop �������I�������邽�߂Ɏ��� */
	void RequestQuit() noexcept;

	bool IsRequestQuit() const noexcept;

	virtual bool IsRealTimeThread() const { return false; };

private:

	// * �h���� Run �֐����� Loop �������L�q�����ۂɎg�p���AMainThread�Ƃ̘A�g���Ƃ�(��ɏI����)
	std::atomic_bool m_isRequestQuit = false;
};