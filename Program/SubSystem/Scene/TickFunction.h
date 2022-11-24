/**
* @file    TickFunction.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


class TickFunction
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(TickFunction)
public:

	TickFunction() = default;
	virtual ~TickFunction() = default;

	virtual void Tick(double deltaTime) = 0;

	/** �X�V�������s�����߂ɂ́A��x���s���Ȃ���΂Ȃ�Ȃ��B*/
	void RegisterToTickManager() noexcept;

	/** TickManager �ɓo�^�����ꍇ�A�I�����Ăяo���K�v������܂��B*/
	void UnRegisterFromTickManager() noexcept;

	/** �X�V�����̑ҋ@�A���s���Ȃ� true ��Ԃ��B*/
	bool IsRegistered() const noexcept;

public:

	/** TickManager �ւ̍X�V�͊֐����ōs�����߂͌Ăяo��������͕s�v */
	void SetEnable(bool enable) noexcept;
	bool GetEnable() noexcept;

	/** ���̐ݒ�͑O������Ɏw�肳��Ă��� TickFunction �̐ݒ�ɂ��e�����܂��B */
	void SetPriority(uint32_t priority) noexcept;
	uint32_t GetPriority() const noexcept;

	/** true �w�肳�ꂽ�ꍇ�AAnyThread �Ŏ��s�����悤�ɂȂ�܂� */
	void SetThreadSafe(bool isThreadSafe) noexcept;
	bool IsThreadSafe() const noexcept;

	/** ���s���g�p�����o�ߎ��Ԃ��Z�b�g�B*/
	void SetDeletaTime(double deltaTime) noexcept;
	double GetDeletaTime() const noexcept;

	/** ���s�O�̑O������ݒ� */
	void AddPrerequisite(TickFunction* tickFunction) noexcept;
	void RemovePrerequisite(TickFunction* tickFunction) noexcept;
	const Vector<TickFunction*>& GetPrerequisites() const noexcept;

private:

	// * ���X���b�h���쎞�Ɏg�p
	double m_deltaTime = 0.f;

	// * ���X���b�h�Ŏ��s�o���邩
	bool m_isThreadSafe = false;

	// * �X�V�����D��x 0 �ɋ߂��l���D�悳���B
	uint32_t m_priority = 128;

	// * �X�V�����̗L������ێ�
	bool m_isEnabled = true;

	// * TickManager �ɓo�^����Ă��邩��ێ�
	bool m_isRegistered = false;

	// * �ݒ肳�ꂽ�O��������g�p�����������쐬���邽��
	Vector<TickFunction*> m_prerequisites;
};

/**
* �Q�[���I�u�W�F�N�g�X�V�I�u�W�F�N�g
*/
class TickGameObjectFunction : public TickFunction
{
public:

	class GameObject* m_gameObject;

public:

	void Tick(double deltaTime);
};

/**
* �R���|�[�l���g�X�V�I�u�W�F�N�g
*/
class TickComponentFunction : public TickFunction
{
public:

	class IComponent* m_component;

public:

	void Tick(double deltaTime);
};