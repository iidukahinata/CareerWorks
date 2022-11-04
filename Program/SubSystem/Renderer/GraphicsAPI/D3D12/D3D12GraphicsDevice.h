/**
* @file    D3D12GraphicsDevice.h
* @brief
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


#include <dxgi1_6.h>
#include "D3D12View.h"
#include "D3D12Helper.h"
#include "D3D12CommandContext.h"
#include "D3D12DescriptorAllocator.h"

class D3D12Sampler;

class D3D12GraphicsDevice
{
	COMPLETED_DEVELOPMENT()

		D3D12GraphicsDevice() = default;
public:

	static D3D12GraphicsDevice& Get() noexcept
	{
		static D3D12GraphicsDevice instance;
		return instance;
	}

	// 初期化
	bool Init(HWND hwnd, UINT screenWidth, UINT screenHeight, bool isFullscreen);

	// 更新
	void BegineFrame();
	void EndFrame();
	void Present();
	void WaitForGpuTask();

	// RenderTarget
	void SetRenderTarget();

	// ViewPort
	void SetViewport(float width, float height);

	// ScissorRect
	void SetScissorRect(float width, float height);

	// Barrier
	void ResourceBarrier(ID3D12Resource* const pResource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState);

	/** アクセス */
	ID3D12Device*			   GetDevice()					const noexcept { return m_device.Get(); }
	D3D12CommandContext&       GetCommandContext()				  noexcept { return m_context; }
	IDXGISwapChain4*		   GetSwapchain()				const noexcept { return m_swapchain.Get(); }
	D3D12DescriptorAllocator&  GetViewDescriptorAllocator()		  noexcept { return m_viewDescriptprAllocator; }
	D3D12DescriptorAllocator&  GetSamplerDescriptorAllocator()	  noexcept { return m_samplerDescriptprAllocator; }
	D3D12DescriptorAllocator&  GetRtvDescriptorAllocator()		  noexcept { return m_rtvDescriptprAllocator; }
	D3D12DescriptorAllocator&  GetDsvDescriptorAllocator()		  noexcept { return m_dsvDescriptprAllocator; }
	D3D12Map<D3D12_SAMPLER_DESC, D3D12Sampler*>& GetSamplerMap()  noexcept { return m_samplerMap; }

private:

	bool InitDevice();
	bool CreateSwapChain(const HWND hwnd, UINT screenWidth, UINT screenHeight, UINT bufferCount);
	bool CreateBackBuffer();
	bool CreateDepthStencil();

	Vector<IDXGIAdapter*> GetAvailableAdapters();
	IDXGIAdapter* GetAdapterByName(WstringView name);
	IDXGIAdapter* GetAdapterWithTheHighestVRAM();

private:

	Microsoft::WRL::ComPtr<IDXGIFactory4>			m_factory;

	// * Direct3D objects
	Microsoft::WRL::ComPtr<ID3D12Device>			m_device;
	D3D12CommandContext								m_context;

	// * swap chain objects
	Microsoft::WRL::ComPtr<IDXGISwapChain4>			m_swapchain;
	Vector<ID3D12Resource*>							m_renderTargets;
	Microsoft::WRL::ComPtr<ID3D12Resource>			m_depthStencil;

	// * fence object
	D3D12Fence										m_fence;

	// * rendering objects
	D3D12RenderTargetView							m_renderTargetViews[2];
	D3D12DepthStencilView							m_depthStencilView;
	D3D12_VIEWPORT                                  m_viewport = {};
	D3D12_RECT                                      m_scissorRect = {};

	// * allocator objects
	D3D12DescriptorAllocator						m_viewDescriptprAllocator;
	D3D12DescriptorAllocator						m_samplerDescriptprAllocator;
	D3D12DescriptorAllocator						m_rtvDescriptprAllocator;
	D3D12DescriptorAllocator						m_dsvDescriptprAllocator;

	D3D12Map<D3D12_SAMPLER_DESC, D3D12Sampler*>		m_samplerMap;
};