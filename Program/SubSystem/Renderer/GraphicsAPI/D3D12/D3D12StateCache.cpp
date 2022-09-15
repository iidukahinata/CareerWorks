/**
* @file    D3D12StateCache.cpp
* @brief

* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "D3D12StateCache.h"
#include "D3D12View.h"
#include "D3D12Sampler.h"
#include "D3D12RootSignature.h"
#include "D3D12PipelineState.h"
#include "D3D12CommandContext.h"

void D3D12StateCache::Create() noexcept
{
	m_descriptorCache.Create();
}

void D3D12StateCache::BegineFrame() noexcept
{
	m_descriptorCache.BegineFrame();
}

void D3D12StateCache::Reset() noexcept
{
	m_state.m_srvCache.Clear();
	m_state.m_uavCache.Clear();
	m_state.m_cbvCache.Clear();
	m_state.m_samplerCache.Clear();
}

void D3D12StateCache::Apply(PipelineType type) noexcept
{
	// PSO
	{
		if (m_setRootSignature)
		{
			if (type == PipelineType_Graphics)
			{
				GetCommandContext()->GetCommandList()->SetGraphicsRootSignature(m_state.m_rootSignature->Get());
			}
			else
			{
				GetCommandContext()->GetCommandList()->SetComputeRootSignature(m_state.m_rootSignature->Get());
			}
		}

		if (m_setPipelineState)
		{
			GetCommandContext()->GetCommandList()->SetPipelineState(m_state.m_pipelineState);
		}
	}

	// not has Heap bindings
	if(type == PipelineType_Graphics)
	{
		if (m_setVertexBuffer)
		{
			GetCommandContext()->GetCommandList()->IASetVertexBuffers(0, m_state.m_numVertexBufferViews, m_state.m_vertexBufferViews.data());
		}
		if (m_setIndexBuffer)
		{
			GetCommandContext()->GetCommandList()->IASetIndexBuffer(&m_state.m_indexBufferView);
		}
		if (m_setPrimitiveTopology)
		{
			GetCommandContext()->GetCommandList()->IASetPrimitiveTopology(m_state.m_primitiveTopology);
		}
		if (m_setRenderTargets)
		{
			m_descriptorCache.SetRenderTargets(m_state.m_numRenderTargets, m_state.m_renderTargets.data(), m_state.m_depthStencil);
		}
		if (m_setViewports)
		{
			GetCommandContext()->GetCommandList()->RSSetViewports(m_state.m_numViewports, m_state.m_viewports.data());
		}
		if (m_setScissorRects)
		{
			GetCommandContext()->GetCommandList()->RSSetScissorRects(m_state.m_numScissorRects, m_state.m_scissorRects.data());
		}
	}

	// has Heap bindings
	if (m_state.m_rootSignature)
	{
		m_descriptorCache.SetDescriptorHeap();

		auto SetSampler = [this](ShaderType type) {
			m_descriptorCache.SetSampler(type, m_state.m_samplerCache, m_state.m_rootSignature);
		};

		if (type == PipelineType_Graphics)
		{
			SetSampler(VertexShader);
			SetSampler(PixelShader);
			SetSampler(GeometryShader);
		}
		else
		{
			SetSampler(ComputeShader);
		}

		auto SetViews = [this](ShaderType type) {
			m_descriptorCache.SetConstantBufferView(type, m_state.m_cbvCache, m_state.m_rootSignature);
			m_descriptorCache.SetShaderResourceView(type, m_state.m_srvCache, m_state.m_rootSignature);
			m_descriptorCache.SetUnorderedAccessView(type, m_state.m_uavCache, m_state.m_rootSignature);
		};

		if (type == PipelineType_Graphics)
		{
			SetViews(VertexShader);
			SetViews(PixelShader);
			SetViews(GeometryShader);
		}
		else
		{
			SetViews(ComputeShader);
		}
	}

	Flush();
}

void D3D12StateCache::SetGraphicsPipelineState(D3D12GraphicsPipelineState* pipelineState) noexcept
{
	ASSERT(pipelineState);

	auto primitiveType = static_cast<D3D_PRIMITIVE_TOPOLOGY>(pipelineState->GetDesc().PrimitiveType);
	SetPrimitiveTopology(primitiveType);

	if (auto rootSignature = pipelineState->GetRootSignature())
	{
		m_state.m_rootSignature = rootSignature;
		m_setRootSignature = true;
	}

	m_state.m_pipelineState = pipelineState->Get();
	m_setPipelineState = true;
}

void D3D12StateCache::SetComputePipelineState(D3D12ComputePipelineState* pipelineState) noexcept
{
	ASSERT(pipelineState);

	if (auto rootSignature = pipelineState->GetRootSignature())
	{
		m_state.m_rootSignature = rootSignature;
		m_setRootSignature = true;
	}

	m_state.m_pipelineState = pipelineState->Get();
	m_setPipelineState = true;
}

void D3D12StateCache::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView, UINT index) noexcept
{
	ASSERT(index < MAX_VERTEX_BUFFER_VIEW);
	ASSERT(vertexBufferView);

	m_state.m_vertexBufferViews[index].BufferLocation = vertexBufferView->BufferLocation;
	m_state.m_vertexBufferViews[index].StrideInBytes = vertexBufferView->StrideInBytes;
	m_state.m_vertexBufferViews[index].SizeInBytes = vertexBufferView->SizeInBytes;

	m_state.m_vertexBufferViewMask |= 1 << (index + 1);
	m_state.m_numVertexBufferViews = index;

	auto max = GetMsb(m_state.m_vertexBufferViewMask);
	m_state.m_numVertexBufferViews = max;

	m_setVertexBuffer = true;
}

void D3D12StateCache::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView) noexcept
{
	ASSERT(indexBufferView);

	m_state.m_indexBufferView.BufferLocation = indexBufferView->BufferLocation;
	m_state.m_indexBufferView.Format = indexBufferView->Format;
	m_state.m_indexBufferView.SizeInBytes = indexBufferView->SizeInBytes;
	m_setIndexBuffer = true;
}

void D3D12StateCache::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology) noexcept
{
	m_state.m_primitiveTopology = primitiveTopology;
	m_setPrimitiveTopology = true;
}

void D3D12StateCache::SetRenderTargets(UINT numRenderTargets, D3D12RenderTargetView** renderTargets, D3D12DepthStencilView* depthStencil) noexcept
{
	ASSERT(numRenderTargets < MAX_RENDER_TARGET);
	ASSERT(renderTargets);
	ASSERT(depthStencil);

	constexpr UINT maxSize = sizeof(D3D12RenderTargetView*) * MAX_RENDER_TARGET;

	m_state.m_depthStencil = depthStencil;
	m_state.m_numRenderTargets = numRenderTargets;
	memcpy_s(m_state.m_renderTargets.data(), maxSize, renderTargets, sizeof(D3D12RenderTargetView) * numRenderTargets);

	m_setRenderTargets = true;
}

void D3D12StateCache::SetViewPorts(UINT numViewPorts, D3D12_VIEWPORT* viewports) noexcept
{
	ASSERT(numViewPorts < MAX_VIEWPORT);
	ASSERT(viewports);

	constexpr UINT maxSize = sizeof(D3D12_VIEWPORT) * MAX_VIEWPORT;

	m_state.m_numViewports = numViewPorts;
	memcpy_s(m_state.m_viewports.data(), maxSize, viewports, sizeof(D3D12_VIEWPORT) * numViewPorts);

	m_setViewports = true;
}

void D3D12StateCache::SetScissorRects(UINT numScissorRects, D3D12_RECT* scissorRects) noexcept
{
	ASSERT(numScissorRects < MAX_SCISSORRECT);
	ASSERT(scissorRects);

	constexpr UINT maxSize = sizeof(D3D12_RECT) * MAX_SCISSORRECT;

	m_state.m_numScissorRects = numScissorRects;
	memcpy_s(m_state.m_scissorRects.data(), maxSize, scissorRects, sizeof(D3D12_RECT) * numScissorRects);

	m_setScissorRects = true;
}

void D3D12StateCache::SetConstantBuffer(ShaderType shaderType, UINT index, D3D12ConstantBufferView* constantBufferView) noexcept
{
	ASSERT(index < MAX_CONSTANT_BUFFER_VIEW);
	ASSERT(constantBufferView);

	auto& views = m_state.m_cbvCache.views[shaderType];
	auto& indexMasks = m_state.m_cbvCache.indexMasks[shaderType];

	views[index] = constantBufferView;
	indexMasks |= (1 << (index + 1));
}

void D3D12StateCache::SetShaderResource(ShaderType shaderType, UINT index, D3D12ShaderResourceView* shaderResourceView) noexcept
{
	ASSERT(index < MAX_SHADER_RESOURCE_VIEW);
	ASSERT(shaderResourceView);

	auto& views = m_state.m_srvCache.views[shaderType];
	auto& indexMasks = m_state.m_srvCache.indexMasks[shaderType];

	views[index] = shaderResourceView;
	indexMasks |= (1 << (index + 1));
}

void D3D12StateCache::SetUnorderedAccess(ShaderType shaderType, UINT index, D3D12UnorderedAccessView* unorderedAccessView) noexcept
{
	ASSERT(index < MAX_UNORDERED_ACCESS_VIEW);
	ASSERT(unorderedAccessView);

	auto& views = m_state.m_uavCache.views[shaderType];
	auto& indexMasks = m_state.m_uavCache.indexMasks[shaderType];

	views[index] = unorderedAccessView;
	indexMasks |= (1 << (index + 1));
}

void D3D12StateCache::SetSampler(ShaderType shaderType, UINT index, D3D12Sampler* sampler) noexcept
{
	ASSERT(index < MAX_SAMPLER);
	ASSERT(sampler);

	auto& views = m_state.m_samplerCache.views[shaderType];
	auto& indexMasks = m_state.m_samplerCache.indexMasks[shaderType];

	views[index] = sampler;
	indexMasks |= (1 << index);
}

void D3D12StateCache::Flush() noexcept
{
	m_state.m_vertexBufferViewMask = 0;

	m_setPipelineState = false;
	m_setRootSignature = false;
	m_setVertexBuffer = false;
	m_setIndexBuffer = false;
	m_setPrimitiveTopology = false;
	m_setRenderTargets = false;
	m_setViewports = false;
	m_setScissorRects = false;
}