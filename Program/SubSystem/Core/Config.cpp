/**
* @file    Config.cpp
* @brief
*
* @date	   2022/10/27 2022”N“x‰”Å
*/


#include "Config.h"

// Subsystem
#include "SubSystem/Scene/World.h"
#include "SubSystem/Timer/Timer.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Audio/FMOD/FMODAudio.h"
#include "SubSystem/Input/Direct/DirectInput.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Renderer/Forward/ForwardRenderer.h"
#include "SubSystem/Renderer/Deferred/DeferredRenderer.h"

RendererType Config::m_rendererSystem = RendererType::Forward;
InputType	 Config::m_inputSystem	  = InputType::Direct;
AudioType	 Config::m_audioSystem	  = AudioType::FMOD;

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
	GenerateFile(RESOURCE_DIRECTORY);
	GenerateFile(MODEL_DIRECTORY);
	GenerateFile(MESH_DIRECTORY);
	GenerateFile(MATERIAL_DIRECTORY);
	GenerateFile(TEXTURE_DIRECTORY);
	GenerateFile(SHADER_DIRECTORY);
	GenerateFile(AUDIO_DIRECTORY);
	GenerateFile(SCENE_DIRECTORY);
	GenerateFile(PROJECT_SETTINGS_DIRECTORY);
}

void Config::RegisterSubsystemsToContainer() noexcept
{
	auto hasSettingsFile = FileSystem::Exists(SYSTEM_SETTINGS_PATH);

	if (hasSettingsFile)
	{
		FileStream file(SYSTEM_SETTINGS_PATH, OpenMode::Write_Mode);
		ASSERT(file.IsOpen());

		file.Read(&m_rendererSystem);
		file.Read(&m_inputSystem);
		file.Read(&m_audioSystem);

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

#define REGISTER_SUBSYSTEM(KEY, CLASS) g_context->RegisterSubsystem<KEY>(std::make_unique<CLASS>());

void Config::RegisterRendererSystem(RendererType type, bool saveSettings /* = true */) noexcept
{
	switch (type)
	{
	case RendererType::Forward: REGISTER_SUBSYSTEM(Renderer, ForwardRenderer); break;
	case RendererType::Deferred: REGISTER_SUBSYSTEM(Renderer, DeferredRenderer); break;
	default: break;
	}

	m_rendererSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::RegisterInputSystem(InputType type, bool saveSettings /* = true */) noexcept
{
	switch (type)
	{
	case InputType::Direct: REGISTER_SUBSYSTEM(Input, DirectInput); break;
	default: break;
	}

	m_inputSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::RegisterAudioSystem(AudioType type, bool saveSettings /* = true */) noexcept
{
	switch (type)
	{
	case AudioType::FMOD: REGISTER_SUBSYSTEM(Audio, FMODAudio); break;
	default: break;
	}

	m_audioSystem = type;
	if (saveSettings) SaveCurrentConfig();
}

void Config::SetUpSubsystem() noexcept
{
	g_context->RegisterSubsystem<Timer>(std::make_unique<Timer>());
	g_context->RegisterSubsystem<ResourceManager>(std::make_unique<ResourceManager>());
	RegisterInputSystem(m_inputSystem, false);
	RegisterAudioSystem(m_audioSystem, false);
	RegisterRendererSystem(m_rendererSystem, false);
	g_context->RegisterSubsystem<World>(std::make_unique<World>());
}

void Config::SaveCurrentConfig() noexcept
{
	FileStream file(SYSTEM_SETTINGS_PATH, OpenMode::Write_Mode);
	ASSERT(file.IsOpen());

	file.Write(m_rendererSystem);
	file.Write(m_inputSystem);
	file.Write(m_audioSystem);
}