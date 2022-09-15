/**
* @file    D3D12Buffer.cpp
* @brief
*
* @date	   2022/08/30 2022”N“x‰”Å
*/


#include "D3D12Buffer.h"

bool D3D12Buffer::Create(
	const D3D12_HEAP_PROPERTIES* heapProperties,
	D3D12_HEAP_FLAGS heapFlags,
	const D3D12_RESOURCE_DESC* desc,
	D3D12_RESOURCE_STATES initialResourceState /* = D3D12_RESOURCE_STATE_GENERIC_READ */,
	const D3D12_CLEAR_VALUE* optimizedClearValue /* = nullptr */
) noexcept
{
	auto hr = GetDevice()->CreateCommittedResource(
		heapProperties,
		heapFlags,
		desc,
		initialResourceState,
		optimizedClearValue,
		IID_PPV_ARGS(m_buffer.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		LOG_ERROR("ID3DResource ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_buffer->Map(0, nullptr, &m_cpuData);
	m_bytesSize = desc->Width;

	return true;
}

void D3D12Buffer::Update(UINT destSize, void* srcPtr, UINT srcSize) noexcept
{
	memcpy_s(m_cpuData, destSize, srcPtr, srcSize);
}