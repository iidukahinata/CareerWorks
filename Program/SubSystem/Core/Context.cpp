/**
 * @file	Context.cpp
 * @brief	Subsystem �R���e�i�N���X
 *
 * @date	2022/08/02 2022�N�x����
 */


#include "Context.h"

void Context::Release()
{
	for (auto& subsystem : m_subsystems)
	{
		subsystem.second->Shutdown();
	}

	m_subsystems.clear();
}

void Context::RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept
{
	// �d���`�F�b�N
	ASSERT(!m_subsystems.contains(hash));

	m_subsystems[hash].reset(subsystem);
}

ISubsystem* Context::GetSubsystemByHash(uint32_t hash) noexcept
{
	if (m_subsystems.contains(hash))
	{
		return m_subsystems[hash].get();
	}
	return nullptr;
}