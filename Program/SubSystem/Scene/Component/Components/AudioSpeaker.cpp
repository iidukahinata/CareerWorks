/**
* @file    AudioSpeaker.cpp
* @brief
*
* @date	   2022/10/02 2022年度初版
*/


#include <fmod.hpp>
#include "AudioSpeaker.h"
#include "SubSystem/Audio/Audio.h"
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

void AudioSpeaker::Deserialization(FileStream* file)
{
	IComponent::Deserialization(file);

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

		SetAudioClip(resource);
	}
}

void AudioSpeaker::OnStart()
{
	if (m_playOnAwake)
	{
		Play();
	}

	m_tickFunction.RegisterToTickManager();
}

void AudioSpeaker::OnStop()
{
	Stop();

	m_tickFunction.UnRegisterFromTickManager();
}

void AudioSpeaker::Tick(double deltaTime)
{
	Math::Vector3 worldPos = GetTransform().GetWoldPosition();
	m_velocity = worldPos - m_oldPos;

	if (m_audioClip)
	{
		m_audioClip->SetAttributes(worldPos, m_velocity);
	}

	m_oldPos = worldPos;
}

void AudioSpeaker::Play() noexcept
{
	ASSERT(m_audioClip);

	if (m_isPlaying)
		Stop();

	m_audioClip->Play();

	// thisの設定をaudioDateに渡す
	MakeAudioSettingsTheSame(m_audioClip);

	m_isPlaying = true;
}

void AudioSpeaker::PlayOneShot(AudioClip* clip, float volume /* = 1.f */) const noexcept
{
	ASSERT(clip);

	clip->Play();

	// thisの設定をaudioDateに渡す
	MakeAudioSettingsTheSame(clip);

	clip->SetVolume(volume);

	// 3D 時、初期値として再生位置をセット。
	if (m_is2DMode == false)
	{
		auto worldPos = GetTransform().GetWoldPosition();
		clip->SetAttributes(worldPos, m_velocity);
	}
}

void AudioSpeaker::Pause() const noexcept
{
	if (m_audioClip)
	{
		m_audioClip->Pause();
	}
}

void AudioSpeaker::UnPause() const noexcept
{
	if (m_audioClip)
	{
		m_audioClip->UnPause();
	}
}

void AudioSpeaker::Stop() noexcept
{
	if (m_audioClip)
	{
		m_audioClip->Stop();
		m_isPlaying = false;
	}
}

void AudioSpeaker::SetAudioClip(IResource* resource, bool playOnAwake) noexcept
{
	if (auto clip = dynamic_cast<AudioClip*>(resource))
	{
		SetAudioClip(clip, playOnAwake);
	}
}

void AudioSpeaker::SetAudioClip(AudioClip* pAudioClip, bool playOnAwake /* = false */) noexcept
{
	m_audioClip = pAudioClip;

	if (playOnAwake)
	{
		Play();
	}
}

void AudioSpeaker::SetMute(bool mute) noexcept
{
	m_mute = mute;
}

void AudioSpeaker::SetIsLoop(bool isLoop) noexcept
{
	m_isLoop = isLoop;
}

void AudioSpeaker::SetPriority(int priority) noexcept
{
	m_priority = std::clamp(priority, 0, 256);
}

void AudioSpeaker::SetVolume(float volume) noexcept
{
	m_volume = std::clamp(volume, 0.f, 1.f);
}

void AudioSpeaker::SetPitch(float pitch) noexcept
{
	m_pitch = std::clamp(pitch, 0.f, 2.f);
}

void AudioSpeaker::SetPan(float pan) noexcept
{
	m_pan = std::clamp(pan, -1.f, 1.f);
}

void AudioSpeaker::SetIs2DMode(bool is2DMode) noexcept
{
	m_is2DMode = is2DMode;
}

void AudioSpeaker::SetMaxDistance(float max) noexcept
{
	m_maxDistance = max(max, 1.f);
}

void AudioSpeaker::SetOutPutMatrix(float* matrix, int size) noexcept
{
	ASSERT(size < m_levelMatrix.size());

	size = min(size, 8);
	for (int i = 0; i < size; ++i)
	{
		m_levelMatrix[i] = std::clamp(matrix[i], 0.f, 1.f);
	}
}

void AudioSpeaker::SetOutPutMatrix(float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright) noexcept
{
	m_levelMatrix[0] = frontleft;
	m_levelMatrix[1] = frontright;
	m_levelMatrix[2] = center;
	m_levelMatrix[3] = lfe;
	m_levelMatrix[4] = surroundleft;
	m_levelMatrix[5] = surroundright;
	m_levelMatrix[6] = backleft;
	m_levelMatrix[7] = backright;
}

void AudioSpeaker::MakeAudioSettingsTheSame(AudioClip* clip) const noexcept
{
	clip->SetMixOutput(
		m_levelMatrix[0], m_levelMatrix[1], m_levelMatrix[2], m_levelMatrix[3],
		m_levelMatrix[4], m_levelMatrix[5], m_levelMatrix[6], m_levelMatrix[7]
	);
	
	clip->SetMute(m_mute);
	clip->SetPriority(m_priority);
	clip->SetVolume(m_volume);
	clip->SetPitch(m_pitch);
	clip->SetPan(m_pan);
	clip->SetMinMaxDistance(1.f, m_maxDistance);
	
	clip->SetMode(GetModeFromSettings());
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