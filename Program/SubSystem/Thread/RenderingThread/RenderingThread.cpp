/**
* @file    RenderingThread.cpp
* @brief
*
* @date	   2022/11/01 2022年度初版
*/


#include "RenderingThread.h"
#include "../ThreadManager.h"
#include "RenderCommandFance.h"
#include "SubSystem/JobSystem/Sync/JobSystem.h"

std::mutex RenderCommandList::m_mutex;
Queue<RenderCommandList::Command> RenderCommandList::m_commandList;
Queue<RenderCommandList::Command> RenderCommandList::m_taskList;

RenderCommandFance RenderingThread::m_renderCommandFance;

static std::thread::id m_renderingThreadID = {};

bool RenderCommandList::HasTask() noexcept
{
	return m_taskList.size() != 0;
}

void RenderCommandList::SwapCommand() noexcept
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_commandList.swap(m_taskList);
}

void RenderCommandList::CommandProcess() noexcept
{
	while (!m_commandList.empty())
	{
		auto&& command = std::move(m_commandList.front());

		// command の実行
		command();

		m_commandList.pop();
	}
}

void RenderingThread::Start() noexcept
{
	// register
	ThreadManager::Get().CreateThread<RenderingThread>();
}

void RenderingThread::Stop() noexcept
{
	// unregister
	ThreadManager::Get().QuitThread(RenderingThread::TypeData.Name);
}

void RenderingThread::BegineFrame() noexcept
{
#ifdef IS_EDITOR
	TIME_LINE_NEW_FRAME();
#endif // IS_EDITOR

	// 描画コマンド発行
	RegisterRenderCommand([] {

		JobSystem::Get().Execute(0.0f, FunctionType::Render);
		JobSystem::Get().Execute(0.0f, FunctionType::PostRender);

	});

	// 描画終了合図取得のため
	m_renderCommandFance.BegineFrame();
}

void RenderingThread::EndFrame() noexcept
{
	// 描画終了まで待機
	m_renderCommandFance.WaitForSingle();

#ifdef IS_EDITOR
	TIME_LINE_END_FRAME();
#endif // IS_EDITOR
}

void RenderingThread::WiatForRenderCommand() noexcept
{
	RenderCommandFance renderCommandFance;
	renderCommandFance.BegineFrame();
	renderCommandFance.WaitForSingle();
}

void RenderingThread::Run()
{
#ifdef IS_EDITOR
	auto showTimeLine = ImTimeLine::ShowTimeLine();
	if (showTimeLine) {
		TIME_LINE_WATCH_START(RenderingThread, "Command Process");
	}
#endif // IS_EDITOR

	m_renderingThreadID = std::this_thread::get_id();

	m_renderCommandList.SwapCommand();

	m_renderCommandList.CommandProcess();

#ifdef IS_EDITOR
	if (showTimeLine) {
		TIME_LINE_WATCH_END(RenderingThread);
	}
#endif // IS_EDITOR
}

bool IsInRenderingThread() noexcept
{
	return m_renderingThreadID == std::this_thread::get_id();
}