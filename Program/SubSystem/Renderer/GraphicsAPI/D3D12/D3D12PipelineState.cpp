/**
* @file    D3D12PipelineState.cpp
* @brief
*
* @date	   2022/08/30 2022”N“x‰”Å
*/


#include "D3D12PipelineState.h"
#include "D3D12Shader.h"
#include "D3D12GrahicsDevice.h"

bool D3D12GraphicsPipelineState::Create(const GraphicsPipelineStateDesc& desc, D3D12RootSignature* rootSignature) noexcept
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};
	gpipeline.pRootSignature = rootSignature->Get();

	Vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;

	// shader
	{
		if (desc.VS)
		{
			gpipeline.VS = CD3DX12_SHADER_BYTECODE(desc.VS->GetBlob());

			inputLayout = desc.VS->GetInputLayout();
			gpipeline.InputLayout.pInputElementDescs = inputLayout.data();
			gpipeline.InputLayout.NumElements = inputLayout.size();
		}
		if (desc.PS)
		{
			gpipeline.PS = CD3DX12_SHADER_BYTECODE(desc.PS->GetBlob());
		}
		if (desc.GS)
		{
			gpipeline.GS = CD3DX12_SHADER_BYTECODE(desc.GS->GetBlob());
		}
	}

	// primitive type
	{
		const auto PrimitiveType = desc.PrimitiveType;
		if (PrimitiveType == PRIMITIVE_TYPE_TRIANGLELIST || PrimitiveType == PRIMITIVE_TYPE_TRIANGLESTRIP)
		{
			gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		}
		else if (PrimitiveType == PRIMITIVE_TYPE_LINELIST || PrimitiveType == PRIMITIVE_TYPE_LINESTRIP)
		{
			gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		}
		else if (PrimitiveType == PRIMITIVE_TYPE_POINTLIST)
		{
			gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		}
	}

	// blend state
	{
		gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		gpipeline.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		switch (desc.BlendMode)
		{
		case BLEND_MODE_NO_ALPHA:
			blendDesc.BlendEnable = false;
			break;

		case BLEND_MODE_ALPHA:
			blendDesc.BlendEnable = true;
			blendDesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			break;

		case BLEND_MODE_ADD:
			blendDesc.BlendEnable = true;
			blendDesc.SrcBlend = D3D12_BLEND_ONE;
			blendDesc.DestBlendAlpha = D3D12_BLEND_ONE;
			break;

		case BLEND_MODE_ADD_ALPHA:
			blendDesc.BlendEnable = true;
			blendDesc.DestBlend = D3D12_BLEND_ONE;
			break;

		case BLEND_MODE_SUB:
			blendDesc.BlendEnable = true;
			blendDesc.SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_COLOR;
			break;

		case BLEND_MODE_MUL:
			blendDesc.BlendEnable = true;
			blendDesc.SrcBlend = D3D12_BLEND_ZERO;
			blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;
			break;

		case BLEND_MODE_SCREEN:
			blendDesc.BlendEnable = true;
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
			blendDesc.DestBlend = D3D12_BLEND_ONE;
			break;

		default: break;
		}

		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
			gpipeline.BlendState.RenderTarget[i] = blendDesc;
		}
	}
	
	// rasterrizer state
	{
		gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		switch (desc.RasterizerState)
		{
		case NO_CULL:
			gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			break;

		case CULL_CLOCK_WISE:
			gpipeline.RasterizerState.FrontCounterClockwise = TRUE;
			break;

		case WIRE_FRAME_CULL:
			gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
			gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
			break;

		case DEPTH_TILT_BIAS:
			gpipeline.RasterizerState.DepthBias = 100000;
			gpipeline.RasterizerState.SlopeScaledDepthBias = 1.0f;
			break;

		default: break;
		}
	}
	
	// depth stencil state
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	gpipeline.DepthStencilState.StencilEnable = false;

	// render target
	gpipeline.NumRenderTargets = desc.NumRenderTargets;
	for (UINT i = 0; i < 8; i++) {
		gpipeline.RTVFormats[0] = desc.RTVForamt[0];
	}

	// sample
	gpipeline.SampleDesc = desc.SampleDesc;

	return Create(gpipeline, rootSignature);
}

bool D3D12GraphicsPipelineState::Create(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, D3D12RootSignature* rootSignature) noexcept
{
	auto hr = GetDevice()->CreateGraphicsPipelineState(
		&desc,
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		LOG_ERROR("pipelineState ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_rootSignature = rootSignature;

	return true;
}

void D3D12GraphicsPipelineState::Set() noexcept
{
	GetCommandContext()->SetGraphicsPipelineState(this);
}

bool D3D12ComputePipelineState::Create(const ComputePipelineStateDesc& desc, D3D12RootSignature* rootSignature) noexcept
{
	D3D12_COMPUTE_PIPELINE_STATE_DESC cpipeline = {};
	cpipeline.pRootSignature = rootSignature->Get();

	// shader
	{
		if (desc.CS)
		{
			cpipeline.CS = CD3DX12_SHADER_BYTECODE(desc.CS->GetBlob());
		}
	}

	return Create(cpipeline, rootSignature);
}

bool D3D12ComputePipelineState::Create(const D3D12_COMPUTE_PIPELINE_STATE_DESC& desc, D3D12RootSignature* rootSignature) noexcept
{
	auto hr = GetDevice()->CreateComputePipelineState(
		&desc,
		IID_PPV_ARGS(m_pipelineState.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		LOG_ERROR("pipelineState ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_rootSignature = rootSignature;

	return true;
}

void D3D12ComputePipelineState::Set() noexcept
{
	GetCommandContext()->SetComputePipelineState(this);
}