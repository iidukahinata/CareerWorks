/**
* @file    Audio.h
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class AudioListener;

/**
* Audio���ۃN���X
* ���̃N���X�ł̓C���^�[�t�F�[�X�݂̂������B
* �h���N���X�ŏ��������̊֐�����������B
*/
class IAudio : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudio)
public:

	virtual ~IAudio() {}

	/** 3D �T�E���h���g�p����ꍇ�͕K���l���Z�b�g����K�v������܂��B*/
	void SetAudioListener(AudioListener* listener) noexcept;

protected:

	AudioListener* m_lisrener = nullptr;
};