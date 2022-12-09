/**
* @file    Config.cpp
* @brief
*
* @date	   2022/11/09 2022”N“x‰”Å
*/


#include "Config.h"

// Subsystem
#include "SubSystem/Scene/World.h"
#include "SubSystem/Timer/Timer.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Physics/PhysX/PhysX.h"
#include "SubSystem/Audio/FMOD/FMODAudio.h"
#include "SubSystem/Input/Direct/DirectInput.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Renderer/Forward/ForwardRenderer.h"
#include "SubSystem/Renderer/Deferred/DeferredRenderer.h"
#include "SubSystem/Script/ScriptEngine.h"

RendererType Config::m_rendererSystem = RendererType::Forward;
InputType	 Config::m_inputSystem	  = InputType::Direct;
AudioType	 Config::m_audioSystem	  = AudioType::FMOD;
PhysicsType	 Config::m_physicsSystem  = PhysicsType::PhysX;

void Config::GenerateUseFile() noexcept
{
	auto GenerateFile = [](StringView path)
	{
		if (FileSystem::Exists(path) == false)
		{
			FileSystem::CreateDirectory(path);
		}
	};

	GenerateFile(IMPORT_ASSET_DIRECTORY);
	GenerateFile(DATA_DIRECTORY);
	GenerateFile(TEMP_DATA_DIRECTORY);
	GenerateFile(LOG_DATA_DIRECTORY);
	GenerateFile(ASSET_DATA_DIRECTORY);
	GenerateFile(ORIGINAL_DATA_DIRECTORY);
	GenerateFile(ORIGINAL_MODEL_DIRECTORY);
	GenerateFile(ORIGINAL_TEXTURE_DIRECTORY);
	GenerateFile(ORIGINAL_SHADER_DIRECTORY);
	GenerateFile(ORIGINAL_AUDIO_DIRECTORY);
	GenerateFile(ORIGINAL_SCRIPT_DIRECTORY);
	GenerateFile(RESOURCE_DIRECTORY);
	GenerateFile(MODEL_DIRECTORY);
	GenerateFile(MESH_DIRECTORY);
	GenerateFile(MATERIAL_DIRECTORY);
	GenerateFile(TEXTURE_DIRECTORY);
	GenerateFile(SHADER_DIRECTORY);
	GenerateFile(AUDIO_DIRECTORY);
	GenerateFile(SCENE_DIRECTORY);
	GenerateFile(SCRIPT_DIRECTORY);
	GenerateFile(PROJECT_SETTINGS_DIRECTORY);
}

void Config::RegisterSubsystemsToContainer() noexcept
{
	auto hasSettingsFile = FileSystem::Exists(SYSTEM_SETTINGS_PATH);

	if (hasSettingsFile)
	{
		FileStream file(SYSTEM_SETTINGS_PATH, OpenMode::Read_Mode);
		ASSERT(file.IsOpen());

		file.Read((int*)&m_rendererSystem);
		file.Read((int*)&m_inputSystem);
		file.Read((int*)&m_audioSystem);
		file.Read((int*)&m_physicsSystem);
		file.Close();

		// set up save settings
		SetUpSubsystem();
	}
	else
	{
		// set up default settings
		SetUpSubsystem();

		SaveCurrentConfig();
	}
}

#define REGISTER_SUBSYSTEM(KEY, CLASS) g_context->RegisterSubsystem<KEY, CLASS>();
#define UNREGISTER_SUBSYSTEM(KEY) g_context->UnRegisterSubsystem<KEY>();

void Config::RegisterRendererSystem(RendererType type, bool saveSettings /* = true */) noexcept
{
	switch (type)
	{
	case RendererType::Forward:  REGISTER_SUBSYSTEM(IRenderer, ForwardRenderer); break;
	case RendererType::Deferred: REGISTER_SUBSYSTEM(IRenderer, DeferredRenderer); break;
	default: break;
	}

	m_rendererSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::RegisterInputSystem(InputType type, bool saveSettings /* = true */) noexcept
{
	UNREGISTER_SUBSYSTEM(IInput);

	switch (type)
	{
	case InputType::Direct: REGISTER_SUBSYSTEM(IInput, DirectInput); break;
	default: break;
	}

	m_inputSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::RegisterAudioSystem(AudioType type, bool saveSettings /* = true */) noexcept
{
	UNREGISTER_SUBSYSTEM(IAudio);

	switch (type)
	{
	case AudioType::FMOD: REGISTER_SUBSYSTEM(IAudio, FMODAudio); break;
	default: break;
	}

	m_audioSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::RegisterPhysicsSystem(PhysicsType type, bool saveSettings) noexcept
{
	UNREGISTER_SUBSYSTEM(IPhysics);

	switch (type)
	{
	case PhysicsType::PhysX: REGISTER_SUBSYSTEM(IPhysics, PhysX); break;
	default: break;
	}

	m_physicsSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::SetUpSubsystem() noexcept
{
	g_context->RegisterSubsystem<Timer>();
	g_context->RegisterSubsystem<ResourceManager>();
	g_context->RegisterSubsystem<ScriptEngine>();
	g_context->RegisterSubsystem<World>();
	RegisterInputSystem(m_inputSystem, false);
	RegisterAudioSystem(m_audioSystem, false);
	RegisterPhysicsSystem(m_physicsSystem, false);
	RegisterRendererSystem(m_rendererSystem, false);
}

void Config::SaveCurrentConfig() noexcept
{
	FileStream file(SYSTEM_SETTINGS_PATH, OpenMode::Write_Mode);
	ASSERT(file.IsOpen());

	file.Write((int)m_rendererSystem);
	file.Write((int)m_inputSystem);
	file.Write((int)m_audioSystem);
	file.Write((int)m_physicsSystem);
}