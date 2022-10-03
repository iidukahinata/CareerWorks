/**
* @file	   ForwardRenderer.cpp
* @brief
*
* @date	   2022/09/16 2022年度初版
*/


#include "ForwardRenderer.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

bool ForwardRenderer::Initialize()
{
	Renderer::Initialize();

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();
	m_transformCBuffer->Initialize();

	RegisterRenderJob();

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
	D3D12GraphicsDevice::Get().BegineFrame();
	D3D12GraphicsDevice::Get().SetRenderTarget();

	if (m_mainCamera)
	{
		m_lightMap->Update(m_mainCamera);
		m_transformCBuffer->Update(m_mainCamera);

		for (auto renderObject : m_renderObjects)
		{
			if (!renderObject->GetActive())
				continue;

			renderObject->Render();
		}
	}
	else
	{
		// gui 表示用
		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(4, 1, 0, 0, 0);
	}
}

void ForwardRenderer::Present() noexcept
{
	D3D12GraphicsDevice::Get().EndFrame();
	D3D12GraphicsDevice::Get().Present();
}

void ForwardRenderer::RegisterRenderJob() noexcept
{
	m_jobs[0].SetFunction([this](double) { Update(); }, FunctionType::Render);
	m_jobs[1].SetFunction([this](double) { Present(); }, FunctionType::PostRender);

	for (int i = 0; i < m_jobs.size(); ++i)
	{
		m_jobs[i].RegisterToJobSystem();
	}
}