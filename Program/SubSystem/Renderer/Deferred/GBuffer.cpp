/**
* @file	   GBuffer.cpp
* @brief
*
* @date	   2022/09/17 2022îNìxèâî≈
*/


#include "GBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12GraphicsDevice.h"

void GBuffer::Initialize(uint32_t wight, uint32_t height) noexcept
{
	m_rendererTextures[GBufferType::Albedo  ].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTextures[GBufferType::Specular].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTextures[GBufferType::Normal  ].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTextures[GBufferType::Depth   ].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTextures[GBufferType::Position].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void GBuffer::SetRenderTargets() noexcept
{
	D3D12RenderTargetView* renderTargets[] = {
		m_rendererTextures[GBufferType::Albedo  ].GetRenderTargetView(),
		m_rendererTextures[GBufferType::Specular].GetRenderTargetView(),
		m_rendererTextures[GBufferType::Normal  ].GetRenderTargetView(),
		m_rendererTextures[GBufferType::Depth   ].GetRenderTargetView(),
		m_rendererTextures[GBufferType::Position].GetRenderTargetView(),
	};

	// Barrier
	for (auto& renderTexture : m_rendererTextures)
	{
		renderTexture.WaitUntilToAvailable();
	}

	auto& context = D3D12GraphicsDevice::Get().GetCommandContext();
	context.SetRenderTargets(GBufferType::Max, renderTargets, m_rendererTextures[GBufferType::Albedo].GetDepthStencilView());
}

void GBuffer::Clear() noexcept
{
	for (auto& renderTexture : m_rendererTextures)
	{
		renderTexture.Clear(Math::Vector4(0, 0, 0, 1));
	}
}

D3D12RenderTexture& GBuffer::GetRenderTexture(GBufferType type) noexcept
{
	return m_rendererTextures[type];
}