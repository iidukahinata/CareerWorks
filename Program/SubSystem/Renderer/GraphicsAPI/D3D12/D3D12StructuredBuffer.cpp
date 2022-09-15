/**
* @file    D3D12StructuredBuffer.h
* @brief
*
* @date	   2022/08/30 2022”N“x‰”Å
*/


#include "D3D12StructuredBuffer.h"
#include "D3D12CommandContext.h"

bool D3D12StructuredBuffer::Create(
	UINT bufferSize,
	UINT elementsCount,
	UINT byteStride,
    D3D12_HEAP_TYPE heapType /* = D3D12_HEAP_TYPE_UPLOAD */,
    D3D12_RESOURCE_STATES initialResourceState /* = D3D12_RESOURCE_STATE_GENERIC_READ */
)
{
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(heapType);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	auto ret = D3D12Buffer::Create(&texHeapProp, D3D12_HEAP_FLAG_NONE, &desc, initialResourceState);
	if (!ret) {
		LOG_ERROR("ID3DResource ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Buffer.NumElements = elementsCount;
	srvDesc.Buffer.StructureByteStride = byteStride;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	m_view.Create(m_buffer.Get(), srvDesc);

	return true;
}

void D3D12StructuredBuffer::VSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(VertexShader, slot, &m_view);
}

void D3D12StructuredBuffer::PSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(PixelShader, slot, &m_view);
}

void D3D12StructuredBuffer::GSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(GeometryShader, slot, &m_view);
}

void D3D12StructuredBuffer::CSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(ComputeShader, slot, &m_view);
}