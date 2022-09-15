/**
* @file		AsyncJob.h
* @brief
*
* @date		2022/09/06 2022�N�x����
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

	/** ���X���b�h�Ń^�X�N�����s->�I������܂őҋ@�B*/
	void WaitForFinish() noexcept;

	/** JobSystem �Ŏ��s�����֐� function ��ݒ�B*/
	void SetFunction(Task&& task) noexcept;

	/** JobSystem �Ŏ��s����郁���o�֐� function ��ݒ�B*/
	template<class T, class Func>
	void SetFunction(T* ptr, Func task) noexcept;

	/** ���s���鏈�����^�X�N�ɒǉ��B*/
	void RegisterToJobSystem();

public:

	/** ���X���b�h�ł̏������I�����Ă���ۏ� */
	bool IsTaskFinish() const noexcept;

private:

	// * �񓯊����s������^�X�N�����B
	Task m_task;

	// * �^�X�N������������������ێ�����B
	std::atomic_bool m_finish = true;
};

template<class T, class Func>
FORCEINLINE void AsyncJob::SetFunction(T* ptr, Func task) noexcept
{
	m_task = std::bind(&task, ptr);
}