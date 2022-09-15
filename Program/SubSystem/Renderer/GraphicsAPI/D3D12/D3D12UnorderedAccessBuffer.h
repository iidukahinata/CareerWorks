/**
* @file    D3D12ConstantBuffer.h
* @brief
*
* @date	   2022/08/30 2022�N�x����
*/
#pragma once


#include "D3D12View.h"
#include "D3D12Buffer.h"


class D3D12UnorderedAccessBuffer : public D3D12Buffer
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(
		UINT bufferSize,
		UINT elementCount,
		UINT byteStride,
		UINT offsetInBytes) noexcept;

	void VSSet(UINT slot = 0) noexcept;
	void PSSet(UINT slot = 0) noexcept;
	void GSSet(UINT slot = 0) noexcept;
	void CSSet(UINT slot = 0) noexcept;

	/** �A�N�Z�X */
	D3D12UnorderedAccessView& GetView() noexcept { return m_view; }

private:

	D3D12UnorderedAccessView m_view;
};