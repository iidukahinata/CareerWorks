/**
* @file    AudioListener.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "AudioListener.h"
#include "SubSystem/Audio/Audio.h"

void AudioListener::Initialize()
{
	m_audio = GetContext()->GetSubsystem<Audio>();

	RegisterToAudioSystem();
}

void AudioListener::Remove()
{
	UnRegisterFromAudioSystem();
}

void AudioListener::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);
	if (active)
	{
		RegisterToAudioSystem();
	}
	else
	{
		UnRegisterFromAudioSystem();
	}
}

void AudioListener::SetVelocity(const Math::Vector3& velocity) noexcept
{
	m_velocity = velocity;
}

const Math::Vector3& AudioListener::GetVelocity() const noexcept
{
	return m_velocity;
}

void AudioListener::RegisterToAudioSystem()
{
	m_audio->SetAudioListener(this);
}

void AudioListener::UnRegisterFromAudioSystem()
{
	m_audio->SetAudioListener(nullptr);
}