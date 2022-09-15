/**
* @file	   EditerHelper.h
* @brief
*
* @date	   2022/09/10 2022îNìxèâî≈
*/
#pragma once


class D3D12DescriptorHeap;

class EditorHelper
{
public:

	static EditorHelper& Get()
	{
		static EditorHelper instance;
		return instance;
	}

	void BegineRenderer() noexcept;

	void Reset() noexcept;

	void AddTexture(D3D12DescriptorHeap* descriptorHeap) noexcept;

private:

	Vector<D3D12DescriptorHeap*> m_descriptorHeaps;
};

String ConvertToJapanese(StringView str) noexcept;