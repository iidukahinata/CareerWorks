/**
* @file    D3D12View.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "D3D12View.h"
#include "D3D12DescriptorAllocator.h"

D3D12View::~D3D12View()
{
	if (m_descriptor) m_descriptor->Release();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12View::GetCPUHandle() noexcept
{
	return m_descriptor ? m_descriptor->m_cpuHandle : D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_GPU_DESCRIPTOR_HANDLE D3D12View::GetGPUHandle() noexcept
{
	return m_descriptor ? m_descriptor->m_gpuHandle : D3D12_GPU_DESCRIPTOR_HANDLE();
}

//
// D3D12ConstantBufferView
//

void D3D12ConstantBufferView::Create(D3D12_GPU_VIRTUAL_ADDRESS bufferLocation, UINT sizeInBytes) noexcept
{
	m_desc.BufferLocation = bufferLocation;
	m_desc.SizeInBytes = sizeInBytes;

	m_descriptor = GetViewDescriptorAllocator().Allocate();
	GetDevice()->CreateConstantBufferView(&m_desc, m_descriptor->m_cpuHandle);
}

void D3D12ConstantBufferView::Create(const D3D12_CONSTANT_BUFFER_VIEW_DESC& desc) noexcept
{
	m_desc = desc;

	m_descriptor = GetViewDescriptorAllocator().Allocate();
	GetDevice()->CreateConstantBufferView(&m_desc, m_descriptor->m_cpuHandle);
}

//
// D3D12ShaderResourceView
//

void D3D12ShaderResourceView::Create(ID3D12Resource* resource, const D3D12_RESOURCE_DESC& desc) noexcept
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = desc.Format;

	if (desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE1D)
	{
		if (desc.DepthOrArraySize == 1)
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MipLevels = desc.MipLevels;
			srvDesc.Texture1D.MostDetailedMip = 0;
			srvDesc.Texture1D.ResourceMinLODClamp = 0.0f;
		}
		else
		{
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			srvDesc.Texture1DArray.MipLevels = desc.MipLevels;
			srvDesc.Texture1DArray.MostDetailedMip = 0;
			srvDesc.Texture1DArray.ResourceMinLODClamp = 0.0f;
			srvDesc.Texture1DArray.FirstArraySlice = 0;
			srvDesc.Texture1DArray.ArraySize = 0;
		}
	}
	else if (desc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		if (desc.SampleDesc.Count == 1)
		{
			if (desc.DepthOrArraySize == 1)
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.PlaneSlice = 0;
				srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
			}
			else if (desc.DepthOrArraySize == 6)
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
				srvDesc.TextureCube.MipLevels = desc.MipLevels;
				srvDesc.TextureCube.MostDetailedMip = 0;
				srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			}
			else
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.MipLevels = desc.MipLevels;
				srvDesc.Texture2DArray.MostDetailedMip = 0;
				srvDesc.Texture2DArray.PlaneSlice = 0;
				srvDesc.Texture2DArray.ResourceMinLODClamp = 0.0f;
				srvDesc.Texture2DArray.FirstArraySlice = 0;
				srvDesc.Texture2DArray.ArraySize = 0;
			}
		}
		else
		{
			if (desc.DepthOrArraySize == 1)
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				srvDesc.Texture2DMSArray.FirstArraySlice = 0;
				srvDesc.Texture2DMSArray.ArraySize = 0;
			}
		}
	}
	else
	{
		srvDesc.Texture3D.MipLevels = desc.MipLevels;
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.ResourceMinLODClamp = 0.f;
	}

	Create(resource, srvDesc);
}

void D3D12ShaderResourceView::Create(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc) noexcept
{
	m_descriptor = GetViewDescriptorAllocator().Allocate();
	GetDevice()->CreateShaderResourceView(resource, &desc, m_descriptor->m_cpuHandle);
}

//
// D3D12UnorderedAccessView
//

void D3D12UnorderedAccessView::Create(ID3D12Resource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC& desc) noexcept
{
	m_descriptor = GetViewDescriptorAllocator().Allocate();
	GetDevice()->CreateUnorderedAccessView(resource, nullptr, &desc, m_descriptor->m_cpuHandle);
}

//
//  D3D12RenderTargetView
//

void D3D12RenderTargetView::Create(ID3D12Resource* resource, DXGI_FORMAT format) noexcept
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	m_descriptor = GetRtvDescriptorAllocator().Allocate();
	GetDevice()->CreateRenderTargetView(resource, nullptr, m_descriptor->m_cpuHandle);
}

void D3D12RenderTargetView::Create(ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC& desc) noexcept
{
	m_descriptor = GetRtvDescriptorAllocator().Allocate();
	GetDevice()->CreateRenderTargetView(resource, &desc, m_descriptor->m_cpuHandle);
}

//
// D3D12DepthStencilView
//

void D3D12DepthStencilView::Create(ID3D12Resource* resource) noexcept
{
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	desc.Flags = D3D12_DSV_FLAG_NONE;

	m_descriptor = GetDsvDescriptorAllocator().Allocate();
	GetDevice()->CreateDepthStencilView(resource, &desc, m_descriptor->m_cpuHandle);
}

void D3D12DepthStencilView::Create(ID3D12Resource* resource, const D3D12_DEPTH_STENCIL_VIEW_DESC& desc) noexcept
{
	m_descriptor = GetDsvDescriptorAllocator().Allocate();
	GetDevice()->CreateDepthStencilView(resource, &desc, m_descriptor->m_cpuHandle);
}