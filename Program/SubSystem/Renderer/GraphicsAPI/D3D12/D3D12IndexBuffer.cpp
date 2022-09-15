/**
* @file    D3D12IndexBuffer.cpp
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/


#include "D3D12IndexBuffer.h"
#include "D3D12CommandContext.h"

bool D3D12IndexBuffer::Create(const Vector<UINT>& indices) noexcept
{
	return Create(indices, DXGI_FORMAT_R32_UINT);
}

bool D3D12IndexBuffer::Create(const Vector<UINT16>& indices) noexcept
{
	return Create(indices, DXGI_FORMAT_R16_UINT);
}

void D3D12IndexBuffer::IASet() noexcept
{
	GetCommandContext()->SetIndexBuffer(&m_bufferView);
}