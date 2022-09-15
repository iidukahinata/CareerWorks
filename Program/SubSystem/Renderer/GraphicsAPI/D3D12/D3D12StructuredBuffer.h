/**
* @file    D3D12StructuredBuffer.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12View.h"
#include "D3D12Buffer.h"

class D3D12StructuredBuffer : public D3D12Buffer
{
public:

	bool Create(
		UINT bufferSize,
		UINT elementsCount,
		UINT byteStride,
		D3D12_HEAP_TYPE heapType = D3D12_HEAP_TYPE_UPLOAD,
		D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_GENERIC_READ
	);

	void VSSet(UINT slot = 0) noexcept;
	void PSSet(UINT slot = 0) noexcept;
	void GSSet(UINT slot = 0) noexcept;
	void CSSet(UINT slot = 0) noexcept;

	/** アクセス */
	D3D12ShaderResourceView& GetView() noexcept { return m_view; }

private:

	D3D12ShaderResourceView m_view;
};