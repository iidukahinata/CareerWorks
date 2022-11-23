/**
* @file    Engine.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "Engine.h"

// Subsystem
#include "SubSystem/Timer/Timer.h"
#include "SubSystem/Input/Input.h"
#include "SubSystem/Audio/Audio.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Physics/Physics.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Script/ScriptEngine.h"
#include "SubSystem/Resource/ResourceManager.h"

// system
#include "SubSystem/Editor/EditorSystem.h"
#include "SubSystem/Core/Event/EventManager.h"
#include "SubSystem/JobSystem/Sync/JobSystem.h"
#include "SubSystem/JobSystem/Async/AsyncJobSystem.h"

#include "SubSystem/Thread/ThreadManager.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"
#include "SubSystem/Thread/LoadingThread/LoadingThread.h"

Context* g_context = nullptr;

bool Engine::Initialize(HINSTANCE hInstance)
{
	// 全て生成済みの場合は何も起きない。
	Config::GenerateUseFile();

	// system コンテナク作成
	m_context = std::make_unique<Context>();
	g_context = m_context.get();

	auto ret = StartUpScreen(hInstance);
	if (!ret) {
		LOG_ERROR("StartUpScreenに失敗");
		return false;
	}

	// renderingThread : task thread : any thread 用として初期化
	AsyncJobSystem::Get().Initialize(3);

	ThreadManager::Get().Initialize();

	// 過去の設定データがあれば、データに沿ったシステムの登録を行う。
	Config::RegisterSubsystemsToContainer();

	RenderingThread::Start();
	LoadingThread::Start();

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
	auto& window = Window::Get();
	auto timer = m_context->GetSubsystem<Timer>();	

	while (window.Tick())
	{
		if (timer->ReachedNextFrame())
		{
			RenderingThread::BegineFrame();

#ifdef IS_EDITOR
			auto showTimeLine = ImTimeLine::ShowTimeLine();
			if (showTimeLine) {
				TIME_LINE_WATCH_START(MainThread, "Update");
			}
#endif // IS_EDITOR

			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::PreUpdate);
			
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::PrePhysics);
			
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::StartPhysics);
			
			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::EndPhysics);

			JobSystem::Get().Execute(timer->GetDeltaTime(), FunctionType::PostUpdate);

#ifdef IS_EDITOR
			if (showTimeLine) {
				TIME_LINE_WATCH_END(MainThread);
			}
#endif // IS_EDITOR

			// vsync
			RenderingThread::EndFrame();
		}
	}

	return window.GetMessage();
}

void Engine::Shutdown()
{
	RenderingThread::Stop();
	LoadingThread::Stop();

	EventManager::Get().Exit();

#ifdef IS_EDITOR
	EditorSystem::Get().Shutdown();
#endif // IS_EDITOR
	Window::Get().Shutdown();

	// release subsystem
	m_context->Release();
}

bool Engine::StartUpScreen(HINSTANCE hInstance) const noexcept
{
	const auto width  = GetSystemMetrics(SM_CXSCREEN);
	const auto height = GetSystemMetrics(SM_CYSCREEN);

	// 第４引数 : フルスクリーンモード指定
	if (!Window::Get().CreateWindowClass(hInstance, width, height, "HEngine", true))
	{
		return false;
	}

#ifdef IS_EDITOR
	if (!EditorSystem::Get().Initialize())
	{
		return false;
	}
#endif // IS_EDITOR

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

	if (!g_context->GetSubsystem<Physics>()->Initialize())
	{
		LOG_ERROR("Physics初期化に失敗");
		return false;
	}

	if (!g_context->GetSubsystem<ScriptEngine>()->Initialize())
	{
		LOG_ERROR("ScriptEngine初期化に失敗");
		return false;
	}

	if (!g_context->GetSubsystem<World>()->Initialize())
	{
		LOG_ERROR("World初期化に失敗");
		return false;
	}
	
	if (!g_context->GetSubsystem<Renderer>()->Initialize())
	{
		LOG_ERROR("Renderer初期化に失敗");
		return false;
	}

	return true;
}