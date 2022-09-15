/**
* @file    D3D12PipelineState.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"
#include "D3D12RootSignature.h"

class D3D12Shader;

enum BLEND_MODE
{
	BLEND_MODE_NO_ALPHA = 0,
	BLEND_MODE_ALPHA = 1,
	BLEND_MODE_ADD = 2,
	BLEND_MODE_ADD_ALPHA = 3,
	BLEND_MODE_SUB = 4,
	BLEND_MODE_MUL = 5,
	BLEND_MODE_SCREEN = 6,
};

enum PRIMITIVE_TYPE
{
    PRIMITIVE_TYPE_UNDEFINED = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
    PRIMITIVE_TYPE_POINTLIST = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
    PRIMITIVE_TYPE_LINELIST = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
    PRIMITIVE_TYPE_LINESTRIP = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
    PRIMITIVE_TYPE_TRIANGLELIST = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    PRIMITIVE_TYPE_TRIANGLESTRIP = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
};

enum RASTERIZER_STATE
{
	NO_CULL,
	CULL_CLOCK_WISE,
	WIRE_FRAME_CULL,
	DEPTH_TILT_BIAS,
};

struct GraphicsPipelineStateDesc
{
	D3D12Shader* VS = nullptr;
	D3D12Shader* PS = nullptr;
	D3D12Shader* GS = nullptr;
	RASTERIZER_STATE RasterizerState = NO_CULL;
	BLEND_MODE BlendMode = BLEND_MODE_NO_ALPHA;
	PRIMITIVE_TYPE PrimitiveType = PRIMITIVE_TYPE_UNDEFINED;
	DXGI_FORMAT RTVForamt[MAX_RENDER_TARGET] = { DXGI_FORMAT_R8G8B8A8_UNORM };
	UINT NumRenderTargets = 0;
	DXGI_SAMPLE_DESC SampleDesc = { 1, 0 };
};

struct ComputePipelineStateDesc
{
	D3D12Shader* CS = nullptr;
};

class D3D12GraphicsPipelineState : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(const GraphicsPipelineStateDesc& desc, D3D12RootSignature* rootSignature) noexcept;
	bool Create(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, D3D12RootSignature* rootSignature) noexcept;

	void Set() noexcept;

	/** アクセス */
	ID3D12PipelineState* Get()				const noexcept { return m_pipelineState.Get(); }
	D3D12RootSignature*  GetRootSignature() const noexcept { return m_rootSignature; }
	const GraphicsPipelineStateDesc& GetDesc() const noexcept { return m_desc; }

private:

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

	D3D12RootSignature* m_rootSignature = nullptr;

	GraphicsPipelineStateDesc m_desc;
};

class D3D12ComputePipelineState : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(const ComputePipelineStateDesc& desc, D3D12RootSignature* rootSignature) noexcept;
	bool Create(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, D3D12RootSignature* rootSignature) noexcept;

	void Set() noexcept;

	/** アクセス */
	ID3D12PipelineState* Get() const noexcept { return m_pipelineState.Get(); }
	D3D12RootSignature*  GetRootSignature() const noexcept { return m_rootSignature; }

private:

	Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;

	D3D12RootSignature* m_rootSignature = nullptr;

	D3D12_COMPUTE_PIPELINE_STATE_DESC m_desc;
};