/**
* @file    D3D12DeviceChild.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12Helper.h"

enum ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	HullShader,
	DomainShader,
	ComputeShader,

	NumGrapihcsType = 5,
	NumAllType = 6,
};

/**
* D3D12 各クラスの抽象クラス
* GraphicsDevive class のアクセス方法に変更があった場合でも、
* 派生先に影響がないようにこのクラスよりアクセスさせる。
* GraphicsDevive class に変更があった場合は遅延セットアップ関数を作成して対処するものと思われる。
*/
class D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	virtual ~D3D12DeviceChild() {}

protected:

	class ID3D12Device* GetDevice() const noexcept;
	class D3D12CommandContext* GetCommandContext() const noexcept;

	class D3D12DescriptorAllocator& GetViewDescriptorAllocator() noexcept;
	class D3D12DescriptorAllocator& GetSamplerDescriptorAllocator() noexcept;
	class D3D12DescriptorAllocator& GetRtvDescriptorAllocator() noexcept;
	class D3D12DescriptorAllocator& GetDsvDescriptorAllocator() noexcept;
	D3D12Map<D3D12_SAMPLER_DESC, class D3D12Sampler>& GetSamplerMap() noexcept;
};