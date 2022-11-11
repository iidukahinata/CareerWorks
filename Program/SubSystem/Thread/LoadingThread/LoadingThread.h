/**
* @file    LoadingThread.h
* @brief
*
* @date	   2022/11/01 2022�N�x����
*/
#pragma once


#include "../Thread.h"

/**
* LoadingThread �ŏ������������ CommandList �� CommandList �ǉ��҂� TaskList ���Ǘ�
* �f�[�^�����j�~�̂��� Command �̒ǉ����� MainThread �ł̃A�N�Z�X�u���b�N�ɂ��҂����Ԃ̔������̖�����̃L���[��p�ӂ�������Ă���B
*/
class LoadingCommandList
{
	COMPLETED_DEVELOPMENT()

	typedef std::function<void()> Command;
public:

	/** GameThread ������A�N�Z�X����� Task �ǉ��֐� */
	static void AddTask(Command&& task) noexcept
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_taskList.push(std::forward<Command>(task));
	}

	/** �o�^ Task �̏����֐� */
	static void FlushTask() noexcept
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		Queue<Command> clearData;
		m_taskList.swap(clearData);
	}

	bool HasTask() noexcept;

	/** �ǉ��҂� TaskList ���� CommandList �� Command �̓���ւ����� */
	void SwapCommand() noexcept;

	/** �O�t���[���Œǉ����ꂽ�R�}���h�����s */
	void CommandProcess() noexcept;

private:

	// * command �� swap ���̔r������Ŏg�p
	static std::mutex m_mutex;

	// * LoadingThread �ŏ������������ CommandList
	static Queue<Command> m_commandList;

	// * CommandList �ǉ��҂��� TaskList
	static Queue<Command> m_taskList;
};

/**
* �g�p�ґ��� LoadingCommand �̒ǉ����s�����߂̃C���^�[�t�F�[�X�֐��B
*/
template<class Func>
void RegisterLoadingCommand(Func&& task) noexcept
{
	LoadingCommandList::AddTask(std::forward<Func>(task));
}

class LoadingThread : public IThread
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(LoadingThread)
public:

	void Run() override;

	bool IsRealTimeThread() const override;

public:

	/** LoadingThread �̗����グ�������s���B*/
	static void Start() noexcept;

	/** LoadingThread �̏I���������s���B*/
	static void Stop() noexcept;

private:

	LoadingCommandList m_commandList;
};

bool IsInLoadingThread() noexcept;