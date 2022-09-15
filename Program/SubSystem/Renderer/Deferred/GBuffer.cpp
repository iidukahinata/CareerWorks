/**
* @file	   GBuffer.cpp
* @brief
*
* @date	   2022/09/02 2022îNìxèâî≈
*/


#include "GBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12GrahicsDevice.h"

void GBuffer::Initialize(uint32_t wight, uint32_t height) noexcept
{
	m_rendererTexture[GBufferType::Color].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTexture[GBufferType::Normal].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_rendererTexture[GBufferType::Depth].Create(wight, height, DXGI_FORMAT_R32G32_FLOAT);
	m_rendererTexture[GBufferType::Parameter].Create(wight, height, DXGI_FORMAT_R32G32B32A32_FLOAT);
}

void GBuffer::SetRenderTargets() noexcept
{
	D3D12RenderTargetView* renderTargets[] = {
		m_rendererTexture[GBufferType::Color].GetRenderTargetView(),
		m_rendererTexture[GBufferType::Normal].GetRenderTargetView(),
		m_rendererTexture[GBufferType::Depth].GetRenderTargetView(),
		m_rendererTexture[GBufferType::Parameter].GetRenderTargetView(),
	};

	auto& context = D3D12GrahicsDevice::Get().GetCommandContext();
	context.SetRenderTargets(GBufferType::Max, renderTargets, m_rendererTexture[GBufferType::Color].GetDepthStencilView());
}

void GBuffer::Clear() noexcept
{
	for (auto& renderTexture : m_rendererTexture)
	{
		renderTexture.Clear(Math::Vector4(0, 0, 0, 1));
	}
}

D3D12RenderTexture& GBuffer::GetRenderTexture(GBufferType type) noexcept
{
	return m_rendererTexture[type];
}