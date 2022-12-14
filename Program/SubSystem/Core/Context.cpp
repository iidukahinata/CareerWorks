/**
 * @file	Context.cpp
 * @brief	Subsystem コンテナクラス
 *
 * @date	2022/11/27 2022年度初版
 */


#include "Context.h"

void Context::Release()
{
	for (const auto& subsystem : m_subsystems)
	{
		subsystem.second->Shutdown();
	}

	m_subsystems.clear();
}

void Context::RegisterSubsystem(uint32_t hash, ISubsystem* subsystem) noexcept
{
	// 重複チェック
	if (!m_subsystems.contains(hash))
	{
		m_subsystems[hash].reset(subsystem);
		m_subsystems[hash]->Initialize();
	}
}

void Context::UnRegisterSubsystem(uint32_t hash) noexcept
{
	if (m_subsystems.contains(hash))
	{
		m_subsystems[hash]->Shutdown();
		m_subsystems.erase(hash);
	}
}

ISubsystem* Context::GetSubsystemByHash(uint32_t hash) noexcept
{
	if (m_subsystems.contains(hash))
	{
		return m_subsystems[hash].get();
	}
	return nullptr;
}