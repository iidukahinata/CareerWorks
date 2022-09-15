/**
* @file    D3D12View.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"

class D3D12Descriptor;

class D3D12View : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	~D3D12View();

	/** アクセス */
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle() noexcept;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() noexcept;

protected:

	D3D12Descriptor* m_descriptor = nullptr;
};

//
// D3D12ConstantBufferView
//

class D3D12ConstantBufferView : public D3D12View
{
	COMPLETED_DEVELOPMENT()
public:

	void Create(D3D12_GPU_VIRTUAL_ADDRESS bufferLocation, UINT sizeInBytes) noexcept;
	void Create(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc) noexcept;

	/** アクセス */
	D3D12_CONSTANT_BUFFER_VIEW_DESC GetDesc() const noexcept { return m_desc; }

private:

	D3D12_CONSTANT_BUFFER_VIEW_DESC m_desc;
};

//
// D3D12ShaderResourceView
//

class D3D12ShaderResourceView : public D3D12View
{
	COMPLETED_DEVELOPMENT()
public:

	void Create(ID3D12Resource* resource, const D3D12_RESOURCE_DESC& desc) noexcept;
	void Create(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc) noexcept;
};

//
// D3D12UnorderedAccessView
//

class D3D12UnorderedAccessView : public D3D12View
{
	COMPLETED_DEVELOPMENT()
public:

	void Create(ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc) noexcept;
};

//
//  D3D12RenderTargetView
//

class D3D12RenderTargetView : public D3D12View
{
	COMPLETED_DEVELOPMENT()
public:

	void Create(ID3D12Resource* resource, DXGI_FORMAT format) noexcept;
	void Create(ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC& desc) noexcept;
};

//
// D3D12DepthStencilView
//

class D3D12DepthStencilView : public D3D12View
{
	COMPLETED_DEVELOPMENT()
public:

	void Create(ID3D12Resource* resource) noexcept;
	void Create(ID3D12Resource* resource, const D3D12_DEPTH_STENCIL_VIEW_DESC& desc) noexcept;
};