/**
* @file    D3D12Buffer.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"

class D3D12Buffer : public D3D12DeviceChild
{
    COMPLETED_DEVELOPMENT()
public:

    void Update(UINT destSize, void* srcPtr, UINT srcSize) noexcept;

    template<class T>
    void Update(const T& buffer) noexcept;

    template<class T>
    void Update(const Vector<T>& dara) noexcept;

    /** アクセス */
    ID3D12Resource*     Get()          const noexcept { return m_buffer.Get(); }
    ID3D12Resource**    GetAddressOf()       noexcept { return m_buffer.GetAddressOf(); }
    D3D12_RESOURCE_DESC GetDesc()      const noexcept { return m_buffer->GetDesc(); }
    HANDLE              GetCPUData()   const noexcept { return m_cpuData; }

protected:

    bool Create(
        const D3D12_HEAP_PROPERTIES* pHeapProperties,
        D3D12_HEAP_FLAGS HeapFlags,
        const D3D12_RESOURCE_DESC* pDesc,
        D3D12_RESOURCE_STATES InitialResourceState = D3D12_RESOURCE_STATE_GENERIC_READ,
        const D3D12_CLEAR_VALUE* pOptimizedClearValue = nullptr) noexcept;

protected:

    Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;

    // * CPU上メモリのバッファ先頭アドレス
    HANDLE m_cpuData = nullptr;

    UINT m_bytesSize;
};

template<class T>
FORCEINLINE void D3D12Buffer::Update(const T& buffer) noexcept
{
    memcpy_s(m_cpuData, m_bytesSize, &buffer, sizeof(buffer));
}

template<class T>
FORCEINLINE void D3D12Buffer::Update(const Vector<T>& buffer) noexcept
{
    memcpy_s(m_cpuData, m_bytesSize, const_cast<T*>(buffer.data()), sizeof(T) * buffer.size());
}