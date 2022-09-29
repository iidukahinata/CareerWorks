/**
* @file    Config.cpp
* @brief
*
* @date	   2022/09/07 2022年度初版
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

extern Context* g_context;

void Config::RegisterSubsystemsToContainer() noexcept
{
	bool hasCofigData = false;

	if (hasCofigData)
	{
		// 前回ロード時のデータをセーブを行うようにしたら使用
	}
	else
	{
		SetUpDefaultSettings();
	}
}

void Config::SetUpDefaultSettings() noexcept
{
	g_context->RegisterSubsystem<Timer>(std::make_unique<Timer>());
	g_context->RegisterSubsystem<ResourceManager>(std::make_unique<ResourceManager>());
	g_context->RegisterSubsystem<Input>(std::make_unique<DirectInput>());
	g_context->RegisterSubsystem<Audio>(std::make_unique<FMODAudio>());
	g_context->RegisterSubsystem<Renderer>(std::make_unique<ForwardRenderer>());
	g_context->RegisterSubsystem<World>(std::make_unique<World>());
}