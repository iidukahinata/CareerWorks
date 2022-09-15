/**
* @file	   ForwardRenderer.cpp
* @brief
*
* @date	   2022/09/07 2022年度初版
*/


#include "ForwardRenderer.h"
#include "../GraphicsAPI/D3D12/D3D12GrahicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

bool ForwardRenderer::Initialize()
{
	Renderer::Initialize();

	m_lightMap = std::make_unique<LightMap>();
	m_lightMap->Initialize();

	m_transformCBuffer = std::make_unique<TransformCBuffer>();
	m_transformCBuffer->Initialize();

	m_jobs[0].SetFunction([this](double) { Update(); }, FunctionType::Render);
	m_jobs[1].SetFunction([this](double) { Present(); }, FunctionType::PostRender);

	for (int i = 0; i < m_jobs.size(); ++i)
	{
		m_jobs[i].RegisterToJobSystem();
	}

	return true;
}

void ForwardRenderer::Shutdown()
{
	for (int i = 0; i < m_jobs.size(); ++i)
	{
		m_jobs[i].UnRegisterFromJobSystem();
	}
}

void ForwardRenderer::Update() noexcept
{
	// 全体の描画準備
	D3D12GrahicsDevice::Get().BegineFrame();

	if (m_mainCamera)
	{
		m_lightMap->Update(m_mainCamera);
		m_transformCBuffer->Update(m_mainCamera);

		for (auto renderObject : m_renderObjects)
		{
			renderObject->Render();
		}
	}
	else
	{
		// gui 表示用
		D3D12GrahicsDevice::Get().GetCommandContext().DrawIndexedInstanced(4, 1, 0, 0, 0);
	}
}

void ForwardRenderer::Present() noexcept
{
	D3D12GrahicsDevice::Get().EndFrame();
	D3D12GrahicsDevice::Get().Present();
}