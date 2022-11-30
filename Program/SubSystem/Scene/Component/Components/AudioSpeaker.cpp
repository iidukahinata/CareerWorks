/**
* @file    AudioSpeaker.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include <fmod.hpp>
#include "AudioSpeaker.h"
#include "SubSystem/Audio/IAudio.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Audio/AudioClip.h"

void AudioSpeaker::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);
	
	file->Write(m_playOnAwake);
	file->Write(m_mute);
	file->Write(m_isLoop);
	file->Write(m_priority);
	file->Write(m_volume);
	file->Write(m_pitch);
	file->Write(m_pan);
	file->Write(m_is2DMode);
	file->Write(m_maxDistance);
	file->Write(m_levelMatrix);

	if (m_audioClip)
	{
		file->Write(m_audioClip->GetFilePath());
	}
	else
	{
		file->Write(String());
	}
}

void AudioSpeaker::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	file->Read(&m_playOnAwake);
	file->Read(&m_mute);
	file->Read(&m_isLoop);
	file->Read(&m_priority);
	file->Read(&m_volume);
	file->Read(&m_pitch);
	file->Read(&m_pan);
	file->Read(&m_is2DMode);
	file->Read(&m_maxDistance);
	file->Read(&m_levelMatrix);

	String audioPath;
	file->Read(&audioPath);

	if (audioPath.empty())
	{
		return;
	}

	if (auto resourceManager = GetContext()->GetSubsystem<ResourceManager>())
	{
		auto resourceData = resourceManager->GetResourceData(audioPath);
		auto resource = resourceManager->GetResource(resourceData);

		SetAudioClip(dynamic_cast<AudioClip*>(resource));
	}
}

void AudioSpeaker::OnStart()
{
	IComponent::OnStart();

	if (m_playOnAwake)
	{
		Play();
	}

	m_tickFunction.RegisterToTickManager();
}

void AudioSpeaker::OnStop()
{
	IComponent::OnStop();

	Stop();

	m_tickFunction.UnRegisterFromTickManager();
}

void AudioSpeaker::Tick(double deltaTime)
{
	// 仮置き
	auto rot = GetTransform().GetRotation();
	GetTransform().SetRotation(rot + Math::Vector3(0, 0.03, 0));

	Math::Vector3 worldPos = GetTransform().GetWoldPosition();
	m_velocity = worldPos - m_oldPos;

	if (m_audioClip)
	{
		m_audioClip->SetAttributes(worldPos, m_velocity);
	}

	m_oldPos = worldPos;
}

void AudioSpeaker::Play()
{
	ASSERT(m_audioClip);

	if (m_isPlaying)
		Stop();

	m_audioClip->Play();

	// thisの設定をaudioDateに渡す
	MakeAudioSettingsTheSame(m_audioClip);

	m_isPlaying = true;
}

void AudioSpeaker::PlayOneShot(AudioClip* audioClip, float volume /* = 1.f */) const
{
	ASSERT(audioClip);

	audioClip->Play();

	// thisの設定をaudioDateに渡す
	MakeAudioSettingsTheSame(audioClip);

	audioClip->SetVolume(volume);

	// 3D 時、初期値として再生位置をセット。
	if (m_is2DMode == false)
	{
		auto worldPos = GetTransform().GetWoldPosition();
		audioClip->SetAttributes(worldPos, m_velocity);
	}
}

void AudioSpeaker::Pause() const
{
	if (m_audioClip)
	{
		m_audioClip->Pause();
	}
}

void AudioSpeaker::UnPause() const
{
	if (m_audioClip)
	{
		m_audioClip->UnPause();
	}
}

void AudioSpeaker::Stop()
{
	if (m_audioClip)
	{
		m_audioClip->Stop();
		m_isPlaying = false;
	}
}

void AudioSpeaker::SetAudioClip(AudioClip* audioClip)
{
	if (!audioClip)
	{
		return;
	}

	m_audioClip = audioClip;

	if (m_playOnAwake)
	{
		Play();
	}
}

AudioClip* AudioSpeaker::GetAudioClip() const
{
	return m_audioClip;
}

void AudioSpeaker::SetPlayOnAwake(bool playOnAwake)
{
	m_playOnAwake = playOnAwake;
}

bool AudioSpeaker::GetPlayOnAwake() const
{
	return m_playOnAwake;
}

void AudioSpeaker::SetMute(bool mute)
{
	m_mute = mute;
}

bool AudioSpeaker::GetMute() const
{
	return m_mute;
}

void AudioSpeaker::SetIsLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

bool AudioSpeaker::GetIsLoop() const
{
	return m_isLoop;
}

void AudioSpeaker::SetPriority(int priority)
{
	m_priority = std::clamp(priority, 0, 256);
}

int AudioSpeaker::GetPriority() const
{
	return m_priority;
}

void AudioSpeaker::SetVolume(float volume)
{
	m_volume = std::clamp(volume, 0.f, 1.f);
}

float AudioSpeaker::GetVolume() const
{
	return m_volume;
}

void AudioSpeaker::SetPitch(float pitch)
{
	m_pitch = std::clamp(pitch, 0.f, 2.f);
}

float AudioSpeaker::GetPitch() const
{
	return m_pitch;
}

void AudioSpeaker::SetPan(float pan)
{
	m_pan = std::clamp(pan, -1.f, 1.f);
}

float AudioSpeaker::GetPan() const
{
	return m_pan;
}

void AudioSpeaker::SetIs2DMode(bool is2DMode)
{
	m_is2DMode = is2DMode;
}

bool AudioSpeaker::GetIs2DMode() const
{
	return m_is2DMode;
}

void AudioSpeaker::SetMaxDistance(float max)
{
	m_maxDistance = max(max, 1.f);
}

float AudioSpeaker::GetMaxDistance() const
{
	return m_maxDistance;
}

void AudioSpeaker::SetOutPutMatrix(const Array<float, 8>& matrix)
{
	m_levelMatrix = matrix;
}

const Array<float, 8>& AudioSpeaker::GetOutPutMatrix() const
{
	return m_levelMatrix;
}

void AudioSpeaker::MakeAudioSettingsTheSame(AudioClip* audioClip) const noexcept
{
	audioClip->SetMixOutput(
		m_levelMatrix[0], m_levelMatrix[1], m_levelMatrix[2], m_levelMatrix[3],
		m_levelMatrix[4], m_levelMatrix[5], m_levelMatrix[6], m_levelMatrix[7]
	);
	
	audioClip->SetMute(m_mute);
	audioClip->SetPriority(m_priority);
	audioClip->SetVolume(m_volume);
	audioClip->SetPitch(m_pitch);
	audioClip->SetPan(m_pan);
	audioClip->SetMinMaxDistance(1.f, m_maxDistance);
	
	audioClip->SetMode(GetModeFromSettings());
}

uint32_t AudioSpeaker::GetModeFromSettings() const noexcept
{
	FMOD_MODE mode = FMOD_DEFAULT;

	if (m_is2DMode) {
		mode |= FMOD_2D;
	}
	else {
		mode |= FMOD_3D;
	}

	if (m_isLoop) {
		mode |= FMOD_LOOP_NORMAL;
	}
	else {
		mode |= FMOD_LOOP_OFF;
	}

	return mode;
}

void AudioSpeaker::GetUseResourcePaths(Vector<String>& resources)
{
	if (m_audioClip)
	{
		resources.emplace_back(m_audioClip->GetFilePath());
	}
}