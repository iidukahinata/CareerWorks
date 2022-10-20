/**
* @file    Engine.cpp
* @brief
*
* @date	   2022/10/03 2022年度初版
*/


#include "Engine.h"

// Subsystem
#include "SubSystem/Timer/Timer.h"
#include "SubSystem/Input/Input.h"
#include "SubSystem/Audio/Audio.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Resource/ResourceManager.h"

// system
#include "SubSystem/Editor/EditorSystem.h"
#include "SubSystem/Core/Event/EventManager.h"
#include "SubSystem/JobSystem/Sync/JobSystem.h"
#include "SubSystem/JobSystem/Async/AsyncJobSystem.h"

Context* g_context = nullptr;

bool Engine::Initialize(HINSTANCE hInstance)
{
	// system コンテナク作成
	m_context = std::make_unique<Context>();
	g_context = m_context.get();

	auto ret = StartUpScreen(hInstance);
	if (!ret) {
		LOG_ERROR("StartUpScreenに失敗");
		return false;
	}

	// task thread : any thread 用として初期化
	AsyncJobSystem::Get().Initialize(2);

	// 過去の設定データがあれば、データに沿ったシステムの登録を行う。
	Config::RegisterSubsystemsToContainer();

	ret = InitializeSubsystems();
	if (!ret) {
		LOG_ERROR("Subsystemの初期化に失敗");
		return false;
	}

	m_hInstance = hInstance;

	return true;
}

long Engine::MainLoop()
{
	Window& window = Window::Get();
	Timer* timer = m_context->GetSubsystem<Timer>();
	
	while (window.Tick())
	{
		if (timer->ReachedNextFrame())
		{
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::Update);
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::PostUpdate);

			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::Render);
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::PostRender);
		}
	}

	return window.GetMessage();
}

void Engine::Shutdown()
{
	EventManager::Get().Exit();
	EditorSystem::Get().Shutdown();

	UnregisterClass("windowClass", m_hInstance);

	// release subsystem
	m_context->Release();
}

bool Engine::StartUpScreen(HINSTANCE hInstance) const noexcept
{
	// 第４引数 : フルスクリーンモード指定
	if (!Window::Get().CreateWindowClass(hInstance, 1280, 720, "Test", true))
	{
		return false;
	}

	if (!EditorSystem::Get().Initialize())
	{
		return false;
	}

	ShowWindow(Window::Get().GetHandle(), SW_SHOW);
	UpdateWindow(Window::Get().GetHandle());

	return true;
}

bool Engine::InitializeSubsystems() noexcept
{
	// system 初期化時に使用される可能性があるため
	EventManager::Get().Initialize();

	if (!g_context->GetSubsystem<Timer>()->Initialize())
	{
		LOG_ERROR("Timer初期化に失敗");
		return false;
	}

	if (!g_context->GetSubsystem<ResourceManager>()->Initialize())
	{
		LOG_ERROR("ResourceManager初期化に失敗");
		return false;
	}
	
	if (!g_context->GetSubsystem<Input>()->Initialize())
	{
		LOG_ERROR("Input初期化に失敗");
		return false;
	}
	
	if (!g_context->GetSubsystem<Audio>()->Initialize())
	{
		LOG_ERROR("Audio初期化に失敗");
		return false;
	}
	
	if (!g_context->GetSubsystem<Renderer>()->Initialize())
	{
		LOG_ERROR("Renderer初期化に失敗");
		return false;
	}
	 
	if (!g_context->GetSubsystem<World>()->Initialize())
	{
		LOG_ERROR("World初期化に失敗");
		return false;
	}

	EditorSystem::Get().PostInitialize();

	return true;
}