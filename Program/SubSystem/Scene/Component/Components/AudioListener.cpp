/**
* @file    AudioListener.cpp
* @brief
*
* @date	   2022/10/02 2022”N“x‰”Å
*/


#include "AudioListener.h"
#include "SubSystem/Audio/Audio.h"

void AudioListener::OnInitialize()
{
	m_audio = GetContext()->GetSubsystem<Audio>();
}

void AudioListener::OnRegister()
{
	IComponent::OnRegister();

	RegisterToAudioSystem();
}

void AudioListener::OnUnRegister()
{
	IComponent::OnUnRegister();

	UnRegisterFromAudioSystem();
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
	ASSERT(m_audio);
	m_audio->SetAudioListener(this);
}

void AudioListener::UnRegisterFromAudioSystem()
{
	ASSERT(m_audio);
	m_audio->SetAudioListener(nullptr);
}