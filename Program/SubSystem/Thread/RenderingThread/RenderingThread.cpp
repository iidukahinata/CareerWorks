/**
* @file    RenderingThread.cpp
* @brief
*
* @date	   2022/11/01 2022�N�x����
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

		// command �̎��s
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
	// �`��R�}���h���s
	RegisterRenderCommand([] {

		JobSystem::Get().Execute(0.0f, FunctionType::Render);
		JobSystem::Get().Execute(0.0f, FunctionType::PostRender);

	});

	// �`��I�����}�擾�̂���
	m_renderCommandFance.BegineFrame();
}

void RenderingThread::EndFrame() noexcept
{
	// �`��I���܂őҋ@
	m_renderCommandFance.WaitForSingle();
}

void RenderingThread::WiatForRenderCommand() noexcept
{
	RenderCommandFance renderCommandFance;
	renderCommandFance.BegineFrame();
	renderCommandFance.WaitForSingle();
}

void RenderingThread::Run()
{
	m_renderingThreadID = std::this_thread::get_id();

	m_renderCommandList.SwapCommand();

	m_renderCommandList.CommandProcess();
}

bool IsInRenderingThread() noexcept
{
	return m_renderingThreadID == std::this_thread::get_id();
}