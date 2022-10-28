/**
* @file    AudioSpeaker.cpp
* @brief
*
* @date	   2022/10/03 2022年度初版
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

		SetAudioClip(resource);
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

AudioClip* AudioSpeaker::GetAudioClip() const noexcept
{
	return m_audioClip;
}

void AudioSpeaker::SetPlayOnAwake(bool playOnAwake) noexcept
{
	m_playOnAwake = playOnAwake;
}

bool AudioSpeaker::GetPlayOnAwake() const noexcept
{
	return m_playOnAwake;
}

void AudioSpeaker::SetMute(bool mute) noexcept
{
	m_mute = mute;
}

bool AudioSpeaker::GetMute() const noexcept
{
	return m_mute;
}

void AudioSpeaker::SetIsLoop(bool isLoop) noexcept
{
	m_isLoop = isLoop;
}

bool AudioSpeaker::GetIsLoop() const noexcept
{
	return m_isLoop;
}

void AudioSpeaker::SetPriority(int priority) noexcept
{
	m_priority = std::clamp(priority, 0, 256);
}

int AudioSpeaker::GetPriority() const noexcept
{
	return m_priority;
}

void AudioSpeaker::SetVolume(float volume) noexcept
{
	m_volume = std::clamp(volume, 0.f, 1.f);
}

float AudioSpeaker::GetVolume() const noexcept
{
	return m_volume;
}

void AudioSpeaker::SetPitch(float pitch) noexcept
{
	m_pitch = std::clamp(pitch, 0.f, 2.f);
}

float AudioSpeaker::GetPitch() const noexcept
{
	return m_pitch;
}

void AudioSpeaker::SetPan(float pan) noexcept
{
	m_pan = std::clamp(pan, -1.f, 1.f);
}

float AudioSpeaker::GetPan() const noexcept
{
	return m_pan;
}

void AudioSpeaker::SetIs2DMode(bool is2DMode) noexcept
{
	m_is2DMode = is2DMode;
}

bool AudioSpeaker::GetIs2DMode() const noexcept
{
	return m_is2DMode;
}

void AudioSpeaker::SetMaxDistance(float max) noexcept
{
	m_maxDistance = max(max, 1.f);
}

float AudioSpeaker::GetMaxDistance() const noexcept
{
	return m_maxDistance;
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

void AudioSpeaker::SetOutPutMatrix(const Array<float, 8>& matrix) noexcept
{
	m_levelMatrix = matrix;
}

void AudioSpeaker::SetOutPutMatrix(float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright) noexcept
{
	m_levelMatrix[0] = std::clamp(frontleft	   , 0.f, 1.f);
	m_levelMatrix[1] = std::clamp(frontright   , 0.f, 1.f);
	m_levelMatrix[2] = std::clamp(center	   , 0.f, 1.f);
	m_levelMatrix[3] = std::clamp(lfe		   , 0.f, 1.f);
	m_levelMatrix[4] = std::clamp(surroundleft , 0.f, 1.f);
	m_levelMatrix[5] = std::clamp(surroundright, 0.f, 1.f);
	m_levelMatrix[6] = std::clamp(backleft	   , 0.f, 1.f);
	m_levelMatrix[7] = std::clamp(backright	   , 0.f, 1.f);
}

const Array<float, 8>& AudioSpeaker::GetOutPutMatrix() const noexcept
{
	return m_levelMatrix;
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