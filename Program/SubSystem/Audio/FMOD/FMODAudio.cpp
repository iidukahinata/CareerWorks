/**
* @file    FMODAudio.cpp
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/


#include <fmod.hpp>
#include "FMODAudio.h"
#include "../AudioHelper.h"
#include "Subsystem/Scene/Component/Components/AudioListener.h"

bool FMODAudio::Initialize()
{
	m_job.SetFunction([this](double) { Update(); }, FunctionType::Update);
	m_job.RegisterToJobSystem();

	constexpr auto maxChannel = 64;
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_system);
	AUDIO_EORROR_CHECK(result);

	result = m_system->init(maxChannel, FMOD_INIT_NORMAL, nullptr);
	AUDIO_EORROR_CHECK(result);
	return true;
}

void FMODAudio::Shutdown()
{
	if (m_system)
	{
		m_system->close();
		m_system->release();
	}

	m_job.UnRegisterFromJobSystem();
}

void FMODAudio::Update() const noexcept
{
	ASSERT(m_system);

	FMOD_RESULT result;

	// 3D Mode éûèàóù
	if (m_lisrener)
	{
		auto& transform = m_lisrener->GetTransform();
		
		auto position = ToFMODVector(transform.GetPosition());
		auto forward = ToFMODVector(transform.GetForward());
		auto up = ToFMODVector(transform.GetUp());
		auto velocity = ToFMODVector(m_lisrener->GetVelocity());
		
		result = m_system->set3DListenerAttributes(0, &position, &velocity, &forward, &up);
		AUDIO_EORROR_CHECK(result);
	}

	result = m_system->update();
	AUDIO_EORROR_CHECK(result);
}

void FMODAudio::Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept
{
	ASSERT(m_system);
	auto result = m_system->set3DSettings(dopplerscale, distancefactor, rolloffscale);
	AUDIO_EORROR_CHECK(result);
}

FMOD::System* FMODAudio::GetSystem() const noexcept
{
	return m_system;
}