/**
* @file    D3D12DeviceChild.cpp
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/


#include "D3D12DeviceChild.h"
#include "D3D12GrahicsDevice.h"
#include "D3D12Sampler.h"

ID3D12Device* D3D12DeviceChild::GetDevice() const noexcept
{
	return D3D12GrahicsDevice::Get().GetDevice();
}

D3D12CommandContext* D3D12DeviceChild::GetCommandContext() const noexcept
{
	return 	&D3D12GrahicsDevice::Get().GetCommandContext();
}

D3D12DescriptorAllocator& D3D12DeviceChild::GetViewDescriptorAllocator() noexcept
{
	return 	D3D12GrahicsDevice::Get().GetViewDescriptorAllocator();
}

D3D12DescriptorAllocator& D3D12DeviceChild::GetSamplerDescriptorAllocator() noexcept
{
	return 	D3D12GrahicsDevice::Get().GetSamplerDescriptorAllocator();
}

D3D12DescriptorAllocator& D3D12DeviceChild::GetRtvDescriptorAllocator() noexcept
{
	return 	D3D12GrahicsDevice::Get().GetRtvDescriptorAllocator();
}

D3D12DescriptorAllocator& D3D12DeviceChild::GetDsvDescriptorAllocator() noexcept
{
	return 	D3D12GrahicsDevice::Get().GetDsvDescriptorAllocator();
}

D3D12Map<D3D12_SAMPLER_DESC, D3D12Sampler*>& D3D12DeviceChild::GetSamplerMap() noexcept
{
	return 	D3D12GrahicsDevice::Get().GetSamplerMap();
}