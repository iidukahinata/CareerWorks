/**
* @file    FMODAudio.h
* @brief
*
* @date	   2022/09/06 2022�N�x����
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

	/** �S�Ă� 3D �T�E���h�̃O���[�o���ݒ��ύX���܂��B*/
	void Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept;

	FMOD::System* GetSystem() const noexcept;

private:

	// * Audio Update �o�^�p
	Job m_job;

	FMOD::System* m_system = nullptr;
};