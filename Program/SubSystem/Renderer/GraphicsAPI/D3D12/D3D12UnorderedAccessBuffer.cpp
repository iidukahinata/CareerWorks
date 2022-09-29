/**
* @file    D3D12ConstantBuffer.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "D3D12UnorderedAccessBuffer.h"
#include "D3D12GraphicsDevice.h"

bool D3D12UnorderedAccessBuffer::Create(
	UINT bufferSize,
	UINT elementCount,
	UINT byteStride,
	UINT offsetInBytes
) noexcept
{
	const auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	const auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	auto ret = D3D12Buffer::Create(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COPY_DEST);
	if (!ret) {
		return false;
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = elementCount;
	uavDesc.Buffer.StructureByteStride = byteStride;
	uavDesc.Buffer.CounterOffsetInBytes = offsetInBytes;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

	m_view.Create(m_buffer.Get(), uavDesc);

	return true;
}

void D3D12UnorderedAccessBuffer::VSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetUnorderedAccess(VertexShader, slot, &m_view);
}

void D3D12UnorderedAccessBuffer::PSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetUnorderedAccess(PixelShader, slot, &m_view);
}

void D3D12UnorderedAccessBuffer::GSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetUnorderedAccess(GeometryShader, slot, &m_view);
}

void D3D12UnorderedAccessBuffer::CSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetUnorderedAccess(ComputeShader, slot, &m_view);
}