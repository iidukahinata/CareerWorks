/**
* @file	   DeferredRenderer.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "DeferredRenderer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/EditorSystem.h"
#include "SubSystem/Scene/Component/ICamera.h"
#include "SubSystem/Scene/Component/IRenderObject.h"
#include "SubSystem/Scene/Component/IPostProcessEffect.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

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

		Vector<D3D_SHADER_MACRO> defines;
		defines.reserve(2);
		defines.push_back(D3D_SHADER_MACRO("USE_INSTANCING", "1"));
		defines.push_back(D3D_SHADER_MACRO(NULL, NULL));

		D3D12Shader instancePreZShader;
		instancePreZShader.Compile(preZShaderPath, VertexShader, defines.data());

		desc.VS = &instancePreZShader;

		if (!m_instancePreZPipeline.Create(desc, &m_preZrootSignature))
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

	if (!m_lightingRenderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT))
	{
		return false;
	}

	return true;
}

void DeferredRenderer::CreateRenderList()
{
	m_bacthList.clear();
	m_instancingBuffer.clear();

	Map<Material*, std::pair<DrawBacth, Vector<DirectX::XMMATRIX>>> instancingMatListMap;

	for (auto renderObject : m_renderObjects)
	{
		auto&& batch = renderObject->GetBatch();
		for (auto mesh : batch.meshes)
		{
			auto material = mesh->GetMaterial();

			if (!material)
			{
				continue;
			}

			//
			// request instancing object is create instancing buffer data
			//
			if (material->IsInstancing())
			{
				auto isRegister = instancingMatListMap.contains(material);
				auto& drawBacth = instancingMatListMap[material].first;
				auto& matList   = instancingMatListMap[material].second;

				if (!isRegister)
				{
					drawBacth.material		 = material;
					drawBacth.isTranslucent  = material->IsTranslucent();
					drawBacth.constantBuffer = batch.constantBuffer;
					drawBacth.indexBuffer	 = mesh->GetIndexBuffer();
					drawBacth.indexNum		 = mesh->GetIndexNum();
					drawBacth.vertexBuffers.emplace_back(mesh->GetVertexBuffer());
				}

				drawBacth.instancingNum++;

				auto buffer = static_cast<ConstantBufferMatrix*>(batch.constantBuffer->GetCPUData());
				matList.emplace_back(buffer->worldViewProjection);
			}
			else
			{
				DrawBacth drawBacth;
				drawBacth.material		 = material;
				drawBacth.isTranslucent = material->IsTranslucent();
				drawBacth.constantBuffer = batch.constantBuffer;
				drawBacth.indexBuffer	 = mesh->GetIndexBuffer();
				drawBacth.indexNum		 = mesh->GetIndexNum();
				drawBacth.instancingNum  = 1;
				drawBacth.vertexBuffers.emplace_back(mesh->GetVertexBuffer());

				m_bacthList.emplace_back(std::move(drawBacth));
			}
		}
	}

	m_instancingBuffer.resize(instancingMatListMap.size());

	// create instancing vertex buffer
	for (int i = 0; auto& instancingData : instancingMatListMap)
	{	
		auto& bacth  = instancingData.second.first;
		auto& matVec = instancingData.second.second;

		// register vertex buffer
		m_instancingBuffer[i].Create(matVec);
		bacth.vertexBuffers.emplace_back(&m_instancingBuffer[i]);
		m_bacthList.emplace_back(bacth);
		++i;
	}
}

void DeferredRenderer::PrePass() noexcept
{
	CreateRenderList();

	m_lightMap->Update(m_mainCamera);
	m_transformCBuffer->Update(m_mainCamera);

	// RenderTarget Set
	m_gbuffer->SetRenderTargets();
	m_gbuffer->Clear();

	ZPrePass();
}

void DeferredRenderer::ZPrePass()
{
	for (auto renderObject : m_renderObjects)
	{
		renderObject->PreRender();
	}

	// Draw Mesh
	for (auto& bacth : m_bacthList)
	{
		if (bacth.indexNum == 1)
		{
			m_preZPipeline.Set();
		}
		else
		{
			m_instancePreZPipeline.Set();
		}

		bacth.constantBuffer->VSSet(0);

		// Set Mesh Buffer
		auto numBuffer = bacth.vertexBuffers.size();
		for (int i = 0; i < numBuffer; ++i)
		{
			bacth.vertexBuffers[i]->IASet(i);
		}

		bacth.indexBuffer->IASet();

		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(bacth.indexNum, bacth.instancingNum, 0, 0, 0);
	}
}

void DeferredRenderer::GBufferPass() noexcept
{
	// Draw Mesh
	for (auto& bacth : m_bacthList)
	{
		if (bacth.isTranslucent)
		{
			continue;
		}

		bacth.constantBuffer->VSSet(0);

		bacth.material->Render();

		// Set Mesh Buffer
		auto numBuffer = bacth.vertexBuffers.size();
		for (int i = 0; i < numBuffer; ++i)
		{
			bacth.vertexBuffers[i]->IASet(i);
		}

		bacth.indexBuffer->IASet();

		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(bacth.indexNum, bacth.instancingNum, 0, 0, 0);
	}
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

	DefferedLightingPass();

	ForwardLightingPass();
}

void DeferredRenderer::DefferedLightingPass()
{
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

void DeferredRenderer::ForwardLightingPass()
{
	D3D12RenderTargetView* renderTargetView;

	if (HasPostProcessSetting())
	{
		renderTargetView = m_lightingRenderTexture.GetRenderTargetView();
	}
	else
	{
		renderTargetView = m_renderTexture.GetRenderTargetView();
	}

	auto& context = D3D12GraphicsDevice::Get().GetCommandContext();
	context.SetRenderTargets(1, &renderTargetView, m_gbuffer->GetRenderTexture(GBufferType::Albedo).GetDepthStencilView());

	m_transformCBuffer->Update(m_mainCamera);

	// Draw Mesh
	for (auto& bacth : m_bacthList)
	{
		if (!bacth.isTranslucent)
		{
			continue;
		}

		bacth.constantBuffer->VSSet(0);

		bacth.material->Render();

		// Set Mesh Buffer
		auto numBuffer = bacth.vertexBuffers.size();
		for (int i = 0; i < numBuffer; ++i)
		{
			bacth.vertexBuffers[i]->IASet(i);
		}

		bacth.indexBuffer->IASet();

		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(bacth.indexNum, bacth.instancingNum, 0, 0, 0);
	}

	// Draw SkyBox
	m_skyBox->Render(m_mainCamera);

	m_transformCBuffer->Update(m_camera2DView.ToMatrixXM(), m_camera2DProj.ToMatrixXM());
	m_constantBuffer.VSSet(0);
}

void DeferredRenderer::PostPass() noexcept
{
	if (!HasPostProcessSetting())
	{
		return;
	}

	// PostEffectPass
	{
		// Texture Set
		m_gbuffer->GetRenderTexture(GBufferType::Albedo	 ).PSSet(0);
		m_gbuffer->GetRenderTexture(GBufferType::Specular).PSSet(1);
		m_gbuffer->GetRenderTexture(GBufferType::Normal	 ).PSSet(2);
		m_gbuffer->GetRenderTexture(GBufferType::Depth	 ).PSSet(3);
		m_gbuffer->GetRenderTexture(GBufferType::Position).PSSet(4);
		m_lightingRenderTexture.PSSet(5);

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