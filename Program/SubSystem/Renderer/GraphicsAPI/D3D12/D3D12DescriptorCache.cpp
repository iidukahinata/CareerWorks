/**
* @file    D3D12DescriptorCache.cpp
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/


#include "D3D12DescriptorCache.h"
#include "D3D12StateCache.h"
#include "D3D12RootSignature.h"
#include "D3D12CommandContext.h"

void D3D12DescriptorCache::Create() noexcept
{
	// init global descriptor
	ASSERT(m_globalDescriptprHeap.Initialilze(50000, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	ASSERT(m_globalSamplerDescriptprHeap.Initialilze());
	m_globalAllocatedDescriptprHeap.Initialilze(&m_globalDescriptprHeap);

	// create null objects
	nullRenderTarget = GetRtvDescriptorAllocator().Allocate();
	nullConstantBuffer = GetViewDescriptorAllocator().Allocate();
	nullShaderResource = GetViewDescriptorAllocator().Allocate();
	nullUnorderedAccess = GetViewDescriptorAllocator().Allocate();
	nullSampler = GetSamplerDescriptorAllocator().Allocate();
}

void D3D12DescriptorCache::BegineFrame() noexcept
{
	m_globalAllocatedDescriptprHeap.Reset();
}

void D3D12DescriptorCache::SetDescriptorHeap() noexcept
{
	auto viewDescriptorHeap = m_globalDescriptprHeap.GetHeap();
	auto samplerDescriptorHeap = m_globalSamplerDescriptprHeap.GetHeap();

	ID3D12DescriptorHeap* heaps[] = {
		viewDescriptorHeap,
		samplerDescriptorHeap
	};

	GetCommandContext()->SetDescriptorHeaps(_countof(heaps), heaps);
}

void D3D12DescriptorCache::SetRenderTargets(UINT numRenderTargets, D3D12RenderTargetView** renderTargets, D3D12DepthStencilView* depthStencil) noexcept
{
	Array<D3D12_CPU_DESCRIPTOR_HANDLE, MAX_RENDER_TARGET> renderTargetHandles;
	for (int i = 0; i < numRenderTargets; ++i)
	{
		if (renderTargets[i])
		{
			renderTargetHandles[i] = renderTargets[i]->GetCPUHandle();
		}
		else
		{
			renderTargetHandles[i] = nullRenderTarget->m_cpuHandle;
		}
	}

	if (depthStencil)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilHandle = depthStencil->GetCPUHandle();
		GetCommandContext()->GetCommandList()->OMSetRenderTargets(numRenderTargets, renderTargetHandles.data(), 0, &depthStencilHandle);
	}
	else
	{
		GetCommandContext()->GetCommandList()->OMSetRenderTargets(numRenderTargets, renderTargetHandles.data(), 0, nullptr);
	}
}

void D3D12DescriptorCache::SetConstantBufferView(ShaderType type, ConstantBufferViewCache& cache, D3D12RootSignature* rootSignature) noexcept
{
	auto views = cache.views[type];
	auto indexMask = cache.indexMasks[type];

	if (indexMask == 0) {
		return;
	}

	UINT size = GetMsb(indexMask);

	Array<D3D12_CPU_DESCRIPTOR_HANDLE, MAX_CONSTANT_BUFFER_VIEW> constantBufferViews;

	for (int i = 0; i < size; ++i)
	{
		if (views[i])
		{
			constantBufferViews[i] = views[i]->GetCPUHandle();
		}
		else
		{
			constantBufferViews[i] = nullConstantBuffer->m_cpuHandle;
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	m_globalAllocatedDescriptprHeap.Allocate(size, &cpuHandle, &gpuHandle);
	GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, constantBufferViews.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	const UINT rootPramIndex = rootSignature->GetCBVRootPramIndex(type);

	if (type == ComputeShader)
	{
		GetCommandContext()->GetCommandList()->SetComputeRootDescriptorTable(rootPramIndex, gpuHandle);
	}
	else
	{
		GetCommandContext()->GetCommandList()->SetGraphicsRootDescriptorTable(rootPramIndex, gpuHandle);
	}
}

void D3D12DescriptorCache::SetShaderResourceView(ShaderType type, ShaderResourceViewCache& cache, D3D12RootSignature* rootSignature) noexcept
{
	auto views = cache.views[type];
	auto indexMask = cache.indexMasks[type];

	if (indexMask == 0) {
		return;
	}

	UINT size = GetMsb(indexMask);

	Array<D3D12_CPU_DESCRIPTOR_HANDLE, MAX_SHADER_RESOURCE_VIEW> shaderResourceViews;

	for (int i = 0; i < size; ++i)
	{
		if (views[i])
		{
			shaderResourceViews[i] = views[i]->GetCPUHandle();
		}
		else
		{
			shaderResourceViews[i] = nullShaderResource->m_cpuHandle;
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	m_globalAllocatedDescriptprHeap.Allocate(size, &cpuHandle, &gpuHandle);
	GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, shaderResourceViews.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	const UINT rootPramIndex = rootSignature->GetSRVRootPramIndex(type);

	if (type == ComputeShader)
	{
		GetCommandContext()->GetCommandList()->SetComputeRootDescriptorTable(rootPramIndex, gpuHandle);
	}
	else
	{
		GetCommandContext()->GetCommandList()->SetGraphicsRootDescriptorTable(rootPramIndex, gpuHandle);
	}
}

void D3D12DescriptorCache::SetUnorderedAccessView(ShaderType type, UnorderedAccessViewCache& cache, D3D12RootSignature* rootSignature) noexcept
{
	auto views = cache.views[type];
	auto indexMask = cache.indexMasks[type];

	if (indexMask == 0) {
		return;
	}

	UINT size = GetMsb(indexMask);

	Array<D3D12_CPU_DESCRIPTOR_HANDLE, MAX_CONSTANT_BUFFER_VIEW> unorderedAccessViews;

	for (int i = 0; i < size; ++i)
	{
		if (views[i])
		{
			unorderedAccessViews[i] = views[i]->GetCPUHandle();
		}
		else
		{
			unorderedAccessViews[i] = nullUnorderedAccess->m_cpuHandle;
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	m_globalAllocatedDescriptprHeap.Allocate(size, &cpuHandle, &gpuHandle);
	GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, unorderedAccessViews.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	const UINT rootPramIndex = rootSignature->GetUAVRootPramIndex(type);

	if (type == ComputeShader)
	{
		GetCommandContext()->GetCommandList()->SetComputeRootDescriptorTable(rootPramIndex, gpuHandle);
	}
	else
	{
		GetCommandContext()->GetCommandList()->SetGraphicsRootDescriptorTable(rootPramIndex, gpuHandle);
	}
}

void D3D12DescriptorCache::SetSampler(ShaderType type, SamplerCache& cache, D3D12RootSignature* rootSignature) noexcept
{
	auto views = cache.views[type];
	auto indexMask = cache.indexMasks[type];

	if (indexMask == 0) {
		return;
	}

	UINT size = GetMsb(indexMask);

	// �g���񂵂̂��߂� sampler �̍\�����擾
	SamplerArrayDesc desc = {};
	for (int i = 0; i < size; ++i)
	{
		desc.m_ids[i] = views[i]->GetID();
	}

	// sampler �͓����g�ݍ��킹���ߋ��ɑ��݂���ꍇ�͎g����
	auto handle = m_samplerMap.Find(desc);
	if (handle.ptr > 0)
	{
		const UINT rootPramIndex = rootSignature->GetSamplerRootPramIndex(type);
		GetCommandContext()->GetCommandList()->SetGraphicsRootDescriptorTable(rootPramIndex, handle);
		return;
	}

	// �n�߂Ă̑g�ݍ��킹�͐V����Global��Ԃɓo�^������g�p�ł���悤�ɂ���B
	Array<D3D12_CPU_DESCRIPTOR_HANDLE, MAX_SAMPLER> samplers;

	for (int i = 0; i < size; ++i)
	{
		if (views[i])
		{
			samplers[i] = views[i]->GetCPUHandle();
		}
		else
		{
			samplers[i] = nullSampler->m_cpuHandle;
		}
	}

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {};
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};
	m_globalSamplerDescriptprHeap.Allocate(size, &cpuHandle, &gpuHandle);
	GetDevice()->CopyDescriptors(1, &cpuHandle, &size, size, samplers.data(), nullptr, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	// ����g�p�ł���悤�ɓo�^
	m_samplerMap.Add(desc, gpuHandle);

	const UINT rootPramIndex = rootSignature->GetSamplerRootPramIndex(type);

	if (type == ComputeShader)
	{
		GetCommandContext()->GetCommandList()->SetComputeRootDescriptorTable(rootPramIndex, gpuHandle);
	}
	else
	{
		GetCommandContext()->GetCommandList()->SetGraphicsRootDescriptorTable(rootPramIndex, gpuHandle);
	}
}