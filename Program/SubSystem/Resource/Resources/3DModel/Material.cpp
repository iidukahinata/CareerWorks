/**
* @file    Material.cpp
* @brief
*
* @date	   2022/11/01 2022年度初版
*/


#include "Material.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"
#include "SubSystem/Resource/ResourceData/ProprietaryShaderData.h"

Material* Material::Create(StringView name, const ProprietaryMaterialData& data /* = ProprietaryMaterialData() */) noexcept
{
	// データ保存先パスを生成
	auto&& path = ProprietaryMaterialData::ConvertProprietaryPath(name);

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
	for (auto& textureInfo : m_materialData.m_textureInfos)
	{
		auto resourceData = m_resourceManager->GetResourceData(textureInfo.second.texturePath);
		auto texture = m_resourceManager->GetResource(resourceData);

		m_textureInfos[textureInfo.first].m_texture	  = dynamic_cast<Texture*>(texture);
		m_textureInfos[textureInfo.first].m_bindPoint = textureInfo.second.bindPoint;
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
	m_constantBufferMaterial.PSSet(2);

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

void Material::SetIsInstancing(bool instancing) noexcept
{
	m_materialData.m_isInstancing = instancing;

	DefineSettingToVertexShader();

	CreatePipeline();
}

bool Material::IsInstancing() const noexcept
{
	return m_materialData.m_isInstancing;
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
	if (type == VertexShader)
	{
		DefineSettingToVertexShader(path);
	}
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

void Material::RefreshShader() noexcept
{
	// recompile shaders
	const auto& paths = m_shader.GetShaderPaths();
	for (int i = 0; i < paths.size(); ++i)
	{
		if (paths[i].empty())
		{
			continue;
		}

		if (i == VertexShader)
		{
			DefineSettingToVertexShader(String(), true);
		}
		else if (i == PixelShader)
		{
			DefineSettingToPixelShader(String(), true);
		}
		else
		{
			m_shader.ReCompile(static_cast<ShaderType>(i));
		}
	}

	CreatePipeline();

	// setup textures
	if (const auto psShader = m_shader.GetShader(PixelShader))
	{
		auto&& srvPrams = psShader->GetSRVBindDesc();
		for (const auto& pram : srvPrams)
		{
			auto& info = m_textureInfos[pram.Name];
			info.m_bindPoint = pram.BindPoint;
		}
	}
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

void Material::AddTexturePram(StringView pramName, uint32_t bindPoint) noexcept
{
	String pram(pramName);
	if (m_textureInfos.contains(pram))
	{
		return;
	}

	auto& textureInfo = m_textureInfos[pram];
	textureInfo.m_texture = nullptr;
	textureInfo.m_bindPoint = bindPoint;
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

const Math::Vector3& Material::GetEmission() const noexcept
{
	return m_materialData.m_emission;
}

bool Material::DefineSettingToPixelShader(StringView path /* = StringView() */, bool recompile /* = false */) noexcept
{
	String shaderPath = path;
	if (path.empty())
	{
		shaderPath = m_materialData.m_shaderPaths[PixelShader];

		if (shaderPath.empty())
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

	if (recompile)
	{
		return m_shader.ReCompile(PixelShader, defines.data());
	}
	else
	{
		return m_shader.SetShader(PixelShader, shaderPath, defines.data());
	}
}

bool Material::DefineSettingToVertexShader(StringView path /* = String() */, bool recompile /* = false */) noexcept
{
	String shaderPath = path;
	if (path.empty())
	{
		shaderPath = m_materialData.m_shaderPaths[VertexShader];

		if (shaderPath.empty())
		{
			return false;
		}
	}

	Map<String, String> macros;
	macros.emplace("USE_INSTANCING", m_materialData.m_isInstancing ? "1" : "0");

	Vector<D3D_SHADER_MACRO> defines;

	constexpr auto NumNullObject = 1;
	defines.reserve(macros.size() + NumNullObject);

	for (auto& macro : macros)
	{
		defines.push_back(D3D_SHADER_MACRO(macro.first.c_str(), macro.second.c_str()));
	}

	defines.push_back(D3D_SHADER_MACRO(NULL, NULL));

	if (recompile)
	{
		return m_shader.ReCompile(VertexShader, defines.data());
	}
	else
	{
		return m_shader.SetShader(VertexShader, shaderPath, defines.data());
	}
}

void Material::ParametricAnalysis(bool isClear /* = true */) noexcept
{
	if (isClear)
	{
		m_textureInfos.clear();
	}

	if (const auto psShader = m_shader.GetShader(PixelShader))
	{
		auto&& srvPrams = psShader->GetSRVBindDesc();
		for (const auto& pram : srvPrams)
		{
			bool isContinue = false;
			for (auto& texInfo : m_textureInfos)
			{
				if (isContinue)
					break;

				isContinue = texInfo.second.m_bindPoint == pram.BindPoint;
			}

			if (isContinue) 
				continue;

			auto& info = m_textureInfos[pram.Name];
			info.m_bindPoint = pram.BindPoint;
		}

		auto&& samplerPrams = psShader->GetSamplerBindDesc();
		for (const auto& pram : samplerPrams)
		{
			auto& sampler = m_samplers[pram.BindPoint];
			sampler.Create(D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
		}
	}
}

bool Material::CreateRootSinature() noexcept
{
	return m_rootSignature.Create(m_rootSignature.GetGrapihcsRootDesc());
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
	if (IsInRenderingThread())
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

	// 実際には処理されていないが、処理を外すとインスタンス有効時に正常に動作しなくなるため記述
	DefineSettingToVertexShader();

	if (!CreatePipeline())
	{
		return false;
	}

	return true;
}

void Material::UpdateConstantBufferData() noexcept
{
	if (IsInRenderingThread())
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
	auto& textureInfos = m_materialData.m_textureInfos;
	auto& shaderPaths = m_materialData.m_shaderPaths;

	textureInfos.clear();

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
			textureInfos[textureInfo.first].texturePath = texture->GetFilePath();
			textureInfos[textureInfo.first].bindPoint	= textureInfo.second.m_bindPoint;
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