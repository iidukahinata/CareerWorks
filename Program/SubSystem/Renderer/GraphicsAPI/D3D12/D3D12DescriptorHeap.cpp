/**
* @file    D3D12DescriptorHeap.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "D3D12DescriptorHeap.h"
#include "D3D12CommandContext.h"

bool D3D12DescriptorHeap::Create(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptHeapDesc = {};
	descriptHeapDesc.NumDescriptors = numDescriptors;
	descriptHeapDesc.Type = type;
	descriptHeapDesc.NodeMask = 0;
	descriptHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto hr = GetDevice()->CreateDescriptorHeap(&descriptHeapDesc, IID_PPV_ARGS(m_descriptHeap.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		LOG_ERROR("DescriptorHeap ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_numDescriptors = numDescriptors;
	m_incrementSize = GetDevice()->GetDescriptorHandleIncrementSize(type);

	m_cpuHandle = m_descriptHeap->GetCPUDescriptorHandleForHeapStart();
	m_gpuHandle = m_descriptHeap->GetGPUDescriptorHandleForHeapStart();

	return true;
}

void D3D12DescriptorHeap::Set() noexcept
{
	GetCommandContext()->SetDescriptorHeap(this);
}

void D3D12DescriptorHeap::Release() noexcept
{
	m_descriptHeap.Reset();
}