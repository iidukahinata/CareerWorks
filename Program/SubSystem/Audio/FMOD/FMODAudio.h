/**
* @file    FMODAudio.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
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

	/** �S�Ă� 3D �T�E���h�̃O���[�o���ݒ��ύX���܂��B*/
	void Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept;

	FMOD::System* GetSystem() const noexcept;

private:

	// * Audio Update �o�^�p
	Job m_job;

	FMOD::System* m_system = nullptr;
};