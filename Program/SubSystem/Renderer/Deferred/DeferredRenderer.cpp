/**
* @file	   DeferredRenderer.cpp
* @brief
*
* @date	   2022/11/27 2022年度初版
*/


#include "DeferredRenderer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/EditorSystem.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"
#include "SubSystem/Scene/Component/Components/PostProcessEffect.h"

bool DeferredRenderer::Initialize()
{
	IRenderer::Initialize();

	const auto width  = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	// Create GBuffer
	m_gbuffer = std::make_unique<GBuffer>();
	m_gbuffer->Initialize(width, height);

	// Create SkyBox
	m_skyBox = std::make_unique<SkyBox>();
	m_skyBox->Initialize();

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();

	auto ret = SetupObjects();
	if(!ret) {
		return false;
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
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(RenderingThread, "Drawing");
#endif // IS_EDITOR

	// 全体の描画準備
	D3D12GraphicsDevice::Get().BegineFrame();

	if (m_mainCamera)
	{
		PrePass();

		GBufferPass();

		PreLightingPass();

		LightingPass();
		
		PostPass();
	}

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(RenderingThread);
#endif // IS_EDITOR
}

void DeferredRenderer::Present() noexcept
{
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(RenderingThread, "Wait For");
#endif // IS_EDITOR

	D3D12GraphicsDevice::Get().EndFrame();
	D3D12GraphicsDevice::Get().Present();

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(RenderingThread);
#endif // IS_EDITOR
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

bool DeferredRenderer::SetupObjects() noexcept
{
	const auto width = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	if (!SetupPrePassObjects())
	{
		return false;
	}

	if (!SetupRenderingObjects(width, height))
	{
		return false;
	}

	if (!SetupLightingObjects(width, height))
	{
		return false;
	}

	if (!SetupPostProcessObjects(width, height))
	{
		return false;
	}

#if IS_EDITOR

	if (!m_renderTexture.Create(Window::Get().GetWindowWidth(), Window::Get().GetWindowHeight()))
	{
		return false;
	}

	if (!EditorSystem::Get().PostInitialize(m_renderTexture.GetShaderResourceView()))
	{
		return false;
	}

#endif // IS_EDITOR

	return true;
}

bool DeferredRenderer::SetupPrePassObjects() noexcept
{
	// Create RootSignature
	{
		Array<CD3DX12_DESCRIPTOR_RANGE1, 1> descTblRanges = {};
		Array<CD3DX12_ROOT_PARAMETER1, 1> rootParams = {};

		//===定数バッファ用設定=====================================
		descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
		//==========================================================

		auto flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |	D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		if (!m_preZrootSignature.Create(rootParams.size(), rootParams.data(), 0, nullptr, flags))
		{
			return false;
		}
	}

	// Create PipelineState
	{
		const auto preZShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PreZ.hlsl");

		D3D12Shader preZShader;
		preZShader.Compile(preZShaderPath, VertexShader);

		GraphicsPipelineStateDesc desc = {};
		desc.VS = &preZShader;
		desc.BlendMode = BLEND_MODE_NO_ALPHA;
		desc.RasterizerState = NO_CULL;
		desc.PrimitiveType = PRIMITIVE_TYPE_POINTLIST;
		desc.NumRenderTargets = 1;

		if (!m_preZPipeline.Create(desc, &m_preZrootSignature))
		{
			return false;
		}
	}

	return true;
}

bool DeferredRenderer::SetupRenderingObjects(UINT width, UINT height) noexcept
{
	// Create Matrix
	m_camera2DProj = Math::Matrix::CreateOrthographicLH(width, height, 0.1f, 100.0f);
	m_camera2DView = Math::Matrix(Math::Vector3(0, 0, 1), Math::Vector3::Zero, Math::Vector3::One);
	m_constantBuffer.Create(sizeof(ConstantBufferMatrix));

	// Create Sampler
	m_sampler.Create(D3D12_FILTER_MIN_MAG_MIP_LINEAR);

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
	if (!m_rootSignature.Create(m_rootSignature.GetGrapihcsRootDesc()))
	{
		return false;
	}

	return true;
}

bool DeferredRenderer::SetupLightingObjects(UINT width, UINT height) noexcept
{
	auto deferredShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "Deferred.hlsl");

	Array<D3D12Shader, 2> deferredShaders;
	deferredShaders[VertexShader].Compile(deferredShaderPath, VertexShader);
	deferredShaders[PixelShader ].Compile(deferredShaderPath, PixelShader );

	GraphicsPipelineStateDesc desc = {};
	desc.VS				  = &deferredShaders[VertexShader];
	desc.PS				  = &deferredShaders[PixelShader ];
	desc.BlendMode		  = BLEND_MODE_NO_ALPHA;
	desc.RasterizerState  = NO_CULL;
	desc.PrimitiveType	  = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;
	desc.RTVForamt[0]	  = DXGI_FORMAT_R32G32B32A32_FLOAT;

	if (!m_deferredPipeline.Create(desc, &m_rootSignature))
	{
		return false;
	}

	return true;
}

bool DeferredRenderer::SetupPostProcessObjects(UINT width, UINT height) noexcept
{
	// Create Luminous Pipeline
	{
		auto luminousShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("Luma", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> luminousShaders;
		luminousShaders[VertexShader].Compile(luminousShaderPath, VertexShader, defines.data());
		luminousShaders[PixelShader].Compile(luminousShaderPath, PixelShader, defines.data());

		GraphicsPipelineStateDesc desc = {};
		desc.VS = &luminousShaders[VertexShader];
		desc.PS = &luminousShaders[PixelShader];
		desc.BlendMode = BLEND_MODE_ALPHA;
		desc.RasterizerState = NO_CULL;
		desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
		desc.NumRenderTargets = 1;
		desc.RTVForamt[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		if (!m_luminousPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	// Create PostProcess Pipeline
	{
		auto postProcessShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "Texture2D.hlsl");

		Array<D3D12Shader, 2> postProcessShaders;
		postProcessShaders[VertexShader].Compile(postProcessShaderPath, VertexShader);
		postProcessShaders[PixelShader].Compile(postProcessShaderPath, PixelShader);

		GraphicsPipelineStateDesc desc = {};
		desc.VS = &postProcessShaders[VertexShader];
		desc.PS = &postProcessShaders[PixelShader];
		desc.BlendMode = BLEND_MODE_ALPHA;
		desc.RasterizerState = NO_CULL;
		desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
		desc.NumRenderTargets = 1;
		desc.RTVForamt[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		if (!m_postProcessPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	if (!m_luminousRenderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT))
	{
		return false;
	}

	if (!m_lightingRenderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT))
	{
		return false;
	}

	return true;
}

void DeferredRenderer::PrePass() noexcept
{
	m_lightMap->Update(m_mainCamera);
	m_transformCBuffer->Update(m_mainCamera);

	// RenderTarget Set
	m_gbuffer->SetRenderTargets();
	m_gbuffer->Clear();

	ZPrePass();
}

void DeferredRenderer::ZPrePass()
{
	m_preZPipeline.Set();

	// Draw
	for (auto renderObject : m_renderObjects)
	{
		if (!renderObject->GetActive())
			continue;

		renderObject->PreRender();
	}
}

void DeferredRenderer::GBufferPass() noexcept
{
	// Draw
	for (auto renderObject : m_renderObjects)
	{
		if (!renderObject->GetActive())
			continue;

		renderObject->Render();
	}

	// Draw SkyBox
	m_skyBox->Render(m_mainCamera);
}

void DeferredRenderer::PreLightingPass() noexcept
{
	m_transformCBuffer->Update(m_camera2DView.ToMatrixXM(), m_camera2DProj.ToMatrixXM());
	m_transformCBuffer->Bind(m_constantBuffer.GetCPUData(), Math::Matrix::Identity.ToMatrixXM());

	m_constantBuffer.VSSet(0);
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
#if IS_EDITOR
		m_renderTexture.SetRenderTarget();
		m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
#else
		D3D12GraphicsDevice::Get().SetRenderTarget();
#endif // IS_EDITOR
	}

	// Pipeline Set
	m_deferredPipeline.Set();
	m_sampler.PSSet();

	// Texture Set
	m_gbuffer->GetRenderTexture(GBufferType::Albedo	 ).PSSet(0);
	m_gbuffer->GetRenderTexture(GBufferType::Specular).PSSet(1);
	m_gbuffer->GetRenderTexture(GBufferType::Normal	 ).PSSet(2);
	m_gbuffer->GetRenderTexture(GBufferType::Depth	 ).PSSet(3);
	m_gbuffer->GetRenderTexture(GBufferType::Position).PSSet(4);

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

	LuminousPass();

	// PostEffectPass
	{
		m_luminousRenderTexture.WaitUntilFinishDrawing();

		// Texture Set
		m_gbuffer->GetRenderTexture(GBufferType::Albedo	 ).PSSet(0);
		m_gbuffer->GetRenderTexture(GBufferType::Specular).PSSet(1);
		m_gbuffer->GetRenderTexture(GBufferType::Normal	 ).PSSet(2);
		m_gbuffer->GetRenderTexture(GBufferType::Depth	 ).PSSet(3);
		m_gbuffer->GetRenderTexture(GBufferType::Position).PSSet(4);
		m_lightingRenderTexture.PSSet(5);
		m_luminousRenderTexture.PSSet(6);
		m_lightingRenderTexture.PSSet(7);

		m_postProcessEffect->Render();
	}

#if IS_EDITOR
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
#else
	D3D12GraphicsDevice::Get().SetRenderTarget();
#endif // IS_EDITOR

	// Pipeline Set
	m_postProcessPipeline.Set();
	m_sampler.PSSet();

	// Matrix Set
	m_constantBuffer.VSSet(0);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void DeferredRenderer::LuminousPass() noexcept
{
	m_lightingRenderTexture.WaitUntilFinishDrawing();

	// RenderTarget Set
	m_luminousRenderTexture.SetRenderTarget();
	m_luminousRenderTexture.Clear(Math::Vector4(0.f, 0.f, 0.f, 1.f));

	// Pipeline Set
	m_luminousPipeline.Set();
	m_sampler.PSSet();

	// Texture Set
	m_lightingRenderTexture.PSSet(5);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
}