/**
* @file    AudioListener.h
* @brief
*
* @date	   2022/10/02 2022年度初版
*/
#pragma once


#include "../IComponent.h"

class IAudio;

class AudioListener : public IComponent
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(AudioListener)
public:

	void OnInitialize() override;
	void OnRegister() override;
	void OnUnRegister() override;

public:

	/** 3D Mode 時に乗り物に乗っているような音源になるよう計算されます。*/
	void SetVelocity(const Math::Vector3& velocity) noexcept;
	const Math::Vector3& GetVelocity() const noexcept;

private:

	void RegisterToAudioSystem();
	void UnRegisterFromAudioSystem();

private:

	Math::Vector3 m_velocity;

	IAudio* m_audio = nullptr;
};