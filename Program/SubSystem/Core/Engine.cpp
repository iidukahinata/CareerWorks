/**
* @file    Engine.cpp
* @brief
*
* @date	   2022/11/27 2022年度初版
*/


#include "Engine.h"

// Subsystem
#include "SubSystem/Timer/Timer.h"
#include "SubSystem/Input/IInput.h"
#include "SubSystem/Audio/IAudio.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Physics/IPhysics.h"
#include "SubSystem/Renderer/IRenderer.h"
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

	// system コンテナクラス作成
	m_context = std::make_unique<Context>();
	g_context = m_context.get();

	auto ret = StartupScreen(hInstance);
	if (!ret) {
		LOG_ERROR("error screen startup");
		return false;
	}

	ret = StartupThread();
	if (!ret) {
		LOG_ERROR("error thread startup");
		return false;
	}

	// 過去の設定データがあれば、データに沿ったシステムの登録を行う。
	Config::RegisterSubsystemsToContainer();

	ret = InitializeSubsystems();
	if (!ret) {
		LOG_ERROR("error init subsystems");
		return false;
	}

	m_hInstance = hInstance;
	m_timer = m_context->GetSubsystem<Timer>();

	return true;
}

long Engine::MainLoop()
{
	while (Window::Get().Tick())
	{
		if (m_timer->ReachedNextFrame())
		{
#ifdef IS_EDITOR
			TIME_LINE_NEW_FRAME();
#endif // IS_EDITOR

			Tick();

#ifdef IS_EDITOR
			TIME_LINE_END_FRAME();
#endif // IS_EDITOR
		}
	}

	return Window::Get().GetMessage();
}

void Engine::Shutdown()
{
	// stop all thread
	RenderingThread::Stop();
	LoadingThread::Stop();

	EventManager::Get().Exit();

#ifdef IS_EDITOR
	EditorSystem::Get().Shutdown();
#endif // IS_EDITOR

	Window::Get().Shutdown();

	// release subsystem
	m_context->Release();
	g_context = nullptr;
}

bool Engine::StartupScreen(HINSTANCE hInstance) const noexcept
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

bool Engine::StartupThread()
{
	// renderingThread : task thread : LoadingThread : any thread として初期化
	if (!AsyncJobSystem::Get().Initialize(ThreadType::MaxUseThread))
	{
		return false;
	}

	ThreadManager::Get().Initialize();

	RenderingThread::Start();
	LoadingThread::Start();

	return true;
}

bool Engine::InitializeSubsystems() noexcept
{
#ifdef IS_EDITOR
	EditorSystem::Get().LastInitialize();
#endif // IS_EDITOR

	EventManager::Get().Initialize();

	return true;
}

void Engine::Tick() const noexcept
{
	RenderingThread::BegineFrame();

#ifdef IS_EDITOR
	auto showTimeLine = ImTimeLine::ShowTimeLine();
	if (showTimeLine) {
		TIME_LINE_WATCH_START(MainThread, "Update");
	}
#endif // IS_EDITOR

	JobSystem::Get().Execute(m_timer->GetDeltaTime(), FunctionType::PreUpdate);

	JobSystem::Get().Execute(m_timer->GetDeltaTime(), FunctionType::PrePhysics);

	JobSystem::Get().Execute(m_timer->GetDeltaTime(), FunctionType::StartPhysics);

	JobSystem::Get().Execute(m_timer->GetDeltaTime(), FunctionType::EndPhysics);

	JobSystem::Get().Execute(m_timer->GetDeltaTime(), FunctionType::PostUpdate);

#ifdef IS_EDITOR
	if (showTimeLine) {
		TIME_LINE_WATCH_END(MainThread);
	}
#endif // IS_EDITOR

	// vsync
	RenderingThread::EndFrame();
}