/**
* @file    D3D12CommandContext.cpp
* @brief

* @date	   2022/08/02 2022年度初版
*/


#include "D3D12CommandContext.h"
#include "D3D12GrahicsDevice.h"
#include "D3D12View.h"
#include "D3D12Sampler.h"
#include "D3D12RootSignature.h"
#include "D3D12PipelineState.h"
#include "D3D12DescriptorHeap.h"

bool D3D12CommandContext::Create() noexcept
{
	auto type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	auto hr = GetDevice()->CreateCommandAllocator(type, IID_PPV_ARGS(m_commandAllocator.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		LOG_ERROR("CommandAllocator の生成に失敗しました。");
		return false;
	}

	hr = GetDevice()->CreateCommandList(0, type, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(m_commandList.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		LOG_ERROR("CommandList の生成に失敗しました。");
		return false;
	}
	
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;//タイムアウトなし
	cmdQueueDesc.NodeMask = 0;
	cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//プライオリティ特に指定なし
	cmdQueueDesc.Type = type;

	hr = GetDevice()->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(m_commandQueue.ReleaseAndGetAddressOf()));//コマンドキュー生成	
	if (FAILED(hr)) {
		LOG_ERROR("CommandQueue の生成に失敗しました。");
		return false;
	}

	m_stateCache.Create();

	return true;
}

void D3D12CommandContext::BegineFream() noexcept
{
	m_stateCache.BegineFrame();
}

void D3D12CommandContext::RunCommandList() noexcept
{
	// 受付を終了
	m_commandList->Close();

	// コマンドリストをGPUに送信する
	ID3D12CommandList* cmdlists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(cmdlists), cmdlists);
}

void D3D12CommandContext::Clear(ID3D12PipelineState* const pPipelineSate /* = nullptr */) noexcept
{
	m_stateCache.Reset();

	m_commandAllocator->Reset();
	m_commandList->Reset(m_commandAllocator.Get(), pPipelineSate);
}

bool D3D12CommandContext::Signal(ID3D12Fence* const pFence, const UINT64& currentFenceValue) noexcept
{
	if (SUCCEEDED(m_commandQueue->Signal(pFence, currentFenceValue)))
	{
		return true;
	}
	return false;
}

void D3D12CommandContext::SetDescriptorHeap(D3D12DescriptorHeap* descriptorHeap) noexcept
{
	ID3D12DescriptorHeap* heaps[] = { descriptorHeap->GetHeap() };
	m_commandList->SetDescriptorHeaps(1, heaps);
}

void D3D12CommandContext::SetDescriptorHeaps(UINT numDescriptors, ID3D12DescriptorHeap** descriptorHeaps) noexcept
{
	m_commandList->SetDescriptorHeaps(numDescriptors, descriptorHeaps);
}

void D3D12CommandContext::SetGraphicsPipelineState(D3D12GraphicsPipelineState* pipelineState) noexcept
{
	m_stateCache.SetGraphicsPipelineState(pipelineState);
}

void D3D12CommandContext::SetComputePipelineState(D3D12ComputePipelineState* pipelineState) noexcept
{
	m_stateCache.SetComputePipelineState(pipelineState);
}

void D3D12CommandContext::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView, UINT index) noexcept
{
	m_stateCache.SetVertexBuffer(vertexBufferView, index);
}

void D3D12CommandContext::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView) noexcept
{
	m_stateCache.SetIndexBuffer(indexBufferView);
}

void D3D12CommandContext::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology) noexcept
{
	m_stateCache.SetPrimitiveTopology(primitiveTopology);
}

void D3D12CommandContext::SetRenderTargets(UINT numRenderTargets, D3D12RenderTargetView** renderTargets, D3D12DepthStencilView* depthStencil) noexcept
{
	m_stateCache.SetRenderTargets(numRenderTargets, renderTargets, depthStencil);
}

void D3D12CommandContext::SetViewPorts(UINT numViewPorts, D3D12_VIEWPORT* viewports) noexcept
{
	m_stateCache.SetViewPorts(numViewPorts, viewports);
}

void D3D12CommandContext::SetScissorRects(UINT numRects, D3D12_RECT* rects) noexcept
{
	m_stateCache.SetScissorRects(numRects, rects);
}

void D3D12CommandContext::SetConstantBuffer(ShaderType shaderType, UINT index, D3D12ConstantBufferView* constantBufferView) noexcept
{
	m_stateCache.SetConstantBuffer(shaderType, index, constantBufferView);
}

void D3D12CommandContext::SetShaderResource(ShaderType shaderType, UINT index, D3D12ShaderResourceView* shaderResourceView) noexcept
{
	m_stateCache.SetShaderResource(shaderType, index, shaderResourceView);
}

void D3D12CommandContext::SetUnorderedAccess(ShaderType shaderType, UINT index, D3D12UnorderedAccessView* unorderedAccessView) noexcept
{
	m_stateCache.SetUnorderedAccess(shaderType, index, unorderedAccessView);
}

void D3D12CommandContext::SetSampler(ShaderType shaderType, UINT index, D3D12Sampler* sampler) noexcept
{
	m_stateCache.SetSampler(shaderType, index, sampler);
}

void D3D12CommandContext::ClearRenderTargetView(D3D12RenderTargetView* renedrTarget, float* color, UINT numRects, D3D12_RECT* rects) noexcept
{
	m_commandList->ClearRenderTargetView(renedrTarget->GetCPUHandle(), color, numRects, rects);
}

void D3D12CommandContext::ClearDepthStencilView(D3D12DepthStencilView* depthStencil, D3D12_CLEAR_FLAGS clearFlags, float depth, UINT8 stencil, UINT numRects, D3D12_RECT* rects) noexcept
{
	m_commandList->ClearDepthStencilView(depthStencil->GetCPUHandle(), clearFlags, depth, stencil, numRects, rects);
}

void D3D12CommandContext::Dispth(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ) noexcept
{
	m_stateCache.Apply(PipelineType_Compute);
	GetCommandList()->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

void D3D12CommandContext::DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) noexcept
{
	m_stateCache.Apply(PipelineType_Graphics);
	GetCommandList()->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}