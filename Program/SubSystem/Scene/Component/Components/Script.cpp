/**
* @file    Script.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "Script.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"
#include "SubSystem/Scene/Component/IRigidBody.h"

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

		if (m_scriptInstance->HasTickFunction())
		{
			m_tickFunction.RegisterToTickManager();
		}

		if (!m_scriptInstance->IsRegisterEventFunction())
		{
			m_scriptInstance->RegisterEventFunctionList();
		}
	}
}

void Script::OnStop()
{
	IComponent::OnStop();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Stop);

		if (m_scriptInstance->HasTickFunction())
		{
			m_tickFunction.UnRegisterFromTickManager();
		}

		if (m_scriptInstance->IsRegisterEventFunction())
		{
			m_scriptInstance->UnRegisterEventFunctionList();
		}
	}
}

void Script::OnRemove()
{
	IComponent::OnRemove();

	if (m_scriptInstance)
	{
		m_scriptInstance->CallFunction(this, ScriptFuncType::Remove);

		// destory this script instance
		m_scriptInstance->UnRegisterScript(this);
	}
}

void Script::Tick(double deltaTime)
{
	m_scriptInstance->CallFunction(this, ScriptFuncType::Update, deltaTime);
}

void Script::SetScript(ScriptInstance* scriptInstance)
{
	if (m_scriptInstance)
	{
		// destory this script instance
		m_scriptInstance->UnRegisterScript(this);
	}

	m_scriptInstance = scriptInstance;

	if (scriptInstance)
	{
		// create function list for this script instance.
		m_scriptInstance->RegisterScript(this);

		FnishSetScript();
	}
}

void Script::FnishSetScript()
{
	// call init functions from component state
	CallInitFunctions();

	if (IsBeginPlay() && m_scriptInstance->HasTickFunction())
	{
		m_tickFunction.RegisterToTickManager();
	}
}

ScriptInstance* Script::GetScript() const
{
	return m_scriptInstance;
}

void Script::NotifyHit(HitEventType type, IRigidBody* rigidBody)
{
	auto funcType = ScriptFuncType::OnCollisionEnter + type;
	m_scriptInstance->CallFunction(this, static_cast<ScriptFuncType>(funcType), boost::python::ptr(rigidBody));
}

void Script::CallInitFunctions()
{
	ASSERT(m_scriptInstance);

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