/**
* @file    FMODAudio.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/


#include "FMODAudio.h"
#include "../AudioHelper.h"
#include "Subsystem/Scene/Component/Components/AudioListener.h"

bool FMODAudio::Initialize()
{
	m_job.SetFunction([this](double) { Update(); }, FunctionType::PrePhysics);
	m_job.RegisterToJobSystem();

	constexpr auto maxChannel = 64;
	AUDIO_EORROR_CHECK(FMOD::System_Create(&m_system));
	AUDIO_EORROR_CHECK(m_system->init(maxChannel, FMOD_INIT_NORMAL, nullptr));

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
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(MainThread, "Audio Update");
#endif // IS_EDITOR

	ASSERT(m_system);

	// 3D Mode Žžˆ—
	if (m_lisrener)
	{
		const auto& transform = m_lisrener->GetTransform();
		
		auto position = ToFMODVector(transform.GetPosition());
		auto forward  = ToFMODVector(transform.GetForward());
		auto up		  = ToFMODVector(transform.GetUp());
		auto velocity = ToFMODVector(m_lisrener->GetVelocity());
		
		AUDIO_EORROR_CHECK(m_system->set3DListenerAttributes(0, &position, &velocity, &forward, &up));
	}

	AUDIO_EORROR_CHECK(m_system->update());

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(MainThread);
#endif // IS_EDITOR
}

void FMODAudio::Set3DSettings(float dopplerscale, float distancefactor, float rolloffscale) const noexcept
{
	ASSERT(m_system);
	AUDIO_EORROR_CHECK(m_system->set3DSettings(dopplerscale, distancefactor, rolloffscale));
}

FMOD::System* FMODAudio::GetSystem() const noexcept
{
	return m_system;
}