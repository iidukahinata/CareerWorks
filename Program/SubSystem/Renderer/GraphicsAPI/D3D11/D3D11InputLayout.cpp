/**
* @file    ID3D11InputLayout.cpp
* @brief
*
* @date	   2022/06/25 2022年度初版
*/


#include "D3D11InputLayout.h"

bool D3D11InputLayout::Create(D3D11_INPUT_ELEMENT_DESC* layout, UINT layoutSize, ID3DBlob* blob) noexcept
{
	if (!layout) {
		LOG_ERROR("layoutを指定してください。");
		return false;
	}

	if (!blob) {
		LOG_ERROR("blobに値をいれてください。");
		return false;
	}

	//頂点レイアウト作成
	HRESULT hr = GetDevice()->CreateInputLayout(
		layout,
		layoutSize,
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		m_inputLayout.ReleaseAndGetAddressOf());

	if (FAILED(hr)) {
		LOG_ERROR("InputLayout作成に失敗。");
		return false;
	}

	return true;
}

void D3D11InputLayout::IASet() const noexcept
{
	GetContext()->IASetInputLayout(m_inputLayout.Get());
}