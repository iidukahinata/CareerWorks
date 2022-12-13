/**
* @file    FXAA.cpp
* @brief
*
* @date	   2022/12/12 2022îNìxèâî≈
*/


#include "FXAA.h"
#include "../IRenderer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "SubSystem/Window/Window.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void FXAA::Serialized(FileStream* file) const
{

}

void FXAA::Deserialized(FileStream* file)
{

}

void FXAA::Initialize()
{
	const auto width  = Window::Get().GetWindowWidth();
	const auto height = Window::Get().GetWindowHeight();

	if (IsInRenderingThread())
	{
		CreateRenderTextures(width, height);
		CreateRenderingObjects(width, height);
		CreatePipelineStates();
	}
	else
	{
		RegisterRenderCommand([this, width, height] {

			CreateRenderTextures(width, height);
			CreateRenderingObjects(width, height);
			CreatePipelineStates();

		});
	}
}

void FXAA::Render()
{
	// RenderTarget Set
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.f, 0.f, 0.f, 1.f));

	// Pipeline Set
	m_fxaaPipeline.Set();

	// CRV Set
	m_fxaaConstantBuffer.PSSet(1);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);

	m_renderTexture.WaitUntilFinishDrawing();
	m_renderTexture.PSSet(7);
}

D3D12RenderTexture& FXAA::GetTexture()
{
	return m_renderTexture;
}

bool FXAA::CreateRenderTextures(int width, int height) noexcept
{
	return m_renderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

bool FXAA::CreateRenderingObjects(int width, int height) noexcept
{
	// Create ConstantBuffer
	{
		if (m_fxaaConstantBuffer.Create(sizeof(ConstantBufferFXAA)))
		{
			auto buffer = static_cast<ConstantBufferFXAA*>(m_fxaaConstantBuffer.GetCPUData());
			buffer->rcpFrame.x = 1.f / width;
			buffer->rcpFrame.y = 1.f / height;
		}

		if (m_matrixConstantBuffer.Create(sizeof(ConstantBufferMatrix)))
		{
			GetRenderer()->GetTransformCBuffer()->Bind(m_matrixConstantBuffer.GetCPUData(), Math::Matrix::Identity.ToMatrixXM());
		}
	}

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

	return false;
}

bool FXAA::CreatePipelineStates() noexcept
{
	GraphicsPipelineStateDesc desc = {};
	desc.BlendMode		  = BLEND_MODE_ALPHA;
	desc.RasterizerState  = NO_CULL;
	desc.PrimitiveType	  = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;
	desc.RTVForamt[0]	  = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Create FXAA Pipeline
	{
		auto bloomShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("FXAA", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> fxaaShaders;
		fxaaShaders[VertexShader].Compile(bloomShaderPath, VertexShader, defines.data());
		fxaaShaders[PixelShader].Compile(bloomShaderPath, PixelShader, defines.data());

		desc.VS = &fxaaShaders[VertexShader];
		desc.PS = &fxaaShaders[PixelShader];

		if (!m_fxaaPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	return true;
}