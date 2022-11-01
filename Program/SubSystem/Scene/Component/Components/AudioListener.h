/**
* @file    AudioListener.h
* @brief
*
* @date	   2022/10/02 2022�N�x����
*/
#pragma once


#include "../IComponent.h"

class Audio;

class AudioListener : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(AudioListener)
public:

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;

public:

	/** 3D Mode ���ɏ�蕨�ɏ���Ă���悤�ȉ����ɂȂ�悤�v�Z����܂��B*/
	void SetVelocity(const Math::Vector3& velocity) noexcept;
	const Math::Vector3& GetVelocity() const noexcept;

private:

	void RegisterToAudioSystem();
	void UnRegisterFromAudioSystem();

private:

	Math::Vector3 m_velocity;

	Audio* m_audio = nullptr;
};