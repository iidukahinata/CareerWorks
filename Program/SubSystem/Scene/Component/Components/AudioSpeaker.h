/**
* @file    AudioSpeaker.h
* @brief
*
* @date	   2022/10/03 2022�N�x����
*/
#pragma once


#include "../IComponent.h"

class AudioClip;

class AudioSpeaker : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(AudioSpeaker)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

public:

	void OnStart() override;
	void OnStop() override;

	void Tick(double deltaTime) override;

public:

	/** �o�^���ꂽ AudioClip ���g�p���ĉ������Đ����܂��B*/
	void Play() noexcept;

	/** SE �Ȃǂ̓����ݒ�ŕ����̉������Đ�����鎞�ȂǂɎg�p�B*/
	void PlayOneShot(AudioClip* audioClip, float volume = 1.f) const noexcept;

	/** �o�^�������Đ�����Ă���ꍇ�̂ݏ������s���B*/
	void Pause() const noexcept;

	/** �o�^������ Pause ���̂ݏ������s���B*/
	void UnPause() const noexcept;

	/** �o�^���ꂽ AudioClip ���g�p���ĉ����𒆎~���܂��B*/
	void Stop() noexcept;

public:

	/** playOnAwake = true �̎��o�^�Ɠ����ɉ������Đ����܂��B*/
	void SetAudioClip(AudioClip* audioClip) noexcept;
	AudioClip* GetAudioClip() const noexcept;

	void SetPlayOnAwake(bool playOnAwake) noexcept;
	bool GetPlayOnAwake() const noexcept;

	void SetMute(bool mute) noexcept;
	bool GetMute() const noexcept;

	void SetIsLoop(bool isLoop) noexcept;
	bool GetIsLoop() const noexcept;

	/* 0 ����ԗD��x�������A�����l�� 0 ���� 256 �̊Ԃ� clamp ����܂��B*/
	void SetPriority(int priority) noexcept;
	int GetPriority() const noexcept;

	/* 1 ����ԉ����傫���A�����l�� 0 ���� 1 �̊Ԃ� clamp ����܂��B*/
	void SetVolume(float volume) noexcept;
	float GetVolume() const noexcept;

	/* 2 ����ԉ��������A�����l�� 0 ���� 2 �̊Ԃ� clamp ����܂��B*/
	void SetPitch(float pitch) noexcept;
	float GetPitch() const noexcept;

	/* 0 �����E�����o�����X�ɂȂ�A�����l�� -1 ���� 1 �̊Ԃ� clamp ����܂��B*/
	void SetPan(float pan) noexcept;
	float GetPan() const noexcept;

	/* true �ɂ���� 3D �����𖳎����܂��B*/
	void SetIs2DMode(bool is2DMode) noexcept;
	bool GetIs2DMode() const noexcept;

	/* 	�T�E���h�̌������~�����鋗�� */
	void SetMaxDistance(float max) noexcept;
	float GetMaxDistance() const noexcept;

	/* �X�s�[�J�[�̉����ʂŒ�������֐��ł��B*/
	void SetOutPutMatrix(const Array<float, 8>& matrix) noexcept;
	const Array<float, 8>& GetOutPutMatrix() const noexcept;

private:

	/** �Đ����ꂽ AudioClip �ɃX�s�[�J�[�ݒ�̔��f���s���B*/
	void MakeAudioSettingsTheSame(AudioClip* audioClip) const noexcept;

	/** �o�^���ꂽ�ݒ肩�特�� Mode ���쐬�B*/
	uint32_t GetModeFromSettings() const noexcept;

private:

	// * ���g�̈ړ����x���t���[���ԍ������狁�߂邽�ߎg�p�B2D �̏ꍇ�͖��g�p�B
	Math::Vector3 m_oldPos;
	Math::Vector3 m_velocity;

	// * �ʒu���� 3D �����̒l�X�V�����B
	AudioClip* m_audioClip = nullptr;

	// * Audio �ݒ�
	bool  m_playOnAwake	   = false;
	bool  m_mute           = false;
	bool  m_isLoop         = false;
	int	  m_priority       = 128;
	float m_volume         = 1.f;
	float m_pitch          = 1.f;
	float m_pan            = 0.f;
	bool  m_is2DMode       = false;
	float m_maxDistance    = 10000;
	Array<float, 8> m_levelMatrix = {};

	bool m_isPlaying = false;
};