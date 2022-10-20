/**
* @file	   DeferredRenderer.cpp
* @brief
*
* @date	   2022/09/17 2022年度初版
*/


#include "DeferredRenderer.h"
#include "GBuffer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "SubSystem/Window/Window.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Resource/ResourceData/ProprietaryShaderData.h"

bool DeferredRenderer::Initialize()
{
	Renderer::Initialize();

	const auto width = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	// Create GBuffer
	m_gbuffer = std::make_unique<GBuffer>();
	m_gbuffer->Initialize(width, height);

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();
	m_transformCBuffer->Initialize();

	// SetUp Object
	{
		if (!SetUpRenderingObjects(width, height))
		{
			return false;
		}

		if (!SetUpLightingObjects(width, height))
		{
			return false;
		}

		if (!SetUpPostProcessObjects(width, height))
		{
			return false;
		}
	}

	RegisterRenderJob();

	return true;
}

void DeferredRenderer::Shutdown()
{
	for (int i = 0; i < m_jobs.size(); ++i)
	{
		m_jobs[i].UnRegisterFromJobSystem();
	}
}

void DeferredRenderer::Update() noexcept
{
	// 全体の描画準備
	D3D12GraphicsDevice::Get().BegineFrame();

	if (m_mainCamera)
	{
		PrePass();

		GBufferPass();

		LightingPass();

		PostPass();
	}
	else
	{
		// gui 表示用
		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(4, 1, 0, 0, 0);
	}
}

void DeferredRenderer::Present() noexcept
{
	D3D12GraphicsDevice::Get().EndFrame();
	D3D12GraphicsDevice::Get().Present();
}

void DeferredRenderer::RegisterRenderJob() noexcept
{
	m_jobs[0].SetFunction([this](double) { Update(); }, FunctionType::Render);
	m_jobs[1].SetFunction([this](double) { Present(); }, FunctionType::PostRender);

	for (int i = 0; i < m_jobs.size(); ++i)
	{
		m_jobs[i].RegisterToJobSystem();
	}
}

bool DeferredRenderer::SetUpRenderingObjects(UINT width, UINT height) noexcept
{
	// Create Sampler
	m_sampler.Create();

	// Create Mesh Buffer
	{
		auto&& triangleList = Quad::Create<Vertex2D>(width, height);

		if (!m_vertexBuffer.Create(triangleList.m_vertices))
		{
			return false;
		}

		if (!m_indexBuffer.Create(triangleList.m_indices))
		{
			return false;
		}
	}

	// Create RootSignature
	{
		Array<CD3DX12_DESCRIPTOR_RANGE1, 3> descTblRanges = {};
		Array<CD3DX12_ROOT_PARAMETER1, 3> rootParams = {};

		//===定数バッファ用設定=====================================
		descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
		//==========================================================

		//===テクスチャ用設定=======================================
		descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 16, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
		//==========================================================

		//===Sampler用設定==========================================
		descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
		//==========================================================

		if (!m_rootSignature.Create(rootParams.size(), rootParams.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT))
		{
			return false;
		}
	}

	return true;
}

bool DeferredRenderer::SetUpLightingObjects(UINT width, UINT height) noexcept
{
	auto deferredShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "Deferred.hlsl");

	Array<D3D12Shader, 2> deferredShaders;
	deferredShaders[VertexShader].Compile(deferredShaderPath, VertexShader);
	deferredShaders[PixelShader ].Compile(deferredShaderPath, PixelShader );

	GraphicsPipelineStateDesc desc = {};
	desc.VS = &deferredShaders[VertexShader];
	desc.PS = &deferredShaders[PixelShader ];
	desc.BlendMode = BLEND_MODE_NO_ALPHA;
	desc.RasterizerState = NO_CULL;
	desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = GBufferType::Max;

	for (size_t i = 0; i < GBufferType::Max; ++i)
	{
		desc.RTVForamt[i] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	if (!m_deferredPipeline.Create(desc, &m_rootSignature))
	{
		return false;
	}

	return true;
}

bool DeferredRenderer::SetUpPostProcessObjects(UINT width, UINT height) noexcept
{
	auto postProcessShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

	Array<D3D12Shader, 2> postProcessShaders;
	postProcessShaders[VertexShader].Compile(postProcessShaderPath, VertexShader);
	postProcessShaders[PixelShader ].Compile(postProcessShaderPath, PixelShader );

	GraphicsPipelineStateDesc desc = {};
	desc.VS = &postProcessShaders[VertexShader];
	desc.PS = &postProcessShaders[PixelShader ];
	desc.BlendMode = BLEND_MODE_NO_ALPHA;
	desc.RasterizerState = NO_CULL;
	desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;

	//if (!m_postProcessPipeline.Create(desc, &m_rootSignature))
	//{
	//	return false;
	//}

	return true;
}

void DeferredRenderer::PrePass() noexcept
{
	m_lightMap->Update(m_mainCamera);
	m_transformCBuffer->Update(m_mainCamera);
}

void DeferredRenderer::GBufferPass() noexcept
{
	// RenderTarget Set
	m_gbuffer->SetRenderTargets();
	m_gbuffer->Clear();

	// Draw
	for (auto renderObject : m_renderObjects)
	{
		renderObject->Render();
	}
}

void DeferredRenderer::LightingPass() noexcept
{
	// RenderTarget Set
	if (HasPostProcessSetting())
	{
		m_lightingRenderTexture.SetRenderTarget();
		m_lightingRenderTexture.Clear(Math::Vector4(0.f, 0.f, 0.f, 1.f));
	}
	else
	{
		D3D12GraphicsDevice::Get().SetRenderTarget();
	}

	// Pipeline Set
	m_deferredPipeline.Set();
	m_sampler.PSSet();

	// Texture Set
	m_gbuffer->GetRenderTexture(GBufferType::Albedo	 ).PSSet(0);
	m_gbuffer->GetRenderTexture(GBufferType::Specular).PSSet(1);
	m_gbuffer->GetRenderTexture(GBufferType::Normal	 ).PSSet(2);
	m_gbuffer->GetRenderTexture(GBufferType::Depth	 ).PSSet(3);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void DeferredRenderer::PostPass() noexcept
{
	if (!HasPostProcessSetting())
	{
		return;
	}

	D3D12GraphicsDevice::Get().SetRenderTarget();

	// Pipeline Set
	m_postProcessPipeline.Set();
	m_sampler.PSSet();

	// Texture Set
	m_gbuffer->GetRenderTexture(GBufferType::Albedo	 ).PSSet(0);
	m_gbuffer->GetRenderTexture(GBufferType::Specular).PSSet(1);
	m_gbuffer->GetRenderTexture(GBufferType::Normal	 ).PSSet(2);
	m_gbuffer->GetRenderTexture(GBufferType::Depth	 ).PSSet(3);
	m_lightingRenderTexture.PSSet(4);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
}