/**
* @file    Bloom.cpp
* @brief
*
* @date	   2022/11/10 2022年度初版
*/


#include "Bloom.h"
#include "../IRenderer.h"
#include "../Geometry/Quad.h"
#include "../Geometry/Vertex.h"
#include "SubSystem/Window/Window.h"
#include "../GraphicsAPI/D3D12/D3D12Shader.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void Bloom::Serialized(FileStream* file) const
{
	file->Write(m_intensity);
	file->Write(m_threshold);
	file->Write(m_diffusion);
	file->Write(m_clamp);
	file->Write(m_color);
}

void Bloom::Deserialized(FileStream* file)
{
	file->Read(&m_intensity);
	file->Read(&m_threshold);
	file->Read(&m_diffusion);
	file->Read(&m_clamp);
	file->Read(&m_color);

	for (auto& blur : m_blurs)
	{
		blur.SetVariance(m_diffusion);
	}
}

void Bloom::Initialize()
{
	m_blurs[0].Initialize(m_postProcessEffect);
	m_blurs[1].Initialize(m_postProcessEffect);
	m_blurs[2].Initialize(m_postProcessEffect);
	m_blurs[3].Initialize(m_postProcessEffect);

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

void Bloom::Render()
{
	LuminousPass();

	DualBlurPass();

	BloomPass();

	m_renderTexture.PSSet(7);
}

D3D12RenderTexture& Bloom::GetTexture()
{
	return m_renderTexture;
}

void Bloom::SetIntensity(float intensity) noexcept
{
	m_intensity = intensity;

	UpdateConstantBufferLuma();
}

float Bloom::GetIntensity() const noexcept
{
	return m_intensity;
}

void Bloom::SetThreshold(float threshold) noexcept
{
	m_threshold = threshold;

	UpdateConstantBufferLuma();
}

float Bloom::GetThreshold() const noexcept
{
	return m_threshold;
}

void Bloom::SetClamp(float clamp) noexcept
{
	m_clamp = clamp;

	UpdateConstantBufferLuma();
}

float Bloom::GetClamp() const noexcept
{
	return m_clamp;
}

void Bloom::SetDiffusion(float variance) noexcept
{
	m_diffusion = variance;

	for (auto& blur : m_blurs)
	{
		blur.SetVariance(variance);
	}
}

float Bloom::GetDiffusion() const noexcept
{
	return m_diffusion;
}

void Bloom::SetColor(const Math::Vector4& color) noexcept
{
	m_color = color;

	UpdateConstantBufferBloom();
}

const Math::Vector4& Bloom::GetColor() const noexcept
{
	return m_color;
}

void Bloom::LuminousPass() noexcept
{
	// RenderTarget Set
	m_luminousRenderTexture.SetRenderTarget();
	m_luminousRenderTexture.Clear(Math::Vector4(0.f, 0.f, 0.f, 1.f));

	// Pipeline Set
	m_luminousPipeline.Set();

	// CRV Set
	m_lumaConstantBuffer.PSSet(1);

	// Mesh Set
	m_vertexBuffer.IASet();
	m_indexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);

	m_luminousRenderTexture.WaitUntilFinishDrawing();
}

void Bloom::DualBlurPass() noexcept
{
	m_luminousRenderTexture.PSSet(7);

	// 輝度テクスチャにブラーをかける
	for (auto& blur : m_blurs)
	{
		blur.Render();
	}
}

void Bloom::BloomPass() noexcept
{
	m_renderTexture.SetRenderTarget();
	m_renderTexture.Clear(Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	m_bloomPipeline.Set();

	m_matrixConstantBuffer.VSSet(0);
	m_bloomConstantBuffer.PSSet(1);

	constexpr int offset = 7;
	for (int i = 0; i < m_blurs.size(); ++i)
	{
		m_blurs[i].GetTexture().PSSet(offset + i);
	}

	m_indexBuffer.IASet();
	m_vertexBuffer.IASet();

	// Draw
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(6, 1, 0, 0, 0);
}

bool Bloom::CreateRenderTextures(int width, int height) noexcept
{
	if (!m_luminousRenderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT))
	{
		return false;
	}

	if (!m_renderTexture.Create(width, height, DXGI_FORMAT_R32G32B32A32_FLOAT))
	{
		return false;
	}

	return true;
}

bool Bloom::CreateRenderingObjects(int width, int height) noexcept
{
	// Create ConstantBuffer
	{
		if (m_lumaConstantBuffer.Create(sizeof(ConstantBufferLuma)))
		{
			UpdateConstantBufferLuma();
		}

		if (m_bloomConstantBuffer.Create(sizeof(ConstantBufferBloom)))
		{
			UpdateConstantBufferBloom();
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

	return true;
}

bool Bloom::CreatePipelineStates() noexcept
{
	GraphicsPipelineStateDesc desc = {};
	desc.BlendMode = BLEND_MODE_ALPHA;
	desc.RasterizerState = NO_CULL;
	desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;
	desc.RTVForamt[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	// Create Bloom Pipeline
	{
		auto bloomShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("Bloom", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> bloomShaders;
		bloomShaders[VertexShader].Compile(bloomShaderPath, VertexShader, defines.data());
		bloomShaders[PixelShader].Compile(bloomShaderPath, PixelShader, defines.data());

		desc.VS = &bloomShaders[VertexShader];
		desc.PS = &bloomShaders[PixelShader];

		if (!m_bloomPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	// Create Luminous Pipeline
	{
		auto luminousShaderPath = FileSystem::FindFilePath(SHADER_DIRECTORY, "PostProcess.hlsl");

		Vector<D3D_SHADER_MACRO> defines;
		defines.emplace_back("Luma", "1");
		defines.emplace_back("UseLumaCRV", "1");
		defines.emplace_back(D3D_SHADER_MACRO(NULL, NULL));

		Array<D3D12Shader, 2> luminousShaders;
		luminousShaders[VertexShader].Compile(luminousShaderPath, VertexShader, defines.data());
		luminousShaders[PixelShader].Compile(luminousShaderPath, PixelShader, defines.data());

		desc.VS = &luminousShaders[VertexShader];
		desc.PS = &luminousShaders[PixelShader];

		if (!m_luminousPipeline.Create(desc, &m_rootSignature))
		{
			return false;
		}
	}

	return true;
}

void Bloom::UpdateConstantBufferLuma() noexcept
{
	if (IsInRenderingThread())
	{
		auto buffer = static_cast<ConstantBufferLuma*>(m_lumaConstantBuffer.GetCPUData());
		buffer->intensity = m_intensity;
		buffer->threshold = m_threshold;
		buffer->clamp = m_clamp;
	}
	else
	{
		RegisterRenderCommand([this] {

			auto buffer = static_cast<ConstantBufferLuma*>(m_lumaConstantBuffer.GetCPUData());
			buffer->intensity = m_intensity;
			buffer->threshold = m_threshold;
			buffer->clamp = m_clamp;

		});
	}
}

void Bloom::UpdateConstantBufferBloom() noexcept
{
	if (IsInRenderingThread())
	{
		auto buffer = static_cast<ConstantBufferBloom*>(m_bloomConstantBuffer.GetCPUData());
		buffer->color = m_color;
	}
	else
	{
		RegisterRenderCommand([this] {

			auto buffer = static_cast<ConstantBufferBloom*>(m_bloomConstantBuffer.GetCPUData());
			buffer->color = m_color;

		});
	}
}