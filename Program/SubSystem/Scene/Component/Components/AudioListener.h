/**
* @file    AudioListener.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../IAudioListener.h"

class IAudio;

class AudioListener : public IAudioListener
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(AudioListener, IAudioListener)
public:

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;

public:

	/** 3D Mode ���ɏ�蕨�ɏ���Ă���悤�ȉ����ɂȂ�悤�v�Z����܂��B*/
	void SetVelocity(const Math::Vector3& velocity) override;
	const Math::Vector3& GetVelocity() const override;

private:

	void RegisterToAudioSystem();
	void UnRegisterFromAudioSystem();

private:

	Math::Vector3 m_velocity;

	IAudio* m_audio = nullptr;
};