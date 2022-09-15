/**
* @file    D3D12DescriptorAllocator.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"

class D3D12DescriptorAllocator;

class D3D12Descriptor
{
	COMPLETED_DEVELOPMENT()
public:

	// * Allocatorテーブルの各メモリ位置
	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuHandle;

public:

	D3D12Descriptor(
		D3D12DescriptorAllocator* allocator,
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle) noexcept;

	void AddRef() noexcept;
	void Release() noexcept;

private:

	// * 自身の解放処理で使用
	D3D12DescriptorAllocator* m_allocator;

	UINT m_refCount = 0;
};

//
// D3D12DescriptorAllocator
//

class D3D12DescriptorAllocator : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	~D3D12DescriptorAllocator();
	bool Initialize(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;
	void Release() noexcept;

	D3D12Descriptor* Allocate() noexcept;
	void Deallocate(D3D12Descriptor* ptr) noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptHeap;

	// * 初期化設定
	UINT m_incrementSize;
	UINT m_numDescriptors;
	D3D12_DESCRIPTOR_HEAP_TYPE m_heapType;

	// * 初期化時用意したDescriptサイズ範囲外アクセスを防ぐため監視
	UINT m_currentPosition;

	// * 次作成されるDescriptorが保持するメモリ位置
	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuBackHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuBackHandle;

	// * 解放されたDescriptorを再利用するために使用
	Vector<D3D12Descriptor*> m_freeList;

	// * 生成された全てのDescriptorを管理
	Vector<UniquePtr<D3D12Descriptor>> m_descripts;
};

//
// D3D12GlobalDescriptorHeapBlock
//

class D3D12GlobalDescriptorHeapBlock
{
	COMPLETED_DEVELOPMENT()
public:

	D3D12GlobalDescriptorHeapBlock(
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle,
		UINT incrementSize,
		UINT blockSize);

	bool Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept;
	void Reset() noexcept;

private:

	// * ブロックが保持するHandleサイズ
	UINT m_blockSize;

	UINT m_incrementSize;

	// * 初期化時用意したDescriptサイズ範囲外アクセスを防ぐため監視
	UINT m_currentPosition;

	// * 次作成されるDescriptorが保持するメモリ位置
	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuBaseHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuBaseHandle;
};

//
// D3D12GlobalDescriptorHeap
//

class D3D12GlobalDescriptorHeap : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Initialilze(UINT totalSize, D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;

	D3D12GlobalDescriptorHeapBlock* Allocate() noexcept;
	void Deallocate(D3D12GlobalDescriptorHeapBlock* ptr) noexcept;

	/** アクセス */
	ID3D12DescriptorHeap* GetHeap() const noexcept { return m_descriptHeap.Get(); }

private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptHeap;

	// * 初期化設定
	const UINT m_splitSize = 2000;
	UINT m_incrementSize;
	UINT m_numDescriptors;
	D3D12_DESCRIPTOR_HEAP_TYPE m_heapType;

	// * 初期化時用意したDescriptサイズ範囲外アクセスを防ぐため監視
	UINT m_currentPosition;

	// * 次作成されるBlockが保持するメモリ位置
	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuCurrentHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuCurrentHandle;

	// * 解放されたBlockを再利用するために使用
	Vector<D3D12GlobalDescriptorHeapBlock*> m_freeBlocks;

	// * Allocateで生成されたBlockを管理
	Vector<UniquePtr<D3D12GlobalDescriptorHeapBlock>> m_blocks;
};

//
// D3D12GlobalSamplerDescriptorHeap
//

/** GlobalDescriptor の各ブロックを管理 */
class D3D12GlobalAllocatedDescriptorHeap
{
	COMPLETED_DEVELOPMENT()
public:

	void Initialilze(D3D12GlobalDescriptorHeap* globalDescriptHeap) noexcept;

	void Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept;
	void Reset() noexcept;

private:

	// * ブロックを取得するため保持
	D3D12GlobalDescriptorHeap* m_globalDescriptHeap;

	// * 使用中のブロックインデックス
	UINT m_blockIndex;

	Vector<D3D12GlobalDescriptorHeapBlock*> m_blocks;
};

//
// D3D12GlobalSamplerDescriptorHeap
//

class D3D12GlobalSamplerDescriptorHeap : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Initialilze() noexcept;

	void Allocate(UINT size, D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle) noexcept;

	/** アクセス */
	ID3D12DescriptorHeap* GetHeap() const noexcept { return m_descriptHeap.Get(); }

private:

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_descriptHeap;

	// * 次作成されるBlockが保持するメモリ位置
	D3D12_CPU_DESCRIPTOR_HANDLE	m_cpuCurrentHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE	m_gpuCurrentHandle;

	// * Sampler として確保できるDescriptorの最大サイズ
	const UINT m_numDescriptors = 2048;

	UINT m_incrementSize = 0;
	UINT m_currentPosition = 0;
};