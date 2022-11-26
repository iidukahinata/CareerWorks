/**
* @file    Script.cpp
* @brief
*
* @date	   2022/11/20 2022”N“x‰”Å
*/


#include "Script.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"

void Script::Serialized(FileStream* file) const
{
	IComponent::Serialized(file);

	if (m_scriptInstance)
	{
		file->Write(m_scriptInstance->GetFilePath());
	}
	else
	{
		file->Write(String());
	}
}

void Script::Deserialized(FileStream* file)
{
	IComponent::Deserialized(file);

	String scriptPath;
	file->Read(&scriptPath);

	if (scriptPath.empty())
	{
		return;
	}

	if (auto resourceManager = GetContext()->GetSubsystem<ResourceManager>())
	{
		if (auto resourceData = resourceManager->GetResourceData(scriptPath))
		{
			auto resource = resourceManager->GetResource(resourceData);
			SetScript(dynamic_cast<ScriptInstance*>(resource));
		}
	}
}

void Script::OnRegister()
{
	IComponent::OnRegister();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Register);
	}
}

void Script::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::UnRegister);
	}
}

void Script::OnStart()
{
	IComponent::OnStart();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Start);
		m_scriptInstance->RegisterScript(this);

		if (m_scriptInstance->HasTickFunction())
		{
			m_tickFunction.RegisterToTickManager();
		}
	}
}

void Script::OnStop()
{
	IComponent::OnStop();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Stop);
		m_scriptInstance->UnRegisterScript(this);

		if (m_scriptInstance->HasTickFunction())
		{
			m_tickFunction.UnRegisterFromTickManager();
		}
	}
}

void Script::OnRemove()
{
	IComponent::OnRemove();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Remove);
	}
}

void Script::Tick(double deltaTime)
{
	ASSERT(m_scriptInstance);
	m_scriptInstance->CallTickFunction(this, deltaTime);
}

void Script::SetScript(ScriptInstance* scriptInstance) noexcept
{
	if (m_scriptInstance == scriptInstance)
	{
		return;
	}

	if (m_scriptInstance && IsBeginPlay())
	{
		m_scriptInstance->UnRegisterScript(this);
	}

	m_scriptInstance = scriptInstance;

	if (m_scriptInstance && IsBeginPlay())
	{
		m_scriptInstance->RegisterScript(this);

		if (m_scriptInstance->HasTickFunction())
		{
			m_tickFunction.RegisterToTickManager();
		}
	}

	// call functions from component state
	CallInitFunctions();
}

ScriptInstance* Script::GetScript() const noexcept
{
	return m_scriptInstance;
}

void Script::CallInitFunctions()
{
	m_scriptInstance->CallFunction(this, ScriptFuncType::Init);

	if (IsRegistered())
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Register);
	}

	if (IsBeginPlay())
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Start);
	}
}