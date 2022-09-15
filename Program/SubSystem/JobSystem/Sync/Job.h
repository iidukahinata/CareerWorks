/**
* @file		Job.h
* @brief
*
* @date		2022/09/06 2022�N�x����
*/
#pragma once


#include "JobHelper.h"

class Job
{
	COMPLETED_DEVELOPMENT()

	friend class JobSystem;

	typedef std::function<void(double)> Task;
public:

	Job() = default;
	Job(Task&& task, FunctionType type) noexcept;

	~Job();

	void Execute(double deletaTime) noexcept;

	/**
	* JobSystem �Ŏ��s�����֐� function ��ݒ�
	* 
	* @note �����ɂ� double �^���󂯎��K�v������܂��B
	*/
	void SetFunction(Task&& task, FunctionType fincType) noexcept;

	/** �X�V�������s�����߂ɂ́A��x���s���Ȃ���΂Ȃ�Ȃ��B*/
	void RegisterToJobSystem() noexcept;

	/** JobSystem �ɓo�^�����ꍇ�A�Ăяo���K�v������܂��B*/
	void UnRegisterFromJobSystem() noexcept;

private:

	// * �d���`�F�b�N�p
	bool m_isRegistered = false;

	// * JobSystem �Z�b�g����s������֐�
	Task m_task;

	// * �����p�ɏ����R���e�i�̃|�C���^��ێ����܂��B
	FunctionType m_funcType = FunctionType::None;
};