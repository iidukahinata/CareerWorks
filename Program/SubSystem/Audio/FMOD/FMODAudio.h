/**
* @file    FMODAudio.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "../IAudio.h"

namespace FMOD
{
	class System;
}

class FMODAudio : public IAudio
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(FMODAudio, IAudio)
public:

	bool Initialize() override;
	void Shutdown() override;

public:

	void Update() const noexcept;

	/** 全ての 3D サウンドのグローバル設定を変更します。*/
	void Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept;

	FMOD::System* GetSystem() const noexcept;

private:

	// * Audio Update 登録用
	Job m_job;

	FMOD::System* m_system = nullptr;
};