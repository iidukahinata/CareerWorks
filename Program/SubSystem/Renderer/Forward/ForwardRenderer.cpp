/**
* @file	   ForwardRenderer.cpp
* @brief
*
* @date	   2022/09/16 2022îNìxèâî≈
*/


#include "ForwardRenderer.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/EditorSystem.h"

bool ForwardRenderer::Initialize()
{
	Renderer::Initialize();

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();
	m_transformCBuffer->Initialize();

#if IS_EDITOR
	m_renderTexture.Create(Window::Get().GetWindowWidth(), Window::Get().GetWindowHeight());

	EditorSystem::Get().PostInitialize(m_renderTexture.GetShaderResourceView());
#endif // IS_EDITOR

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
	// ëSëÃÇÃï`âÊèÄîı
	D3D12GraphicsDevice::Get().BegineFrame();

#if IS_EDITOR
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
#else
	D3D12GraphicsDevice::Get().SetRenderTarget();
#endif // IS_EDITOR

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