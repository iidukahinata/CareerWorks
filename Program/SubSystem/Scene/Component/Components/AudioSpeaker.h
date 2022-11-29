/**
* @file    AudioSpeaker.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IAudioSpeaker.h"

class AudioSpeaker : public IAudioSpeaker
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(AudioSpeaker, IAudioSpeaker)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

public:

	void OnStart() override;
	void OnStop() override;

	void Tick(double deltaTime) override;

public:

	/** 登録された AudioClip を使用して音源を再生します。*/
	void Play() override;

	/** SE などの同じ設定で複数の音源を再生される時などに使用。*/
	void PlayOneShot(AudioClip* audioClip, float volume = 1.f) const override;

	/** 登録音源が再生されている場合のみ処理を行う。*/
	void Pause() const override;

	/** 登録音源が Pause 時のみ処理を行う。*/
	void UnPause() const override;

	/** 登録された AudioClip を使用して音源を中止します。*/
	void Stop() override;

public:

	/** playOnAwake = true の時登録と同時に音源を再生します。*/
	void SetAudioClip(AudioClip* audioClip) override;
	AudioClip* GetAudioClip() const override;

	void SetPlayOnAwake(bool playOnAwake) override;
	bool GetPlayOnAwake() const override;

	void SetMute(bool mute) override;
	bool GetMute() const override;

	void SetIsLoop(bool isLoop) override;
	bool GetIsLoop() const override;

	/* 0 が一番優先度が高く、引数値は 0 から 256 の間に clamp されます。*/
	void SetPriority(int priority) override;
	int GetPriority() const override;

	/* 1 が一番音が大きく、引数値は 0 から 1 の間に clamp されます。*/
	void SetVolume(float volume) override;
	float GetVolume() const override;

	/* 2 が一番音が高く、引数値は 0 から 2 の間に clamp されます。*/
	void SetPitch(float pitch) override;
	float GetPitch() const override;

	/* 0 が左右同じバランスになり、引数値は -1 から 1 の間に clamp されます。*/
	void SetPan(float pan) override;
	float GetPan() const override;

	/* true にすると 3D 処理を無視します。*/
	void SetIs2DMode(bool is2DMode) override;
	bool GetIs2DMode() const override;

	/* 	サウンドの減衰を停止させる距離 */
	void SetMaxDistance(float max) override;
	float GetMaxDistance() const override;

	/* スピーカーの音を個別で調整する関数です。*/
	void SetOutPutMatrix(const Array<float, 8>& matrix) override;
	const Array<float, 8>& GetOutPutMatrix() const override;

private:

	/** 再生された AudioClip にスピーカー設定の反映を行う。*/
	void MakeAudioSettingsTheSame(AudioClip* audioClip) const noexcept;

	/** 登録された設定から音声 Mode を作成。*/
	uint32_t GetModeFromSettings() const noexcept;

public:

	void GetUseResourcePaths(Vector<String>& resources) override;

private:

	// * 自身の移動速度をフレーム間差分から求めるため使用。2D の場合は未使用。
	Math::Vector3 m_oldPos;
	Math::Vector3 m_velocity;

	// * 位置等の 3D 処理の値更新される。
	AudioClip* m_audioClip = nullptr;

	// * Audio 設定
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