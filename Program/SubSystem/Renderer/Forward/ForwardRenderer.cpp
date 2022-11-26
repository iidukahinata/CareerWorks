/**
* @file	   ForwardRenderer.cpp
* @brief
*
* @date	   2022/10/30 2022年度初版
*/


#include "ForwardRenderer.h"
#include "SubSystem/Window/Window.h"
#include "../LightMap/DefaultLightMap.h"
#include "SubSystem/Editor/EditorSystem.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "SubSystem/Scene/Component/Components/RenderObject.h"

bool ForwardRenderer::Initialize()
{
	Renderer::Initialize();

	// Create SkyBox
	m_skyBox = std::make_unique<SkyBox>();
	m_skyBox->Initialize();

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();

	// SetUp Objects
	{
		if (!SetUpPrePassObjects())
		{
			return false;
		}
	}

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
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(RenderingThread, "Drawing");
#endif // IS_EDITOR

	// 全体の描画準備
	D3D12GraphicsDevice::Get().BegineFrame();

#if IS_EDITOR
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
#else
	D3D12GraphicsDevice::Get().SetRenderTarget();
#endif // IS_EDITOR

	if (m_mainCamera)
	{
		PrePass();

		LightingPass();
	}

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(RenderingThread);
#endif // IS_EDITOR
}

void ForwardRenderer::Present() noexcept
{
#ifdef IS_EDITOR
	TIME_LINE_WATCH_START(RenderingThread, "Wait For");
#endif // IS_EDITOR

	D3D12GraphicsDevice::Get().EndFrame();

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(RenderingThread);
#endif // IS_EDITOR

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

bool ForwardRenderer::SetUpPrePassObjects() noexcept
{
	// Create RootSignature
	{
		Array<CD3DX12_DESCRIPTOR_RANGE1, 1> descTblRanges = {};
		Array<CD3DX12_ROOT_PARAMETER1, 1> rootParams = {};

		//===定数バッファ用設定=====================================
		descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
		rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
		//==========================================================

		auto flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
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
		desc.VS				  = &preZShader;
		desc.BlendMode		  = BLEND_MODE_NO_ALPHA;
		desc.RasterizerState  = NO_CULL;
		desc.PrimitiveType	  = PRIMITIVE_TYPE_POINTLIST;
		desc.NumRenderTargets = 1;

		if (!m_preZPipeline.Create(desc, &m_preZrootSignature))
		{
			return false;
		}
	}

	return true;
}

void ForwardRenderer::PrePass() noexcept
{
	m_lightMap->Update(m_mainCamera);
	m_transformCBuffer->Update(m_mainCamera);

	// Z prepass
	{
		m_preZPipeline.Set();
		
		for (auto renderObject : m_renderObjects)
		{
			if (!renderObject->GetActive())
				continue;
		
			renderObject->PreRender();
		}
	}
}

void ForwardRenderer::LightingPass() noexcept
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