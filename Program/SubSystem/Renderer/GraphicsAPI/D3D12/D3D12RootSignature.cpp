/**
* @file    D3D12RootSignature.cpp
* @brief
*
* @date	   2022/08/30 2022年度初版
*/


#include "D3D12RootSignature.h"

bool D3D12RootSignature::Create(ID3DBlob* blob) noexcept
{
	Microsoft::WRL::ComPtr<ID3DBlob> rootSignature;
	auto hr = D3DGetBlobPart(blob->GetBufferPointer(), blob->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, rootSignature.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("ShaderDataのシリアル化出来ませんでした。");
		return false;
	}

	// ルートシグネチャの生成
	hr = GetDevice()->CreateRootSignature(0, rootSignature->GetBufferPointer(), rootSignature->GetBufferSize(), IID_PPV_ARGS(m_rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) {
		LOG_ERROR("RootSignature 生成に失敗しました。");
		return false;
	}

	return true;
}

bool D3D12RootSignature::Create(
	UINT parameterCount,
	CD3DX12_ROOT_PARAMETER1* rootParameters,
	UINT sampleCount,
	D3D12_STATIC_SAMPLER_DESC* samplerDesc,
	D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags
) noexcept
{
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1(parameterCount, rootParameters, sampleCount, samplerDesc, rootSignatureFlags);
	return Create(rootSignatureDesc);
}

bool D3D12RootSignature::Create(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& rootSignatureDesc) noexcept
{
	// シリアル化
	Microsoft::WRL::ComPtr<ID3DBlob> rootSignature;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	auto hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, rootSignature.GetAddressOf(), errorBlob.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("SerializeVersionedRootSignature に失敗しました。");
		return false;
	}

	// ルートシグネチャ生成
	hr = GetDevice()->CreateRootSignature(
		0, rootSignature->GetBufferPointer(), rootSignature->GetBufferSize(),
		IID_PPV_ARGS(m_rootsignature.ReleaseAndGetAddressOf()));

	if (FAILED(hr)) {
		LOG_ERROR("RootSignature 生成に失敗しました。");
		return false;
	}

	auto parameterCount = rootSignatureDesc.Desc_1_1.NumParameters;
	auto rootParameters = rootSignatureDesc.Desc_1_1.pParameters;
	InitBindSlotIndices(parameterCount, rootParameters);

	return true;
}

UINT8 D3D12RootSignature::GetCBVRootPramIndex(ShaderType type) const noexcept
{
	switch (type)
	{
	case VertexShader  : return m_bindSlotIndices[VS_CBV];
	case PixelShader   : return m_bindSlotIndices[PS_CBV];
	case GeometryShader: return m_bindSlotIndices[GS_CBV];
	case HullShader	   : return m_bindSlotIndices[HS_CBV];
	case DomainShader  : return m_bindSlotIndices[DS_CBV];
	default			   : return m_bindSlotIndices[ALL_CBV];
	}
}

UINT8 D3D12RootSignature::GetSRVRootPramIndex(ShaderType type) const noexcept
{
	switch (type)
	{
	case VertexShader  : return m_bindSlotIndices[VS_SRV];
	case PixelShader   : return m_bindSlotIndices[PS_SRV];
	case GeometryShader: return m_bindSlotIndices[GS_SRV];
	case HullShader    : return m_bindSlotIndices[HS_SRV];
	case DomainShader  : return m_bindSlotIndices[DS_SRV];
	default			   : return m_bindSlotIndices[ALL_SRV];
	}
}

UINT8 D3D12RootSignature::GetUAVRootPramIndex(ShaderType type) const noexcept
{
	switch (type)
	{
	case VertexShader  : return m_bindSlotIndices[VS_UAV];
	case PixelShader   : return m_bindSlotIndices[PS_UAV];
	case GeometryShader: return m_bindSlotIndices[GS_UAV];
	case HullShader    : return m_bindSlotIndices[HS_UAV];
	case DomainShader  : return m_bindSlotIndices[DS_UAV];
	default            : return m_bindSlotIndices[ALL_UAV];
	}
}

UINT8 D3D12RootSignature::GetSamplerRootPramIndex(ShaderType type) const noexcept
{
	switch (type)
	{
	case VertexShader  : return m_bindSlotIndices[VS_Sampler];
	case PixelShader   : return m_bindSlotIndices[PS_Sampler];
	case GeometryShader: return m_bindSlotIndices[GS_Sampler];
	case HullShader    : return m_bindSlotIndices[HS_Sampler];
	case DomainShader  : return m_bindSlotIndices[DS_Sampler];
	default			   : return m_bindSlotIndices[ALL_Sampler];
	}
}

const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& D3D12RootSignature::GetGrapihcsRootDesc() noexcept
{
	struct InitRootData
	{
		D3D12_SHADER_VISIBILITY visibility;
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType;
		uint32_t numDescriptors;
		uint32_t baseShaderRegister;
		D3D12_DESCRIPTOR_RANGE_FLAGS flags;
	};

	constexpr UINT numDescriptorTable = 9U;
	static InitRootData initData[numDescriptorTable] = {

		{ D3D12_SHADER_VISIBILITY_VERTEX, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, MAX_CONSTANT_BUFFER_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_VERTEX, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_SHADER_RESOURCE_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_VERTEX, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, MAX_SAMPLER, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },

		{ D3D12_SHADER_VISIBILITY_GEOMETRY, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, MAX_CONSTANT_BUFFER_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_GEOMETRY, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_SHADER_RESOURCE_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_GEOMETRY, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, MAX_SAMPLER, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },

		{ D3D12_SHADER_VISIBILITY_PIXEL, D3D12_DESCRIPTOR_RANGE_TYPE_CBV, MAX_CONSTANT_BUFFER_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_PIXEL, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_SHADER_RESOURCE_VIEW, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
		{ D3D12_SHADER_VISIBILITY_PIXEL, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, MAX_SAMPLER, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC },
	};

	static Array<CD3DX12_DESCRIPTOR_RANGE1, numDescriptorTable> descTblRanges = {};
	static Array<CD3DX12_ROOT_PARAMETER1, numDescriptorTable> rootParams = {};

	for (int i = 0; i < numDescriptorTable; ++i)
	{
		descTblRanges[i].Init(initData[i].rangeType, initData[i].numDescriptors, initData[i].baseShaderRegister, 0U, initData[i].flags);
		rootParams[i].InitAsDescriptorTable(1, &descTblRanges[i], initData[i].visibility);
	}

	auto flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS;

	static CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc(numDescriptorTable, rootParams.data(), 0, nullptr, flags);
	return rootSignatureDesc;
}

const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& D3D12RootSignature::GetComputeRootDesc() noexcept
{
	constexpr UINT numDescriptorTable = 3U;
	static Array<CD3DX12_DESCRIPTOR_RANGE1, numDescriptorTable> descTblRanges = {};
	static Array<CD3DX12_ROOT_PARAMETER1, numDescriptorTable> rootParams = {};

	//===定数バッファ用設定=====================================
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, MAX_CONSTANT_BUFFER_VIEW, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0], D3D12_SHADER_VISIBILITY_ALL);
	//==========================================================

	//===テクスチャ用設定=======================================
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_SHADER_RESOURCE_VIEW, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1], D3D12_SHADER_VISIBILITY_ALL);
	//==========================================================

	//===Sampler用設定==========================================
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, MAX_SAMPLER, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_ALL);
	//==========================================================

	//===UAV用設定==============================================
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, MAX_UNORDERED_ACCESS_VIEW, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_ALL);
	//==========================================================

	static CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc(numDescriptorTable, rootParams.data(), 0, nullptr);
	return rootSignatureDesc;
}

void D3D12RootSignature::SetCBVRootPramIndex(ShaderType type, UINT8 index) noexcept
{
	switch (type)
	{
	case VertexShader  : m_bindSlotIndices[VS_CBV] = index; break;
	case PixelShader   : m_bindSlotIndices[PS_CBV] = index; break;
	case GeometryShader: m_bindSlotIndices[GS_CBV] = index; break;
	case HullShader    : m_bindSlotIndices[HS_CBV] = index; break;
	case DomainShader  : m_bindSlotIndices[DS_CBV] = index; break;
	default			   : m_bindSlotIndices[ALL_CBV] = index; break;
	}
}

void D3D12RootSignature::SetUAVRootPramIndex(ShaderType type, UINT8 index) noexcept
{
	switch (type)
	{
	case VertexShader  : m_bindSlotIndices[VS_UAV] = index; break;
	case PixelShader   : m_bindSlotIndices[PS_UAV] = index; break;
	case GeometryShader: m_bindSlotIndices[GS_UAV] = index; break;
	case HullShader    : m_bindSlotIndices[HS_UAV] = index; break;
	case DomainShader  : m_bindSlotIndices[DS_UAV] = index; break;
	default			   : m_bindSlotIndices[ALL_UAV] = index; break;
	}
}

void D3D12RootSignature::SetSRVRootPramIndex(ShaderType type, UINT8 index) noexcept
{
	switch (type)
	{
	case VertexShader  : m_bindSlotIndices[VS_SRV] = index; break;
	case PixelShader   : m_bindSlotIndices[PS_SRV] = index; break;
	case GeometryShader: m_bindSlotIndices[GS_SRV] = index; break;
	case HullShader    : m_bindSlotIndices[HS_SRV] = index; break;
	case DomainShader  : m_bindSlotIndices[DS_SRV] = index; break;
	default			   : m_bindSlotIndices[ALL_SRV] = index; break;
	}
}

void D3D12RootSignature::SetSamplerRootPramIndex(ShaderType type, UINT8 index) noexcept
{
	switch (type)
	{
	case VertexShader  : m_bindSlotIndices[VS_Sampler] = index;
	case PixelShader   : m_bindSlotIndices[PS_Sampler] = index;
	case GeometryShader: m_bindSlotIndices[GS_Sampler] = index;
	case HullShader    : m_bindSlotIndices[HS_Sampler] = index;
	case DomainShader  : m_bindSlotIndices[DS_Sampler] = index;
	default			   : m_bindSlotIndices[ALL_Sampler] = index;
	}
}

void D3D12RootSignature::InitBindSlotIndices(UINT parameterCount, const D3D12_ROOT_PARAMETER1* rootParameters) noexcept
{
	for (int i = 0; i < parameterCount; ++i)
	{
		auto param = rootParameters[i];

		// パラメーターが使用されるShaderStageを調べ独自データに変換する
		auto currentShaderType = ShaderType::NumAllType;
		switch (param.ShaderVisibility)
		{
		case D3D12_SHADER_VISIBILITY_VERTEX:
			currentShaderType = VertexShader;
			break;

		case D3D12_SHADER_VISIBILITY_HULL:
			currentShaderType = HullShader;
			break;

		case D3D12_SHADER_VISIBILITY_DOMAIN:
			currentShaderType = DomainShader;
			break;

		case D3D12_SHADER_VISIBILITY_GEOMETRY:
			currentShaderType = GeometryShader;
			break;

		case D3D12_SHADER_VISIBILITY_PIXEL:
			currentShaderType = PixelShader;
			break;

		case D3D12_SHADER_VISIBILITY_AMPLIFICATION:
		case D3D12_SHADER_VISIBILITY_MESH:
		case D3D12_SHADER_VISIBILITY_ALL:
			currentShaderType = NumAllType;
			break;

		default: break;
		}
		
		// パラメーターインデックスに登録
		auto rangeType = param.DescriptorTable.pDescriptorRanges->RangeType;
		switch (rangeType)
		{
		case D3D12_DESCRIPTOR_RANGE_TYPE_SRV:
			SetSRVRootPramIndex(currentShaderType, i);
			break;

		case D3D12_DESCRIPTOR_RANGE_TYPE_UAV:
			SetUAVRootPramIndex(currentShaderType, i);
			break;

		case D3D12_DESCRIPTOR_RANGE_TYPE_CBV:
			SetCBVRootPramIndex(currentShaderType, i);
			break;

		case D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER:
			SetSamplerRootPramIndex(currentShaderType, i);
			break;

		default: break;
		}
	}
}