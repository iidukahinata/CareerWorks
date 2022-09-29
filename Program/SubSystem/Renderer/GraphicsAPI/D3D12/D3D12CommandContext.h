/**
* @file    D3D12CommandContext.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"
#include "D3D12StateCache.h"

class D3D12DescriptorHeap;

class D3D12Fence : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	UINT64 m_fenceValue = 0;

public:

	~D3D12Fence();

	bool Create() noexcept;
	void WaitForSingleToFinish() noexcept;

	/** アクセス */
	ID3D12Fence* Get() const noexcept { return m_fence.Get(); }

private:

	// * GPUとの同期に使用
	Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

	// * GPU処理待機用WinAPIEvent
	HANDLE m_fenceEvent;
};

class D3D12CommandContext : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create() noexcept;

	void BeginFrame() noexcept;
	void RunCommandList() noexcept;
	void Clear(ID3D12PipelineState* const pPipelineSate = nullptr) noexcept;
	bool Signal(ID3D12Fence* const pFence, const UINT64& currentFenceValue) noexcept;

	/** set state */
	void SetDescriptorHeap(D3D12DescriptorHeap* descriptorHeap) noexcept;
	void SetDescriptorHeaps(UINT numDescriptors, ID3D12DescriptorHeap** descriptorHeaps) noexcept;
	void SetGraphicsPipelineState(D3D12GraphicsPipelineState* pipelineState) noexcept;
	void SetComputePipelineState(D3D12ComputePipelineState* pipelineState) noexcept;
	void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView, UINT index) noexcept;
	void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView) noexcept;
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitiveTopology) noexcept;
	void SetRenderTargets(UINT numRenderTargets, D3D12RenderTargetView** renderTargets, D3D12DepthStencilView* depthStencil) noexcept;
	void SetViewPorts(UINT numViewPorts, D3D12_VIEWPORT* viewports) noexcept;
	void SetScissorRects(UINT numRects, D3D12_RECT* rects) noexcept;
	void SetConstantBuffer(ShaderType shaderType, UINT index, D3D12ConstantBufferView* constantBufferView) noexcept;
	void SetShaderResource(ShaderType shaderType, UINT index, D3D12ShaderResourceView* shaderResourceView) noexcept;
	void SetUnorderedAccess(ShaderType shaderType, UINT index, D3D12UnorderedAccessView* unorderedAccessView) noexcept;
	void SetSampler(ShaderType shaderType, UINT index, D3D12Sampler* sampler) noexcept;

	/** クリア */
	void ClearRenderTargetView(D3D12RenderTargetView* renedrTarget, float* color, UINT numRects, D3D12_RECT* rects) noexcept;
	void ClearDepthStencilView(D3D12DepthStencilView* depthStencil, D3D12_CLEAR_FLAGS clearFlags, float depth, UINT8 stencil, UINT numRects, D3D12_RECT* rects) noexcept;

	/** ドローコール */
	void Dispth(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ) noexcept;
	void DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) noexcept;

	/** アクセス */
	ID3D12GraphicsCommandList* GetCommandList()      const noexcept { return m_commandList.Get(); }
	ID3D12CommandQueue*		   GetCommandQueue()     const noexcept { return m_commandQueue.Get(); }
	ID3D12CommandAllocator*	   GetCommandAllocator() const noexcept { return m_commandAllocator.Get(); }

private:

	// * GPUに命令するメソッドを記述していく
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

	// * コマンドの記憶領域
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;

	// * 実際にGPUにデータを渡す object
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;

	// * ドローコールまで描画設定を一時保存する入れ物
	D3D12StateCache m_stateCache;
};