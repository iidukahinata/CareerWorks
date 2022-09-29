/**
* @file    D3D12DescriptorHeap.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"

class D3D12DescriptorHeap : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;
	void Set() noexcept;

	void Release() noexcept;

	/** アクセス */
	UINT GetIncrementSize() const noexcept { return m_incrementSize; }
	ID3D12DescriptorHeap* GetHeap() const noexcept { return m_descriptHeap.Get(); }
	D3D12_CPU_DESCRIPTOR_HANDLE	GetCPUHandle() const noexcept { return m_cpuHandle; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const noexcept { return m_gpuHandle; }

private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptHeap;

	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuHandle;

	UINT m_incrementSize = 0;
	UINT m_numDescriptors = 0;
};