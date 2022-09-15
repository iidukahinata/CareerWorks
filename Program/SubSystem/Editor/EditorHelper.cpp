/**
* @file	   EditerHelper.cpp
* @brief
*
* @date	   2022/09/12 2022îNìxèâî≈
*/


#include "EditorHelper.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12GrahicsDevice.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12DescriptorHeap.h"

void EditorHelper::BegineRenderer() noexcept
{
	const auto numHeap = m_descriptorHeaps.size();
	Vector<ID3D12DescriptorHeap*> heap(numHeap);

	for (size_t i = 0; i < numHeap; ++i)
	{
		heap[i] = m_descriptorHeaps[i]->GetHeap();
	}

	D3D12GrahicsDevice::Get().GetCommandContext().SetDescriptorHeaps(numHeap, heap.data());
}

void EditorHelper::Reset() noexcept
{
	m_descriptorHeaps.clear();
}

void EditorHelper::AddTexture(D3D12DescriptorHeap* descriptorHeap) noexcept
{
	m_descriptorHeaps.emplace_back(descriptorHeap);
}

String ConvertToJapanese(StringView str) noexcept
{
	auto&& u8str = ToU8String(str);
	return String(reinterpret_cast<const char*>(u8str.c_str()));
}