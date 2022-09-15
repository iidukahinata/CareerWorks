/**
* @file    D3D12VertexBuffer.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "D3D12VertexBuffer.h"
#include "D3D12CommandContext.h"

bool D3D12VertexBuffer::Create(UINT size, UINT stride) noexcept
{
	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

	auto ret = D3D12Buffer::Create(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc);
	if (!ret) {
		return false;
	}

	// ビュー生成
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.SizeInBytes = size;
	m_bufferView.StrideInBytes = stride;

	return true;
}

void D3D12VertexBuffer::IASet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetVertexBuffer(&m_bufferView, slot);
}