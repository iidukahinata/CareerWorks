/**
* @file	   DeferredRenderer.cpp
* @brief
*
* @date	   2022/09/02 2022年度初版
*/


#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "SubSystem/Window/Window.h"
#include "../GraphicsAPI/D3D12/D3D12GrahicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Resource/ResourceData/ProprietaryShaderData.h"

bool DeferredRenderer::Initialize()
{
	Renderer::Initialize();

	const auto width = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	m_gbuffer = std::make_unique<GBuffer>();
	m_gbuffer->Initialize(width, height);

	m_lightMap = std::make_unique<LightMap>();
	m_lightMap->Initialize();

	m_transformCBuffer = std::make_unique<TransformCBuffer>();
	m_transformCBuffer->Initialize();

	if (!SetUpDeferredObjects(width, height))
	{
		return false;
	}

	if (!SetUpSpriteObjects(width, height))
	{
		return false;
	}

	m_job.SetFunction([this](double) { Update(); }, FunctionType::Render);
	m_job.RegisterToJobSystem();

	return true;
}

void DeferredRenderer::Shutdown()
{
	m_job.UnRegisterFromJobSystem();
}

void DeferredRenderer::Update() noexcept
{
	auto& grahicsDevice = D3D12GrahicsDevice::Get();

	// 全体の描画準備
	grahicsDevice.BegineFrame();

	m_transformCBuffer->Update(m_mainCamera);
	m_lightMap->Update(m_mainCamera);

	for (auto renderObject : m_renderObjects)
	{
		renderObject->Render();
	}

	grahicsDevice.EndFrame();

	// フリップ
	grahicsDevice.Present();
}

bool DeferredRenderer::SetUpDeferredObjects(UINT width, UINT height) noexcept
{
	auto deferredShader = ProprietaryShaderData::ConvertProprietaryPath("Deferred.cso");

	m_deferredShaders[VertexShader].Compile(deferredShader, VertexShader);
	m_deferredShaders[PixelShader].Compile(deferredShader, PixelShader);

	m_sampler.Create();

	return m_renderTexture.Create(width, height);
}

bool DeferredRenderer::SetUpSpriteObjects(UINT width, UINT height) noexcept
{
	auto spriteShader = ProprietaryShaderData::ConvertProprietaryPath("Texture.cso");

	m_spriteShaders[VertexShader].Compile(spriteShader, VertexShader);
	m_spriteShaders[PixelShader].Compile(spriteShader, PixelShader);

	auto&& triangleList = Quad::Create<Vertex2D>(width, height);

	if (!m_vertexBuffer.Create(triangleList.m_vertices))
	{
		return false;
	}

	if (!m_indexBuffer.Create(triangleList.m_indices))
	{
		return false;
	}

	return true;
}