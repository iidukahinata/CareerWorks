/**
* @file    IAudioSpeaker.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "IComponent.h"

class AudioClip;

class IAudioSpeaker : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudioSpeaker, IComponent)
public:

	/** 登録された AudioClip を使用して音源を再生します。*/
	virtual void Play()
	{
		ASSERT(0);
	}

	/** SE などの同じ設定で複数の音源を再生される時などに使用。*/
	virtual void PlayOneShot(AudioClip* audioClip, float volume = 1.f) const
	{
		ASSERT(0);
	}

	/** 登録音源が再生されている場合のみ処理を行う。*/
	virtual void Pause() const
	{
		ASSERT(0);
	}

	/** 登録音源が Pause 時のみ処理を行う。*/
	virtual void UnPause() const
	{
		ASSERT(0);
	}

	/** 登録された AudioClip を使用して音源を中止します。*/
	virtual void Stop()
	{
		ASSERT(0);
	}

public:

	/** playOnAwake = true の時登録と同時に音源を再生します。*/
	virtual void SetAudioClip(AudioClip* audioClip)
	{
		ASSERT(0);
	}
	virtual AudioClip* GetAudioClip() const
	{
		ASSERT(0); return {};
	}

	virtual void SetPlayOnAwake(bool playOnAwake)
	{
		ASSERT(0);
	}
	virtual bool GetPlayOnAwake() const
	{
		ASSERT(0); return {};
	}

	virtual void SetMute(bool mute)
	{
		ASSERT(0);
	}
	virtual bool GetMute() const
	{
		ASSERT(0); return {};
	}

	virtual void SetIsLoop(bool isLoop)
	{
		ASSERT(0);
	}
	virtual bool GetIsLoop() const
	{
		ASSERT(0); return {};
	}

	/* 0 が一番優先度が高く、引数値は 0 から 256 の間に clamp されます。*/
	virtual void SetPriority(int priority)
	{
		ASSERT(0);
	}
	virtual int GetPriority() const
	{
		ASSERT(0); return {};
	}

	/* 1 が一番音が大きく、引数値は 0 から 1 の間に clamp されます。*/
	virtual void SetVolume(float volume)
	{
		ASSERT(0);
	}
	virtual float GetVolume() const
	{
		ASSERT(0); return {};
	}

	/* 2 が一番音が高く、引数値は 0 から 2 の間に clamp されます。*/
	virtual void SetPitch(float pitch)
	{
		ASSERT(0);
	}
	virtual float GetPitch() const
	{
		ASSERT(0); return {};
	}

	/* 0 が左右同じバランスになり、引数値は -1 から 1 の間に clamp されます。*/
	virtual void SetPan(float pan)
	{
		ASSERT(0);
	}
	virtual float GetPan() const
	{
		ASSERT(0); return {};
	}

	/* true にすると 3D 処理を無視します。*/
	virtual void SetIs2DMode(bool is2DMode)
	{
		ASSERT(0);
	}
	virtual bool GetIs2DMode() const
	{
		ASSERT(0); return {};
	}

	/* 	サウンドの減衰を停止させる距離 */
	virtual void SetMaxDistance(float max)
	{
		ASSERT(0);
	}
	virtual float GetMaxDistance() const
	{
		ASSERT(0); return {};
	}

	/* スピーカーの音を個別で調整する関数です。*/
	virtual void SetOutPutMatrix(const Array<float, 8>& matrix)
	{
		ASSERT(0);
	}
	virtual const Array<float, 8>& GetOutPutMatrix() const
	{
		ASSERT(0); return {};
	}
};