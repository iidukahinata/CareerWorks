/**
* @file    D3D12DescriptorAllocator.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "D3D12DescriptorAllocator.h"

void D3D12Descriptor::AddRef() noexcept
{
	++m_refCount;
}

void D3D12Descriptor::Release() noexcept
{
	--m_refCount;
	if (m_refCount == 0)
	{
		m_allocator->Deallocate(this);
	}
}

D3D12Descriptor::D3D12Descriptor(
	D3D12DescriptorAllocator* allocator,
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle
) noexcept
{
	m_allocator = allocator;
	m_cpuHandle = cpuHandle;
	m_gpuHandle = gpuHandle;
}

//
// D3D12DescriptorAllocator
//

D3D12DescriptorAllocator::~D3D12DescriptorAllocator()
{
	Release();
}

bool D3D12DescriptorAllocator::Initialize(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept
{
	Release();

	D3D12_DESCRIPTOR_HEAP_DESC descriptHeapDesc = {};
	descriptHeapDesc.NumDescriptors = numDescriptors;
	descriptHeapDesc.Type = type;
	descriptHeapDesc.NodeMask = 1;
	descriptHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	auto hr = GetDevice()->CreateDescriptorHeap(&descriptHeapDesc, IID_PPV_ARGS(m_descriptHeap.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	m_heapType = type;
	m_numDescriptors = numDescriptors;
	m_incrementSize = GetDevice()->GetDescriptorHandleIncrementSize(type);

	m_cpuBackHandle = m_descriptHeap->GetCPUDescriptorHandleForHeapStart();
	m_gpuBackHandle = m_descriptHeap->GetGPUDescriptorHandleForHeapStart();

	return true;
}

void D3D12DescriptorAllocator::Release() noexcept
{
	m_currentPosition = 0;
	m_descripts.clear();
	m_descripts.shrink_to_fit();
	m_freeList.clear();
	m_freeList.shrink_to_fit();
}

D3D12Descriptor* D3D12DescriptorAllocator::Allocate() noexcept
{
	D3D12Descriptor* descript;

	if (m_freeList.empty())
	{
		ASSERT(m_currentPosition <= m_numDescriptors);

		descript = new D3D12Descriptor(this, m_cpuBackHandle, m_gpuBackHandle);
		m_descripts.emplace_back(descript);

		// 次のメモリ位置に移動させておく
		m_cpuBackHandle.ptr += m_incrementSize;
		m_gpuBackHandle.ptr += m_incrementSize;
		++m_currentPosition;
	}
	else
	{
		descript = m_freeList.back();
		m_freeList.pop_back();
	}

	descript->AddRef();
	return descript;
}

void D3D12DescriptorAllocator::Deallocate(D3D12Descriptor* ptr) noexcept
{
	m_freeList.emplace_back(ptr);
}

//
// D3D12GlobalDescriptorHeapBlock
//

D3D12GlobalDescriptorHeapBlock::D3D12GlobalDescriptorHeapBlock(
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
	UINT incrementSize,
	UINT blockSize
) : m_cpuBaseHandle(cpuHandle),
	m_gpuBaseHandle(gpuHandle),
	m_incrementSize(incrementSize),
	m_blockSize(blockSize),
	m_currentPosition(0)
{

}

bool D3D12GlobalDescriptorHeapBlock::Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept
{
	const auto nextPos = m_currentPosition + size;

	// 取得サイズが無い状態を通知
	if (nextPos >= m_blockSize)
	{
		return false;
	}
	else
	{
		*cpuHandle = m_cpuBaseHandle;
		*gpuHandle = m_gpuBaseHandle;
		cpuHandle->ptr += static_cast<SIZE_T>(m_incrementSize * m_currentPosition);
		gpuHandle->ptr += static_cast<SIZE_T>(m_incrementSize * m_currentPosition);

		m_currentPosition += size;

		return true;
	}
}

void D3D12GlobalDescriptorHeapBlock::Reset() noexcept
{
	m_currentPosition = 0;
}

//
// D3D12GlobalDescriptorHeap
//

bool D3D12GlobalDescriptorHeap::Initialilze(UINT totalSize, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptHeapDesc = {};
	descriptHeapDesc.NumDescriptors = totalSize;
	descriptHeapDesc.Type = type;
	descriptHeapDesc.NodeMask = 1;
	descriptHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto hr = GetDevice()->CreateDescriptorHeap(&descriptHeapDesc, IID_PPV_ARGS(m_descriptHeap.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	m_heapType = type;
	m_numDescriptors = totalSize;
	m_incrementSize = GetDevice()->GetDescriptorHandleIncrementSize(type);

	m_cpuCurrentHandle = m_descriptHeap->GetCPUDescriptorHandleForHeapStart();
	m_gpuCurrentHandle = m_descriptHeap->GetGPUDescriptorHandleForHeapStart();

	return true;
}

D3D12GlobalDescriptorHeapBlock* D3D12GlobalDescriptorHeap::Allocate() noexcept
{
	D3D12GlobalDescriptorHeapBlock* block = nullptr;

	if (m_freeBlocks.empty())
	{
		ASSERT(m_currentPosition <= m_numDescriptors);

		block = new D3D12GlobalDescriptorHeapBlock(m_cpuCurrentHandle, m_gpuCurrentHandle, m_incrementSize, m_splitSize);
		m_blocks.emplace_back(block);

		// 次のメモリ位置に移動させておく
		m_cpuCurrentHandle.ptr += m_splitSize;
		m_gpuCurrentHandle.ptr += m_splitSize;
		++m_currentPosition;
	}
	else
	{
		block = m_freeBlocks.back();
		m_freeBlocks.pop_back();
	}

	return block;
}

void D3D12GlobalDescriptorHeap::Deallocate(D3D12GlobalDescriptorHeapBlock* ptr) noexcept
{
	ptr->Reset();
	m_freeBlocks.push_back(ptr);
}

//
// D3D12GlobalAllocatedDescriptorHeap
//

void D3D12GlobalAllocatedDescriptorHeap::Initialilze(D3D12GlobalDescriptorHeap* globalDescriptHeap) noexcept
{
	m_globalDescriptHeap = globalDescriptHeap;
	m_blockIndex = 0;

	m_blocks.emplace_back(m_globalDescriptHeap->Allocate());
}

void D3D12GlobalAllocatedDescriptorHeap::Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept
{
	if (m_blocks[m_blockIndex]->Allocate(size, cpuHandle, gpuHandle))
	{
		return;
	}

	// 指定サイズ領域がない場合新しいBlockを取得する
	++m_blockIndex;
	m_blocks.emplace_back(m_globalDescriptHeap->Allocate());

	Allocate(size, cpuHandle, gpuHandle);
}

void D3D12GlobalAllocatedDescriptorHeap::Reset() noexcept
{
	for (auto block : m_blocks)
	{
		m_globalDescriptHeap->Deallocate(block);
	}

	m_blockIndex = 0;
	m_blocks.clear();
	
	m_blocks.emplace_back(m_globalDescriptHeap->Allocate());
}

//
// D3D12GlobalSamplerDescriptorHeap
//

bool D3D12GlobalSamplerDescriptorHeap::Initialilze() noexcept
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptHeapDesc = {};
	descriptHeapDesc.NumDescriptors = m_numDescriptors;
	descriptHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	descriptHeapDesc.NodeMask = 1;
	descriptHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	auto hr = GetDevice()->CreateDescriptorHeap(&descriptHeapDesc, IID_PPV_ARGS(m_descriptHeap.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		return false;
	}

	m_incrementSize = GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	
	m_cpuCurrentHandle = m_descriptHeap->GetCPUDescriptorHandleForHeapStart();
	m_gpuCurrentHandle = m_descriptHeap->GetGPUDescriptorHandleForHeapStart();

	return true;
}

void D3D12GlobalSamplerDescriptorHeap::Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept
{
	ASSERT((m_currentPosition + size) <= m_numDescriptors);

	*cpuHandle = m_cpuCurrentHandle;
	*gpuHandle = m_gpuCurrentHandle;

	// 次のメモリ位置に移動させておく
	m_cpuCurrentHandle.ptr += (m_incrementSize * size);
	m_gpuCurrentHandle.ptr += (m_incrementSize * size);

	m_currentPosition += size;
}