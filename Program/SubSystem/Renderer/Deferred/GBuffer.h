/**
* @file	   GBuffer.h
* @brief
*
* @date	   2022/09/02 2022îNìxèâî≈
*/
#pragma once


#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"

enum GBufferType
{
	Color,
	Normal,
	Depth,
	Parameter,
	Max,
};

class GBuffer
{
	COMPLETED_DEVELOPMENT()
public:

	void Initialize(uint32_t wight, uint32_t height) noexcept;
	void SetRenderTargets() noexcept;
	void Clear() noexcept;

	D3D12RenderTexture& GetRenderTexture(GBufferType type) noexcept;

private:

	Array<D3D12RenderTexture, GBufferType::Max> m_rendererTexture;
};