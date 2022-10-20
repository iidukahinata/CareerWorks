/**
* @file    TickManager.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


class TickFunction;

/**
* �e Tick �������e�N���X�������₷���悤�ɑO������Ȃǂ̏������Ȃ�
* �g�p�o����悤�Ɋȗ��������邽�߂̃N���X
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

	/** �^�X�N���X�g�̎��s�J�n */
	void Tick(double deltaTime) noexcept;

public:

	/** �X�V�������s�����߂ɁA�Ăяo���K�v������܂��B*/
	void RegisterTickFunction(TickFunction* function) noexcept;
	void UnRegisterTickFunction(TickFunction* function) noexcept;

	/** TickFunction �̐ݒ肪�ύX���ꂽ���ɌĂяo���B*/
	void AddTickFunction(TickFunction* function) noexcept;

	/** TickFunction �̐ݒ肪�ύX����O�Ɏ��ɌĂяo���K�v������܂��B*/
	void RemoveTickFunction(TickFunction* function) noexcept;

	/** �X�V�����̂��߂ɓo�^����Ă��邩�𔻕ʁB*/
	bool HasTickFunction(TickFunction* function) noexcept;

private:

	/** Tick �J�n���ɓo�^���ꂽ�S�^�X�N���I���������Ƃ�ۏ؂��� */
	void WaitForTask() noexcept;

	/** Function �ݒ�����ɏ����������߃^�X�N���X�g���쐬 */
	void CreateTaskList(double deltaTime) noexcept;

	/** �ċA���g�p���đO��������l�����ēo�^���� */
	void RegisterToTaskList(TickFunction* function, double deltaTime, Set<TickFunction*>& registeredList) noexcept;

private:

	HANDLE m_hEvent;

	// * �X�V�֐��Z�b�g�p
	Job m_job;

	// * ���U�ł̍X�V�֐���ێ��p
	AsyncJob m_anyThreadTask;

	// * �ʏ퓮��(main thread)�ōX�V������^�X�N�z��B�l�͖��t���[���X�V
	Vector<TickFunction*> m_gameThreadTaskList;

	// * main thread �ȊO�ɕ��U�����čX�V������^�X�N�z��B�l�͖��t���[���X�V
	Vector<TickFunction*> m_anyThreadTaskList;

	// * enable �ݒ��킸�X�V�������s���\�������� TickFunction ��ێ��B
	Set<TickFunction*> m_allTickFunctions;

	// * -> �D��x : �R���e�i (enable �ݒ肳��Ă��� TickFunction �̂ݕێ�)
	Map<uint32_t, TickContainer> m_tickContainers;
};