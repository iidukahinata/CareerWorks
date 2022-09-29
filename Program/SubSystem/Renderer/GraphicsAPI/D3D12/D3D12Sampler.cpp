/**
* @file    D3D12Sampler.cpp
* @brief
*
* @date	   2022/08/02 2022年度初版
*/


#include "D3D12Sampler.h"
#include "D3D12GraphicsDevice.h"

D3D12Sampler::~D3D12Sampler()
{
	if (m_descriptor) m_descriptor->Release();
}

void D3D12Sampler::Create(const D3D12_SAMPLER_DESC& desc) noexcept
{
	if (auto sampler = GetSamplerMap().Find(desc))
	{
		sampler->m_descriptor->AddRef();
		m_descriptor = sampler->m_descriptor;
		m_id = sampler->m_id;
	}
	else
	{
		// ビュー作成
		m_descriptor = GetSamplerDescriptorAllocator().Allocate();
		GetDevice()->CreateSampler(&desc, m_descriptor->m_cpuHandle);

		m_id = GetSamplerMap().Add(desc, this);
	}

	m_samplerDesc = desc;
}

void D3D12Sampler::Create(D3D12_FILTER filter, D3D12_TEXTURE_ADDRESS_MODE addressU, D3D12_TEXTURE_ADDRESS_MODE addressV, D3D12_TEXTURE_ADDRESS_MODE addressW) noexcept
{
	D3D12_SAMPLER_DESC desc = {};
	desc.Filter = filter;
	desc.AddressU = addressU;
	desc.AddressV = addressV;
	desc.AddressW = addressW;

	return Create(desc);
}

void D3D12Sampler::VSSet(UINT slot) noexcept
{
	GetCommandContext()->SetSampler(VertexShader, slot, this);
}

void D3D12Sampler::PSSet(UINT slot) noexcept
{
	GetCommandContext()->SetSampler(PixelShader, slot, this);
}

void D3D12Sampler::GSSet(UINT slot) noexcept
{
	GetCommandContext()->SetSampler(GeometryShader, slot, this);
}