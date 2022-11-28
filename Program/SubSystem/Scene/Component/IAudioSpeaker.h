/**
* @file    IAudioSpeaker.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "IComponent.h"

class AudioClip;

class IAudioSpeaker : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudioSpeaker, IComponent)
public:

	/** �o�^���ꂽ AudioClip ���g�p���ĉ������Đ����܂��B*/
	virtual void Play()
	{
		ASSERT(0);
	}

	/** SE �Ȃǂ̓����ݒ�ŕ����̉������Đ�����鎞�ȂǂɎg�p�B*/
	virtual void PlayOneShot(AudioClip* audioClip, float volume = 1.f) const
	{
		ASSERT(0);
	}

	/** �o�^�������Đ�����Ă���ꍇ�̂ݏ������s���B*/
	virtual void Pause() const
	{
		ASSERT(0);
	}

	/** �o�^������ Pause ���̂ݏ������s���B*/
	virtual void UnPause() const
	{
		ASSERT(0);
	}

	/** �o�^���ꂽ AudioClip ���g�p���ĉ����𒆎~���܂��B*/
	virtual void Stop()
	{
		ASSERT(0);
	}

public:

	/** playOnAwake = true �̎��o�^�Ɠ����ɉ������Đ����܂��B*/
	virtual void SetAudioClip(AudioClip* audioClip)
	{
		ASSERT(0);
	}
	virtual AudioClip* GetAudioClip() const
	{
		ASSERT(0); return {};
	}

	virtual void SetPlayOnAwake(bool playOnAwake)
	{
		ASSERT(0);
	}
	virtual bool GetPlayOnAwake() const
	{
		ASSERT(0); return {};
	}

	virtual void SetMute(bool mute)
	{
		ASSERT(0);
	}
	virtual bool GetMute() const
	{
		ASSERT(0); return {};
	}

	virtual void SetIsLoop(bool isLoop)
	{
		ASSERT(0);
	}
	virtual bool GetIsLoop() const
	{
		ASSERT(0); return {};
	}

	/* 0 ����ԗD��x�������A�����l�� 0 ���� 256 �̊Ԃ� clamp ����܂��B*/
	virtual void SetPriority(int priority)
	{
		ASSERT(0);
	}
	virtual int GetPriority() const
	{
		ASSERT(0); return {};
	}

	/* 1 ����ԉ����傫���A�����l�� 0 ���� 1 �̊Ԃ� clamp ����܂��B*/
	virtual void SetVolume(float volume)
	{
		ASSERT(0);
	}
	virtual float GetVolume() const
	{
		ASSERT(0); return {};
	}

	/* 2 ����ԉ��������A�����l�� 0 ���� 2 �̊Ԃ� clamp ����܂��B*/
	virtual void SetPitch(float pitch)
	{
		ASSERT(0);
	}
	virtual float GetPitch() const
	{
		ASSERT(0); return {};
	}

	/* 0 �����E�����o�����X�ɂȂ�A�����l�� -1 ���� 1 �̊Ԃ� clamp ����܂��B*/
	virtual void SetPan(float pan)
	{
		ASSERT(0);
	}
	virtual float GetPan() const
	{
		ASSERT(0); return {};
	}

	/* true �ɂ���� 3D �����𖳎����܂��B*/
	virtual void SetIs2DMode(bool is2DMode)
	{
		ASSERT(0);
	}
	virtual bool GetIs2DMode() const
	{
		ASSERT(0); return {};
	}

	/* 	�T�E���h�̌������~�����鋗�� */
	virtual void SetMaxDistance(float max)
	{
		ASSERT(0);
	}
	virtual float GetMaxDistance() const
	{
		ASSERT(0); return {};
	}

	/* �X�s�[�J�[�̉����ʂŒ�������֐��ł��B*/
	virtual void SetOutPutMatrix(const Array<float, 8>& matrix)
	{
		ASSERT(0);
	}
	virtual const Array<float, 8>& GetOutPutMatrix() const
	{
		ASSERT(0); return {};
	}
};