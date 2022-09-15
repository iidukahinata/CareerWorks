/**
* @file    D3D12IndexBuffer.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12Buffer.h"

class D3D12IndexBuffer : public D3D12Buffer
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(const Vector<UINT>& indices) noexcept;
	bool Create(const Vector<UINT16>& indices) noexcept;

	void IASet() noexcept;

	/** アクセス */
	D3D12_INDEX_BUFFER_VIEW& GetBufferView() noexcept { return m_bufferView; }

private:

	template<class T>
	bool Create(const Vector<T>& indices, DXGI_FORMAT format) noexcept;

private:

	D3D12_INDEX_BUFFER_VIEW m_bufferView;
};

template<class T>
FORCEINLINE bool D3D12IndexBuffer::Create(const Vector<T>& indices, DXGI_FORMAT format) noexcept
{
	auto bytesSize = sizeof(T) * indices.size();

	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(bytesSize);

	auto ret = D3D12Buffer::Create(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc);
	if (!ret) {
		return false;
	}

	// ビュー生成
	m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
	m_bufferView.Format = format;
	m_bufferView.SizeInBytes = bytesSize;

	Update(indices);

	return true;
}