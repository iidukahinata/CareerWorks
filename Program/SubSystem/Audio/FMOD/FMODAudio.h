/**
* @file    FMODAudio.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include "../Audio.h"

namespace FMOD
{
	class System;
}

class FMODAudio : public Audio
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(FMODAudio)
public:

	bool Initialize() override;
	void Shutdown() override;

	void Update() const noexcept;

	/** 全ての 3D サウンドのグローバル設定を変更します。*/
	void Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept;

	FMOD::System* GetSystem() const noexcept;

private:

	// * Audio Update 登録用
	Job m_job;

	FMOD::System* m_system = nullptr;
};