/**
* @file	   ForwardRenderer.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/


#include "ForwardRenderer.h"
#include "../LightMap/DefaultLightMap.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Window/Window.h"
#include "SubSystem/Editor/EditorSystem.h"
#include "SubSystem/Scene/Component/ICamera.h"
#include "SubSystem/Scene/Component/IRenderObject.h"
#include "SubSystem/Resource/Resources/3DModel/Mesh.h"
#include "SubSystem/Resource/Resources/3DModel/Material.h"

bool ForwardRenderer::Initialize()
{
	IRenderer::Initialize();

	// Create SkyBox
	m_skyBox = std::make_unique<SkyBox>();
	m_skyBox->Initialize();

	// Create LightMap
	m_lightMap = std::make_unique<DefaultLightMap>();
	m_lightMap->Initialize();

	// Create TransCBuffer
	m_transformCBuffer = std::make_unique<TransformCBuffer>();

	auto ret = SetupObjects();
	if (!ret) {
		return false;
	}

	RegisterRenderJob();

	return true;
}

bool ForwardRenderer::SetupObjects() noexcept
{
	if (!SetupPrePassObjects())
	{
		return false;
	}

#if IS_EDITOR
	m_renderTexture.Create(Window::Get().GetWindowWidth(), Window::Get().GetWindowHeight());
	EditorSystem::Get().PostInitialize(m_renderTexture.GetShaderResourceView());
#endif // IS_EDITOR

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
	D3D12GraphicsDevice::Get().Present();

#ifdef IS_EDITOR
	TIME_LINE_WATCH_END(RenderingThread);
#endif // IS_EDITOR
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

bool ForwardRenderer::SetupPrePassObjects() noexcept
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

void ForwardRenderer::CreateRenderList()
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
				auto& bacth = instancingMatListMap[material].first;
				auto& matList = instancingMatListMap[material].second;

				if (!isRegister)
				{
					bacth.material = material;
					bacth.constantBuffer = batch.constantBuffer;
					bacth.indexBuffer = mesh->GetIndexBuffer();
					bacth.indexNum = mesh->GetIndexNum();
					bacth.vertexBuffers.emplace_back(mesh->GetVertexBuffer());
				}

				bacth.instancingNum++;

				auto buffer = static_cast<ConstantBufferMatrix*>(batch.constantBuffer->GetCPUData());
				matList.emplace_back(buffer->worldViewProjection);
			}
			else
			{
				DrawBacth drawBacth;
				drawBacth.material = material;
				drawBacth.constantBuffer = batch.constantBuffer;
				drawBacth.indexBuffer = mesh->GetIndexBuffer();
				drawBacth.indexNum = mesh->GetIndexNum();
				drawBacth.instancingNum = 1;
				drawBacth.vertexBuffers.emplace_back(mesh->GetVertexBuffer());

				m_bacthList.emplace_back(drawBacth);
			}
		}
	}

	m_instancingBuffer.resize(instancingMatListMap.size());

	// create instancing vertex buffer
	for (int i = 0; auto & instancingData : instancingMatListMap)
	{
		auto& bacth = instancingData.second.first;
		auto& matVec = instancingData.second.second;

		// register vertex buffer
		m_instancingBuffer[i].Create(matVec);
		bacth.vertexBuffers.emplace_back(&m_instancingBuffer[i]);
		m_bacthList.emplace_back(bacth);
		++i;
	}
}

void ForwardRenderer::PrePass() noexcept
{
	CreateRenderList();

	m_lightMap->Update(m_mainCamera);
	m_transformCBuffer->Update(m_mainCamera);

#if IS_EDITOR
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
#else
	D3D12GraphicsDevice::Get().SetRenderTarget();
#endif // IS_EDITOR

	ZPrePass();
}

void ForwardRenderer::ZPrePass()
{
	for (auto renderObject : m_renderObjects)
	{
		renderObject->PreRender();
	}

	// Draw
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

void ForwardRenderer::LightingPass() noexcept
{
	// Draw
	for (auto& bacth : m_bacthList)
	{
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
}

void ForwardRenderer::PostPass() noexcept
{

}