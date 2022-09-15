/**
 * @file	Timer.h
 * @brief   �t���[���Ǘ��N���X
 *
 * @date	2022/09/06 2022�N�x����
 */
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class Timer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Timer)
public:

	bool Initialize() override;

	/**
	* �o�ߎ��ԂƐݒ肳�ꂽ FPS ����t���[���̏�Ԃ�Ԃ��B
	* 2022/04/24���_�ł͐��x�������邽�߁A�c��t���[���҂����Ԃł̑҂����������Ă��Ȃ��B
	*/
	bool ReachedNextFrame() noexcept;

	/** ���f���ꂽ�A�v���P�[�V�����ĊJ���Ɏ��Ԓ����̂��߂Ɏg�p�B*/
	void ResetMeasurement() noexcept;

	/** FPS �l �ύX���Ɏg�p */
	void SetFPS(float fps) noexcept;

	/** �Ō�� ReachedNextFrame �֐��ōX�V���ꂽ���_�̌o�ߎ��Ԃ�Ԃ� */
	double GetDeltaTime() const noexcept;

private:

	// * �ŐV�t���[���X�V����
	std::chrono::high_resolution_clock::time_point m_previousTime;

	// * �t���[���o�ߎ���(�b)
	double m_deltaTime;

	// * �ݒ蒆��FPS�l��b�ɕϊ������l
	double m_fpsTime;
};