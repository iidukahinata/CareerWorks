/**
* @file		AudioClip.h
* @brief
*
* @date		2022/08/09 2022�N�x����
*/
#pragma once


#include <fmod.hpp>
#include "../IResource.h"

class FMODAudio;

/**
* �I�[�f�B�I�Đ����邽�тɁA�Ăяo������ Channel �ݒ��ύX����K�v������B
* AudioClip �݂̂ł̎g�p���\�����AAudioSpeaker �ƕ��p���Ă̎g�p�𐄏��B
*/
class AudioClip : public IResource
{
	SUB_CLASS(AudioClip)
public:

	AudioClip();
	~AudioClip();

	bool Load(StringView path) override;

public:

	void Play(FMOD::ChannelGroup* channelgroup = nullptr) noexcept;
	void Pause() const noexcept;
	void UnPause() const noexcept;
	void Stop() noexcept;

	/** Audio Channel �ݒ� */
	void SetMute(bool mute) const noexcept;
	void SetVolume(float volume) const noexcept;
	void SetPitch(float pitch) const noexcept;
	void SetPan(float pan) const noexcept;
	void SetMinMaxDistance(float min, float max) const noexcept;
	void SetAttributes(const Math::Vector3& pos, const Math::Vector3& vel) const noexcept;
	void SetMixOutput(float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright) const noexcept;

	/* Audio Sound �ݒ� */
	void SetPriority(int priority) const noexcept;
	void SetDefaults(float frequency, int priority) const noexcept;
	void SetMode(FMOD_MODE mode) const noexcept;

	bool IsPlaying() const noexcept;

private:

	void Release() noexcept;

private:

	FMODAudio* m_audio;

	//* �T�E���h�f�[�^��ێ��B
	FMOD::Sound* m_sound = nullptr;

	//* �T�E���h�ݒ��ύX���Ɏg�p�B�T�E���h�Đ����̂݃I�u�W�F�N�g��ێ�����B
	FMOD::Channel* m_channel = nullptr;
};