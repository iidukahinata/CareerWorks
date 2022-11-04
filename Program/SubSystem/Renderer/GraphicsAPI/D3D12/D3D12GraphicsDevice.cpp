/**
* @file    D3D12GraphicsDevice.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "D3D12GraphicsDevice.h"

bool D3D12GraphicsDevice::Init(HWND hwnd, UINT screenWidth, UINT screenHeight, bool isFullscreen)
{
#ifdef _DEBUG
	Microsoft::WRL::ComPtr<ID3D12Debug> debugLayer = nullptr;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.GetAddressOf()));
	debugLayer->EnableDebugLayer();

	if (FAILED(hr)) {
		LOG_ERROR("DebugLayer を有効化出来ませんでした。");
		return false;
	}
#endif

	if (!InitDevice()) {
		LOG_ERROR("Device の生成に失敗しました。");
		return false;
	}

	Array<UINT, D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES> descriptorAllocSizes = { 1024, 128, 128, 32 };

	auto ret = m_viewDescriptprAllocator.Initialize(descriptorAllocSizes[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV], D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	if (!ret) {
		LOG_ERROR("viewDescriptprAllocator の初期化に失敗しました。");
		return false;
	}

	m_samplerDescriptprAllocator.Initialize(descriptorAllocSizes[D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER], D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	if (!ret) {
		LOG_ERROR("samplerDescriptprAllocator の初期化に失敗しました。");
		return false;
	}

	m_rtvDescriptprAllocator.Initialize(descriptorAllocSizes[D3D12_DESCRIPTOR_HEAP_TYPE_RTV], D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (!ret) {
		LOG_ERROR("rtvDescriptprAllocator の初期化に失敗しました。");
		return false;
	}

	m_dsvDescriptprAllocator.Initialize(descriptorAllocSizes[D3D12_DESCRIPTOR_HEAP_TYPE_DSV], D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	if (!ret) {
		LOG_ERROR("dsvDescriptprAllocator の初期化に失敗しました。");
		return false;
	}

	if (!m_context.Create()) {
		return false;
	}

	constexpr auto bufferCount = 2;
	if (!CreateSwapChain(hwnd, screenWidth, screenHeight, bufferCount)) {
		LOG_ERROR("SwapChain の生成に失敗しました。");
		return false;
	}

	if (!CreateBackBuffer()) {
		LOG_ERROR("BackBuffer の生成に失敗しました。");
		return false;
	}

	if (!CreateDepthStencil()) {
		LOG_ERROR("DepthStencil の生成に失敗しました。");
		return false;
	}

	if (!m_fence.Create()) {
		return false;
	}

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = screenWidth;
	m_viewport.Height = screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	m_scissorRect.bottom = static_cast<LONG>(screenHeight);
	m_scissorRect.top = 0;
	m_scissorRect.left = 0;
	m_scissorRect.right = static_cast<LONG>(screenWidth);

	return true;
}

void D3D12GraphicsDevice::BegineFrame()
{
	m_context.BeginFrame();

	// ビューポート、シザー矩形のセット
	m_context.SetViewPorts(1, &m_viewport);
	m_context.SetScissorRects(1, &m_scissorRect);
}

void D3D12GraphicsDevice::EndFrame()
{
	auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();
	ResourceBarrier(m_renderTargets[bbIdx], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	m_context.RunCommandList();

	WaitForGpuTask();

	m_context.Clear();
}

void D3D12GraphicsDevice::Present()
{
	m_swapchain->Present(1, 0);
}

void D3D12GraphicsDevice::SetRenderTarget()
{
	// バックバッファのインデックスを取得
	auto bbIdx = m_swapchain->GetCurrentBackBufferIndex();
	ResourceBarrier(m_renderTargets[bbIdx], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	D3D12RenderTargetView* renderTarges[] = {
		&m_renderTargetViews[bbIdx]
	};

	// レンダーターゲットを指定
	m_context.SetRenderTargets(1, renderTarges, &m_depthStencilView);

	// 画面クリア
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context.ClearRenderTargetView(&m_renderTargetViews[bbIdx], clearColor, 0, nullptr);
	m_context.ClearDepthStencilView(&m_depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void D3D12GraphicsDevice::SetViewport(float width, float height)
{
	D3D12_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_context.SetViewPorts(1, &viewport);
}

void D3D12GraphicsDevice::SetScissorRect(float width, float height)
{
	D3D12_RECT rect;
	rect.bottom = static_cast<LONG>(height);
	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<LONG>(width);
	m_context.SetScissorRects(1, &rect);
}

void D3D12GraphicsDevice::ResourceBarrier(ID3D12Resource* const pResource, D3D12_RESOURCE_STATES beforeState, D3D12_RESOURCE_STATES afterState)
{
	auto transition = CD3DX12_RESOURCE_BARRIER::Transition(pResource, beforeState, afterState);
	m_context.GetCommandList()->ResourceBarrier(1, &transition);
}

void D3D12GraphicsDevice::WaitForGpuTask()
{
	// 待ち
	m_context.Signal(m_fence.Get(), m_fence.m_fenceValue);

	m_fence.WaitForSingleToFinish();
}

bool D3D12GraphicsDevice::InitDevice()
{
	// 機能レベル
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

#ifdef _DEBUG
	auto hr = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(m_factory.ReleaseAndGetAddressOf()));
#else
	auto hr = CreateDXGIFactory1(IID_PPV_ARGS(factory.ReleaseAndGetAddressOf()));
#endif // _DEBUG
	if (FAILED(hr)) {
		return false;
	}

	IDXGIAdapter* adapter = GetAdapterByName(L"NVIDIA");
	if (!adapter)
	{
		return false;
	}

	// 指定されたアダプターでデバイスの生成
	hr = S_FALSE;
	for (auto level : levels)
	{
		hr = D3D12CreateDevice(adapter, level, IID_PPV_ARGS(m_device.ReleaseAndGetAddressOf()));
		if (SUCCEEDED(hr)) {
			break;
		}
	}

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool D3D12GraphicsDevice::CreateSwapChain(const HWND hwnd, UINT screenWidth, UINT screenHeight, UINT bufferCount)
{
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
	swapchainDesc.Width = screenWidth;
	swapchainDesc.Height = screenHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.Stereo = false;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.SampleDesc.Quality = 0;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = bufferCount;
	swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto hr = m_factory->CreateSwapChainForHwnd(
		m_context.GetCommandQueue(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)m_swapchain.ReleaseAndGetAddressOf());

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool D3D12GraphicsDevice::CreateBackBuffer()
{
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	auto hr = m_swapchain->GetDesc(&swcDesc);
	auto bufferFormat = swcDesc.BufferDesc.Format;

	// SRGBレンダーターゲットビュー設定
	m_renderTargets.resize(swcDesc.BufferCount);
	for (int i = 0; i < swcDesc.BufferCount; ++i) 
	{
		hr = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (FAILED(hr)) {
			return false;
		}

		m_renderTargetViews[i].Create(m_renderTargets[i], m_renderTargets[i]->GetDesc().Format);
	}

	auto isHDR = false;

#if defined(NTDDI_WIN10_RS2)
	Microsoft::WRL::ComPtr<IDXGIOutput> output;
	if (SUCCEEDED(m_swapchain->GetContainingOutput(output.GetAddressOf())))
	{
		Microsoft::WRL::ComPtr<IDXGIOutput6> output6;
		if (SUCCEEDED(output.As(&output6)))
		{
			DXGI_OUTPUT_DESC1 desc;
			hr = output6->GetDesc1(&desc);
			if (FAILED(hr))
			{
				return false;
			}

			isHDR = desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
		}
	}
#endif

	DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

	if (isHDR)
	{
		switch (bufferFormat)
		{
		case DXGI_FORMAT_R10G10B10A2_UNORM: colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020; break;
		case DXGI_FORMAT_R16G16B16A16_FLOAT: colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709; break;
		default: break;
		}
	}

	UINT colorSpaceSupport = 0;
	if (SUCCEEDED(m_swapchain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)))
	{
		if (colorSpaceSupport == DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT)
		{
			hr = m_swapchain->SetColorSpace1(colorSpace);
			if (FAILED(hr)) {
				return false;
			}
		}
	}

	return true;
}

bool D3D12GraphicsDevice::CreateDepthStencil()
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	auto hr = m_swapchain->GetDesc1(&desc);
	if (FAILED(hr)) {
		return false;
	}

	CD3DX12_RESOURCE_DESC resdesc(
		D3D12_RESOURCE_DIMENSION_TEXTURE2D,
		0,
		desc.Width,
		desc.Height,
		1,
		1,
		DXGI_FORMAT_D32_FLOAT,
		1,
		0,
		D3D12_TEXTURE_LAYOUT_UNKNOWN,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

	// DSV用ヒーププロパティ
	auto depthHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto depthClearValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	hr = m_device->CreateCommittedResource(
		&depthHeapProp, D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(m_depthStencil.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		return false;
	}

	// ディスクリプタを作成
	m_depthStencilView.Create(m_depthStencil.Get());

	return true;
}

Vector<IDXGIAdapter*> D3D12GraphicsDevice::GetAvailableAdapters()
{
	Vector <IDXGIAdapter*> adapters;
	IDXGIAdapter* adapter = nullptr;
	for (int i = 0; m_factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapters.push_back(adapter);
	}
	return adapters;
}

IDXGIAdapter* D3D12GraphicsDevice::GetAdapterByName(WstringView name)
{
	Vector<IDXGIAdapter*> adapters = GetAvailableAdapters();

	for (auto adpter : adapters)
	{
		DXGI_ADAPTER_DESC adesc = {};
		adpter->GetDesc(&adesc);

		Wstring strDesc = adesc.Description;
		if (strDesc.find(name.data()) != String::npos) {
			return adpter;
		}
	}
	return nullptr;
}

IDXGIAdapter* D3D12GraphicsDevice::GetAdapterWithTheHighestVRAM()
{
	auto adapters = GetAvailableAdapters();

	IDXGIAdapter* resultAdapter = nullptr;
	UINT maxVRAMSize = 0;

	DXGI_ADAPTER_DESC adesc = {};
	for (auto adapter : adapters)
	{
		adapter->GetDesc(&adesc);
		if (adesc.DedicatedVideoMemory > maxVRAMSize)
		{
			maxVRAMSize = adesc.DedicatedVideoMemory;
			resultAdapter = adapter;
		}
	}

	return resultAdapter;
}