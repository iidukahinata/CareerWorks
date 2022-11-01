/**
* @file    RenderingThread.h
* @brief
*
* @date	   2022/11/01 2022�N�x����
*/
#pragma once


#include "../Thread.h"
#include "RenderCommandFance.h"

/**
* RenderingThread �ŏ������������ CommandList �� CommandList �ǉ��҂� TaskList ���Ǘ�
* �f�[�^�����j�~�̂��� Command �̒ǉ����� MainThread �ł̃A�N�Z�X�u���b�N�ɂ��҂����Ԃ̔������̖�����̃L���[��p�ӂ�������Ă���B
*/
class RenderCommandList
{
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

	// * RenderingThread �ŏ������������ CommandList
	static Queue<Command> m_commandList;

	// * CommandList �ǉ��҂��� TaskList
	static Queue<Command> m_taskList;
};

/**
* �g�p�ґ��� RenderingCommand �̒ǉ����s�����߂̃C���^�[�t�F�[�X�֐��B
*/
template<class Func>
void RegisterRenderCommand(Func&& task) noexcept
{
	RenderCommandList::AddTask(std::forward<Func>(task));
}

class RenderingThread : public IThread
{
	SUB_CLASS(RenderingThread)
public:

	/** RenderingThread �̗����グ�������s���B*/
	static void Start() noexcept;

	/** RenderingThread �̏I���������s���B*/
	static void Stop() noexcept;

	/**
	* GemaThread �J�n���ɌĂяo�����B
	* RenderingThread �ł̊J�n�����̃R�}���h�Z�b�g���s���B
	*/
	static void BegineFrame() noexcept;

	/**
	* GemaThread �I�����ɌĂяo�����B
	* RenderingThread �ł̏I�������̃R�}���h�Z�b�g���s���B
	*/
	static void EndFrame() noexcept;

	/**
	* RenderingThread �ŏ��������R�}���h�������I������܂őҋ@����B
	* ��ɁA�A�v���P�[�V�����I�����ȂǂɎg�p����B
	*/
	static void WiatForRenderCommand() noexcept;

	void Run() override;

private:

	RenderCommandList m_renderCommandList;

	static RenderCommandFance m_renderCommandFance;
};

bool IsRenderingThread() noexcept;