/**
* @file    Material.cpp
* @brief
*
* @date	   2022/11/01 2022年度初版
*/


#include "Material.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/ResourceData/ProprietaryShaderData.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

Material* Material::Create(StringView name, const ProprietaryMaterialData& data /* = ProprietaryMaterialData() */) noexcept
{
	// データ保存先パスを生成
	auto path = ProprietaryMaterialData::ConvertProprietaryPath(name);

	// 各データの作成
	if (const auto material = CreateResource<Material>(path))
	{
		material->m_materialData = data;
		material->Update();
		return material;
	}

	return nullptr;
}

bool Material::Load(StringView path)
{
	if (!m_materialData.LoadFromFile(path))
	{
		return false;
	}

	// Set Texture Settings
	for (auto& texturePath : m_materialData.m_texturePaths)
	{
		auto resourceData = m_resourceManager->GetResourceData(texturePath.second);
		auto texture = m_resourceManager->GetResource(resourceData);

		m_textureInfos[texturePath.first].m_texture = dynamic_cast<Texture*>(texture);
	}

	if (!CreateConstantBuffer())
	{
		return false;
	}
	if (!CreateRootSinature())
	{
		return false;
	}
	if (!CompileShader())
	{
		return false;
	}

	return true;
}

void Material::Update()
{
	UpdateProprietaryDataFile();

	UpdateResourceDataFile();
}

void Material::Render() noexcept
{
	// Set PipelineState
	m_pipeline.Set();

	// Set ConstantBuffer
	m_constantBufferMaterial.VSSet(2);

	// Set Sampler
	for (auto& sampler : m_samplers)
	{
		sampler.second.PSSet(sampler.first);
	}

	// Set Texture
	for (auto& textureInfo : m_textureInfos)
	{
		auto texture = textureInfo.second.m_texture;
		auto bindPoint = textureInfo.second.m_bindPoint;

		if (texture)
		{
			texture->PSSet(bindPoint);
		}
	}
}

void Material::SetBlendMode(BLEND_MODE mode) noexcept
{
	m_materialData.m_blendMode = mode;

	CreatePipeline();
}

BLEND_MODE Material::GetBlendMode() const noexcept
{
	return m_materialData.m_blendMode;
}

bool Material::IsTranslucent() const noexcept
{
	return m_materialData.m_blendMode != BLEND_MODE_NO_ALPHA;
}

void Material::SetRasterizerState(RASTERIZER_STATE rasterizerState) noexcept
{
	m_materialData.m_rasterizerState = rasterizerState;

	CreatePipeline();
}

RASTERIZER_STATE Material::GetRasterizerState() const noexcept
{
	return m_materialData.m_rasterizerState;
}

bool Material::SetShader(ShaderType type, StringView path, bool createPipeline /* = true */) noexcept
{
	const auto& shaderPaths = m_shader.GetShaderPaths();
	if (shaderPaths[type] == path)
	{
		return true;
	}

	// テクスチャ設定の Define を定義したコンパイルを行うため
	if (type == PixelShader)
	{
		// 初期ロード時かを PixelShader の有無で判断している
		const auto hasPixelShader = !!m_shader.GetShader(PixelShader);
		if (hasPixelShader)
		{
			m_textureInfos.clear();
		}

		if (!DefineSettingToPixelShader(path))
		{
			return false;
		}

		ParametricAnalysis(hasPixelShader);
	}
	else
	{
		if (!m_shader.SetShader(type, path))
		{
			return false;
		}
	}

	// シェーダー変更の適用
	if (createPipeline)
	{
		return CreatePipeline();
	}

	return true;
}

const Array<String, ShaderType::NumAllType>& Material::GetShaderPaths() const noexcept
{
	return m_shader.GetShaderPaths();
}

void Material::SetTexture(StringView pramName, Texture* texture, bool isDefineToShader /* = false */) noexcept
{
	String pram(pramName);

	if (m_textureInfos.contains(pram))
	{
		m_textureInfos[pram].m_texture = texture;

		// テクスチャ設定の Define を更新し Pipeline に適用させるため
		if (isDefineToShader)
		{
			DefineSettingToPixelShader();

			CreatePipeline();
		}
	}
	else
	{
		LOG_ERROR("指定されたテクスチャパラメーターは存在しませんでした。\n ファイル名 : " + pram);
	}
}

const Unordered_Map<String, TexturePramInfo>& Material::GetTextures() const noexcept
{
	return m_textureInfos;
}

void Material::SetAlbedo(const Math::Vector3& albedo) noexcept
{
	m_materialData.m_albedo = albedo;

	UpdateConstantBufferData();
}

const Math::Vector3& Material::GetAlbedo() const noexcept
{
	return m_materialData.m_albedo;
}

void Material::SetMetallic(float metallic) noexcept
{
	m_materialData.m_metallic = metallic;

	UpdateConstantBufferData();
}

float Material::GetMetallic() const noexcept
{
	return m_materialData.m_metallic;
}

void Material::SetSmooth(float smooth) noexcept
{
	m_materialData.m_smooth = smooth;

	UpdateConstantBufferData();
}

float Material::GetSmooth() noexcept
{
	return m_materialData.m_smooth;
}

void Material::SetEmission(const Math::Vector3& emissive) noexcept
{
	m_materialData.m_emission = emissive;

	UpdateConstantBufferData();
}

const Math::Vector3 Material::GetEmission() const noexcept
{
	return m_materialData.m_emission;
}

bool Material::DefineSettingToPixelShader(StringView path /* = StringView() */) noexcept
{
	if (path.empty())
	{
		path = m_materialData.m_shaderPaths[PixelShader];

		if (path.empty())
		{
			return false;
		}
	}

	// 変数の一時的な保存場所として使用
	Map<String, String> macros;

	// テクスチャ用のマクロ定義を作成
	for (auto& textureInfo : m_textureInfos)
	{
		auto texture = textureInfo.second.m_texture;
		auto bindPoint = textureInfo.second.m_bindPoint;
	
		auto&& name = "HAS_SRV_" + std::to_string(bindPoint);
		macros.emplace(std::move(name), texture ? "1" : "0");
	}
	
	Vector<D3D_SHADER_MACRO> defines;

	constexpr auto NumNullObject = 1;
	defines.reserve(macros.size() + NumNullObject);

	for (auto& macro : macros)
	{
		defines.push_back(D3D_SHADER_MACRO(macro.first.c_str(), macro.second.c_str()));
	}

	// 終端定義
	defines.push_back(D3D_SHADER_MACRO(NULL, NULL));

	return m_shader.SetShader(PixelShader, path, defines.data());
}

void Material::ParametricAnalysis(bool isClear /* = true */) noexcept
{
	if (isClear)
	{
		m_textureInfos.clear();
	}

	if (const auto psShader = m_shader.GetShader(PixelShader))
	{
		auto srvPrams = psShader->GetSRVBindDesc();
		for (const auto& pram : srvPrams)
		{
			auto& info = m_textureInfos[pram.Name];
			info.m_bindPoint = pram.BindPoint;
		}

		auto samplerPrams = psShader->GetSamplerBindDesc();
		for (const auto& pram : samplerPrams)
		{
			auto& sampler = m_samplers[pram.BindPoint];
			sampler.Create(D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
		}
	}
}

bool Material::CreateRootSinature() noexcept
{
	Array<CD3DX12_DESCRIPTOR_RANGE1, 3> descTblRanges = {};
	Array<CD3DX12_ROOT_PARAMETER1, 3> rootParams = {};

	//===定数バッファ用設定=====================================
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
	//==========================================================

	//===テクスチャ用設定=======================================
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 16, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	//===Sampler用設定==========================================
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	return m_rootSignature.Create(rootParams.size(), rootParams.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

bool Material::CreatePipeline() noexcept
{
	GraphicsPipelineStateDesc desc;
	desc.VS				 = m_shader.GetShader(VertexShader);
	desc.PS				 = m_shader.GetShader(PixelShader);
	desc.GS				 = m_shader.GetShader(GeometryShader);
	desc.BlendMode		 = m_materialData.m_blendMode;
	desc.RasterizerState = m_materialData.m_rasterizerState;
	desc.PrimitiveType	 = PRIMITIVE_TYPE_TRIANGLELIST;
	//desc.SampleDesc.Count = 4;

	if (desc.PS)
	{
		// 色んなシェーダーに対応出来るようにするためシェーダーデータから設定を取得
		auto renderTargetsDesc = desc.PS->GetRenderTargetsDesc();

		desc.NumRenderTargets = renderTargetsDesc.size();
		for (int i = 0; i < desc.NumRenderTargets; ++i)
		{
			desc.RTVForamt[i] = renderTargetsDesc[i];
		}
	}
	else
	{
		desc.NumRenderTargets = 1;
	}

#ifdef IS_EDITOR
	RegisterRenderCommand([this, desc] {
		ASSERT(m_pipeline.Create(desc, &m_rootSignature));
	});
#else
	if (IsRenderingThread())
	{
		ASSERT(m_pipeline.Create(desc, &m_rootSignature));
	}
	else
	{
		RegisterRenderCommand([this, desc] {
			ASSERT(m_pipeline.Create(desc, &m_rootSignature));
		});
	}
#endif // IS_EDITOR

	return true;
}

bool Material::CreateConstantBuffer() noexcept
{
	if (m_constantBufferMaterial.Create(sizeof(ConstantBufferMaterial)))
	{
		auto buffer = static_cast<ConstantBufferMaterial*>(m_constantBufferMaterial.GetCPUData());
		buffer->albedo	 = m_materialData.m_albedo;
		buffer->metallic = m_materialData.m_metallic;
		buffer->smooth	 = m_materialData.m_smooth;
		buffer->emission = m_materialData.m_emission;

		return true;
	}

	return false;
}

bool Material::CompileShader() noexcept
{
	bool result = true;
	for (int i = 0; i < ShaderType::NumAllType; ++i)
	{
		auto& name = m_materialData.m_shaderPaths[i];
		if (name.empty())
			continue;

		if (!SetShader(static_cast<ShaderType>(i), name, false))
		{
			result = false;
			break;
		}
	}

	if (!result)
	{
		return false;
	}

	if (!CreatePipeline())
	{
		return false;
	}

	return true;
}

void Material::UpdateConstantBufferData() noexcept
{
	if (IsRenderingThread())
	{
		auto buffer = static_cast<ConstantBufferMaterial*>(m_constantBufferMaterial.GetCPUData());

		buffer->albedo = m_materialData.m_albedo;
		buffer->metallic = m_materialData.m_metallic;
		buffer->smooth = m_materialData.m_smooth;
		buffer->emission = m_materialData.m_emission;
	}
	else
	{
		RegisterRenderCommand([this] {

			auto buffer = static_cast<ConstantBufferMaterial*>(m_constantBufferMaterial.GetCPUData());

			buffer->albedo = m_materialData.m_albedo;
			buffer->metallic = m_materialData.m_metallic;
			buffer->smooth = m_materialData.m_smooth;
			buffer->emission = m_materialData.m_emission;

		});
	}
}

void Material::UpdateProprietaryDataFile() noexcept
{
	auto& texturePaths = m_materialData.m_texturePaths;
	auto& shaderPaths = m_materialData.m_shaderPaths;

	texturePaths.clear();

	// 初期化時は保持していないため
	if (m_shader.HasShader())
	{
		// 現在シェーダ設定を反映
		shaderPaths = m_shader.GetShaderPaths();
	}

	// 現在テクスチャ設定を反映
	for (const auto& textureInfo : m_textureInfos)
	{
		if (const auto texture = textureInfo.second.m_texture)
		{
			texturePaths[textureInfo.first] = texture->GetFilePath();
		}
	}

	m_materialData.SaveToFile(m_filePath);
}

void Material::UpdateResourceDataFile() noexcept
{
	const auto resourceData = GetResourceData();
	auto& refResourcePaths = resourceData->m_refResourcePaths;

	refResourcePaths.clear();
	refResourcePaths.shrink_to_fit();
	
	// 現在テクスチャ設定を反映
	for (const auto& textureInfo : m_textureInfos)
	{
		if (const auto texture = textureInfo.second.m_texture)
		{
			refResourcePaths.emplace_back(ResourcePath(Texture::TypeData.Hash, texture->GetFilePath()));
		}
	}

	m_resourceManager->UpdateResourceData(resourceData);
}