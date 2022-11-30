/**
* @file    D3D12Texture.cpp
* @brief
*
* @date	   2022/08/08 2022年度初版
*/


#include "D3D12Texture.h"
#include "D3D12CommandContext.h"

bool D3D12Texture::Create(const D3D12_RESOURCE_DESC* desc) noexcept
{
	auto texHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	auto ret = D3D12Buffer::Create(&texHeapProp, D3D12_HEAP_FLAG_NONE, desc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	if (!ret) {
		LOG_ERROR("ID3DResource の生成に失敗しました。");
		return false;
	}

	m_view.Create(m_buffer.Get(), GetDesc());

	return true;
}

bool D3D12Texture::Create(const DirectX::TexMetadata& meta) noexcept
{
	D3D12_RESOURCE_DESC texDesc = {};

	if (meta.dimension == DirectX::TEX_DIMENSION_TEXTURE1D)
	{
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	}
	else if (meta.dimension == DirectX::TEX_DIMENSION_TEXTURE2D)
	{
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	}
	else if (meta.dimension == DirectX::TEX_DIMENSION_TEXTURE3D)
	{
		texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	}
;
	//texDesc.Alignment;
	texDesc.DepthOrArraySize = meta.arraySize;
	//texDesc.Flags;
	texDesc.Format = meta.format;
	texDesc.Height = meta.height;
	//texDesc.Layout;
	texDesc.MipLevels = meta.mipLevels;
	texDesc.SampleDesc.Count = 1;
	texDesc.Width = meta.width;

	return Create(&texDesc);
}

bool D3D12Texture::Create(DXGI_FORMAT format, UINT width) noexcept
{
	auto texDesc = CD3DX12_RESOURCE_DESC::Tex1D(format, width, 1, 1);

	if (!Create(&texDesc))
	{
		return false;
	}

	return true;
}

bool D3D12Texture::Create(DXGI_FORMAT format, UINT width, UINT height) noexcept
{
	auto texDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height, 1, 1);

	if (!Create(&texDesc))
	{
		return false;
	}

	return true;
}

bool D3D12Texture::Create(DXGI_FORMAT format, UINT width, UINT height, UINT depth) noexcept
{
	auto texDesc = CD3DX12_RESOURCE_DESC::Tex3D(format, width, height, depth, 1);

	if (!Create(&texDesc)) 
	{
		return false;
	}

	return true;
}

bool D3D12Texture::Create(const DirectX::Image* images, size_t imageSize, const DirectX::TexMetadata& meta) noexcept
{
	if (!Create(meta))
	{
		return false;
	}

	for (int i = 0; i < imageSize; ++i)
	{
		auto hr = Get()->WriteToSubresource(i, nullptr, images[i].pixels, images[i].rowPitch, images[i].slicePitch);
		if (FAILED(hr)) {
			LOG_ERROR("データが正しく書き込めませんでした。");
		}
	}

	return true;
}

void D3D12Texture::VSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(VertexShader, slot, &m_view);
}

void D3D12Texture::PSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(PixelShader, slot, &m_view);
}

void D3D12Texture::GSSet(UINT slot /* = 0 */) noexcept
{
	GetCommandContext()->SetShaderResource(GeometryShader, slot, &m_view);
}