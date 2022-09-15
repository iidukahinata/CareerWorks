/**
* @file    D3D12ConstantBuffer.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12View.h"
#include "D3D12Buffer.h"

class D3D12ConstantBuffer : public D3D12Buffer
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(UINT bufferSize) noexcept;

	void VSSet(UINT slot = 0) noexcept;
	void PSSet(UINT slot = 0) noexcept;
	void GSSet(UINT slot = 0) noexcept;

	/** アクセス */
	D3D12ConstantBufferView& GetView() noexcept { return m_view; }

private:

	D3D12ConstantBufferView m_view;
};