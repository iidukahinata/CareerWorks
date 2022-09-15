/**
* @file    D3D11IndexBuffer.cpp
* @brief
*
* @date	   2022/07/06 2022年度初版
*/


#include "D3D11IndexBuffer.h"

bool D3D11IndexBuffer::Create(const Vector<UINT>& indices, D3D11_USAGE usage /* = D3D11_USAGE_DEFAULT */) noexcept
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = (usage & D3D11_USAGE_DYNAMIC) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(UINT) * indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = (usage & D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices.data();

	HRESULT hr = GetDevice()->CreateBuffer(&bufferDesc, &InitData, m_indexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		LOG_ERROR("インデックスバッファー作成失敗 : D3D11IndexBuffer.h");
		return false;
	}
	return true;
}

void D3D11IndexBuffer::Update(const Vector<UINT>& indices) noexcept
{
	D3D11_MAPPED_SUBRESOURCE pData;
	HRESULT hr = GetContext()->Map(m_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr))
	{
		memcpy_s(pData.pData, pData.RowPitch, indices.data(), sizeof(UINT) * indices.size());
		GetContext()->Unmap(m_indexBuffer.Get(), 0);
	}
}

void D3D11IndexBuffer::IASet() const noexcept
{
	GetContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}