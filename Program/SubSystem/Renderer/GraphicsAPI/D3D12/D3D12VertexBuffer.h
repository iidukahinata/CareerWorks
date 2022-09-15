/**
* @file    D3D12VertexBuffer.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12Buffer.h"

class D3D12VertexBuffer : public D3D12Buffer
{
	COMPLETED_DEVELOPMENT()
public:

	template<class T>
	bool Create(const Vector<T>& vertices) noexcept;
	bool Create(UINT size, UINT stride) noexcept;

	void IASet(UINT slot = 0) noexcept;

	/** アクセス */
	ID3D12Resource*			  Get()		const noexcept { return m_buffer.Get(); }
	D3D12_VERTEX_BUFFER_VIEW& GetBufferView() noexcept { return m_bufferView; }

private:

	D3D12_VERTEX_BUFFER_VIEW m_bufferView;
};

template<class T>
FORCEINLINE bool D3D12VertexBuffer::Create(const Vector<T>& vertices) noexcept
{
	const UINT stride = sizeof(T);
	const UINT size = sizeof(T) * vertices.size();

	if (!Create(size, stride)) {
		return false;
	}

	Update(vertices);

	return true;
}