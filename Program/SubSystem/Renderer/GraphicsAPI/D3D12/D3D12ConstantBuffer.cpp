/**
* @file    D3D12ConstantBuffer.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "D3D12ConstantBuffer.h"
#include "D3D12GrahicsDevice.h"

bool D3D12ConstantBuffer::Create(UINT bufferSize) noexcept
{
	bufferSize = AdjustToMultiples(bufferSize, 256);
	const auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	auto ret = D3D12Buffer::Create(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc);
	if (!ret) {
		return false;
	}

	m_view.Create(m_buffer->GetGPUVirtualAddress(), bufferSize);

	ZeroMemory(m_cpuData, m_view.GetDesc().SizeInBytes);

	return true;
}

void D3D12ConstantBuffer::VSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetConstantBuffer(VertexShader, slot, &m_view);
}

void D3D12ConstantBuffer::PSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetConstantBuffer(PixelShader, slot, &m_view);
}

void D3D12ConstantBuffer::GSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetConstantBuffer(GeometryShader, slot, &m_view);
}