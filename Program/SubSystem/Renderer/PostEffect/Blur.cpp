/**
* @file    Blur.cpp
* @brief
*
* @date	   2022/11/10 2022年度初版
*/


#include "Blur.h"
#include "../IRenderer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "SubSystem/Window/Window.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void Blur::Serialized(FileStream* file) const
{
	file->Write(m_variance);
}

void Blur::Deserialized(FileStream* file)
{
	file->Read(&m_variance);

	CalculateWeightTablesFromGaussian();
}

void Blur::Initialize()
{
	auto width = Window::Get().GetWindowWidth();
	auto height = Window::Get().GetWindowHeight();

	CreateRenderTextures(width, height);
	CreateRenderingObjects(width, height);
	CreatePipelineStates();
}

void Blur::Initialize(PostProcessEffect* postProcessEffect, bool useluma) noexcept
{
	m_postProcessEffect = postProcessEffect;

	auto width = Window::Get().GetWindowWidth();
	auto height = Window::Get().GetWindowHeight();

	if (IsInRenderingThread())
	{
		CreateRenderTextures(width, height);
		CreateRenderingObjects(width, height);
		CreatePipelineStates(useluma);
	}
	else
	{
		RegisterRenderCommand([this, width, height, useluma] {

			CreateRenderTextures(width, height);
			CreateRenderingObjects(width, height);
			CreatePipelineStates(useluma);

		});
	}
}

void Blur::Render()
{
	m_blurConstantBuffer.VSSet(1);
	m_blurConstantBuffer.PSSet(1);

	// x blur pass
	{
		m_xBlurRenderTexture.SetRenderTarget();
		m_xBlurRenderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		
		m_xBlurMatrixConstantBuffer.VSSet(0);

		m_xBlurPipeline.Set();
		m_sampler.PSSet();
		m_xBlurIndexBuffer.IASet();
		m_xBlurVertexBuffer.IASet();
		
		// Draw
		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	// y blur pass
	{
		m_xBlurRenderTexture.WaitUntilFinishDrawing();

		m_yBlurRenderTexture.SetRenderTarget();
		m_yBlurRenderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		
		m_yBlurMatrixConstantBuffer.VSSet(0);
		m_xBlurRenderTexture.PSSet(7);
		
		m_yBlurPipeline.Set();
		m_sampler.PSSet();
		m_yBlurIndexBuffer.IASet();
		m_yBlurVertexBuffer.IASet();
		
		// Draw
		D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
	}

	m_yBlurRenderTexture.WaitUntilFinishDrawing();
	m_yBlurRenderTexture.PSSet(7);
}

void Blur::SetVariance(float variance) noexcept
{
	m_variance = variance;

	// 新しく重みテーブル作成
	if (IsInRenderingThread())
	{
		CalculateWeightTablesFromGaussian();
	}
	else
	{
		RegisterRenderCommand([this] {
			CalculateWeightTablesFromGaussian();
		});
	}
}

float Blur::GetVariance() const noexcept
{
	return m_variance;
}

D3D12RenderTexture& Blur::GetTexture()
{
	return m_yBlurRenderTexture;
}

bool Blur::CreateRenderTextures(int width, int height) noexcept
{
	m_xBlurRenderTexture.Create(width / 2, height	 , DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_yBlurRenderTexture.Create(width / 2, height / 2, DXGI_FORMAT_R32G32B32A32_FLOAT);
	return true;
}

bool Blur::CreateRenderingObjects(int width, int height) noexcept
{
	// Create ConstantBuffer
	{
		auto transformCBuffer = GetRenderer()->GetTransformCBuffer();
		if (m_xBlurMatrixConstantBuffer.Create(sizeof(ConstantBufferMatrix)))
		{
			Math::Matrix world(Math::Vector3::Zero, Math::Vector3::Zero, Math::Vector3(2, 1, 1));
			transformCBuffer->Bind(m_xBlurMatrixConstantBuffer.GetCPUData(), world.ToMatrixXM());
		}

		if (m_yBlurMatrixConstantBuffer.Create(sizeof(ConstantBufferMatrix)))
		{
			Math::Matrix world(Math::Vector3::Zero, Math::Vector3::Zero, Math::Vector3(2, 2, 1));
			transformCBuffer->Bind(m_yBlurMatrixConstantBuffer.GetCPUData(), world.ToMatrixXM());
		}

		if (!m_blurConstantBuffer.Create(sizeof(ConstantBufferBlur)))
		{
			return false;
		}

		auto buffer = static_cast<ConstantBufferBlur*>(m_blurConstantBuffer.GetCPUData());

		SetVariance(5.0f);
		buffer->screenSize.x = Window::Get().GetWindowWidth();
		buffer->screenSize.y = Window::Get().GetWindowHeight();
	}

	// Create Sampler
	m_sampler.Create(D3D12_FILTER_MIN_MAG_MIP_LINEAR);

	// Create Mesh Buffer
	{
		auto&& xBlurTriangleList = Quad::Create<Vertex2D>(width / 2, height);
		auto&& yBlurTriangleList = Quad::Create<Vertex2D>(width / 2, height / 2);

		if (!m_xBlurVertexBuffer.Create(xBlurTriangleList.m_vertices))
		{
			return false;
		}
		if (!m_xBlurIndexBuffer.Create(xBlurTriangleList.m_indices))
		{
			return false;
		}

		if (!m_yBlurVertexBuffer.Create(yBlurTriangleList.m_vertices))
		{
			return false;
		}
		if (!m_yBlurIndexBuffer.Create(yBlurTriangleList.m_indices))
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

bool Blur::CreatePipelineStates(bool useluna /* = false */) noexcept
{
	auto blurShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

	GraphicsPipelineStateDesc desc = {};
	desc.BlendMode = BLEND_MODE_ALPHA;
	desc.RasterizerState = NO_CULL;
	desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;
	desc.RTVForamt[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Create XBlur Pipeline
	{
		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("XBlur", "1");
		if (useluna) defines.emplace_back("UseLuma", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> xBlurShaders;
		xBlurShaders[VertexShader].Compile(blurShaderPath, VertexShader, defines.data());
		xBlurShaders[PixelShader].Compile(blurShaderPath, PixelShader, defines.data());

		desc.VS = &xBlurShaders[VertexShader];
		desc.PS = &xBlurShaders[PixelShader];

		if (!m_xBlurPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	// Create YBlur Pipeline
	{
		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("YBlur", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> yBlurShaders;
		yBlurShaders[VertexShader].Compile(blurShaderPath, VertexShader, defines.data());
		yBlurShaders[PixelShader].Compile(blurShaderPath, PixelShader, defines.data());

		desc.VS = &yBlurShaders[VertexShader];
		desc.PS = &yBlurShaders[PixelShader];

		if (!m_yBlurPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	return true;
}

void Blur::CalculateWeightTablesFromGaussian() noexcept
{
	auto buffer = static_cast<ConstantBufferBlur*>(m_blurConstantBuffer.GetCPUData());

	float weightSum = 0.f;
	for (int i = 0; i < 8; ++i)
	{
		buffer->weights[i] = expf(-0.5f * static_cast<float>(i * i) / m_variance);
		weightSum += 2.f * buffer->weights[i];
	}

	// normalize
	for (int i = 0; i < 8; ++i)
	{
		buffer->weights[i] /= weightSum;
	}
}