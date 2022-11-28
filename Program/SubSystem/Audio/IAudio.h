/**
* @file    IAudio.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class IAudioListener;

/**
* Audio���ۃN���X
* ���̃N���X�ł̓C���^�[�t�F�[�X�݂̂������B
* �h���N���X�ŏ��������̊֐�����������B
*/
class IAudio : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudio, ISubsystem)
public:

	virtual ~IAudio() {}

	/** 3D �T�E���h���g�p����ꍇ�͕K���l���Z�b�g����K�v������܂��B*/
	void SetAudioListener(IAudioListener* listener) noexcept;

protected:

	IAudioListener* m_lisrener = nullptr;
};