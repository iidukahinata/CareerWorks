/**
* @file    Material.cpp
* @brief
*
* @date	   2022/09/04 2022�N�x����
*/


#include "Material.h"
#include "SubSystem/Resource/ResourceManager.h"

Material* Material::Create(StringView name, const ProprietaryMaterialData& data /* = ProprietaryMaterialData() */) noexcept
{
	// �f�[�^�ۑ���p�X�𐶐�
	auto path = ProprietaryMaterialData::ConvertProprietaryPath(name);

	// �e�f�[�^�̍쐬
	auto material = CreateResource<Material>(path);
	material->m_materialData = data;

	material->Update();

	return material;
}

bool Material::Load(StringView path)
{
	if (!m_materialData.LoadFromFile(path))
	{
		return false;
	}

	if (!CreateConstantBuffer())
	{
		return false;
	}

	if (!CreateSampler())
	{
		return false;
	}

	if (!CreateRootSinature())
	{
		return false;
	}

	// �e�N�X�`���Z�b�g
	for (auto& texturePath : m_materialData.m_texturePaths)
	{
		auto resourceData = m_resourceManager->GetResourceData(texturePath.second);
		auto texture = m_resourceManager->GetResource(resourceData);

		m_textures[texturePath.first].m_texture = dynamic_cast<Texture*>(texture);
	}

	CompileShader();

	// �O��f�[�^�̃e�N�X�`����ݒ�ς݂̂��� false �w��
	ParametricAnalysis(false);

	if (!CreatePipeline())
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
	m_pipeline.Set();

	m_constantBufferMaterial.VSSet(2);

	for (int i = 0; i < m_samplers.size(); ++i)
	{
		m_samplers[i].PSSet(i);
	}

	for (auto& texture : m_textures)
	{
		auto& textureInfo = texture.second;
		if (textureInfo.m_texture)
		{
			textureInfo.m_texture->PSSet(textureInfo.m_bindPoint);
		}
	}
}

void Material::SetShader(ShaderType type, StringView name, bool createPipeline /* = true */) noexcept
{
	m_shader.SetShader(type, name);

	// �e�N�X�`���̃o�C���h�����X�V����K�v�����邽��
	if (type == PixelShader)
	{
		ParametricAnalysis();
	}

	// �V�F�[�_�[�ύX�̓K�p
	if (createPipeline)
	{
		CreatePipeline();
	}
}

void Material::SetTexture(StringView pramName, Texture* texture, bool isDefineToShader /* = false */) noexcept
{
	String pram(pramName);

	if (m_textures.contains(pram))
	{
		m_textures[pram].m_texture = texture;
	}
	else
	{
		LOG_ERROR("�w�肳�ꂽ�e�N�X�`���p�����[�^�[�͑��݂��܂���ł����B\n �t�@�C���� : " + pram);
	}

	// �e�N�X�`���ݒ�� Define ���X�V�� Pipeline �ɓK�p�����邽��
	if (isDefineToShader)
	{
		DefineSettingToShader();

		CreatePipeline();
	}
}

const Unordered_Map<String, TexturePramInfo>& Material::GetTextures() const noexcept
{
	return m_textures;
}

void Material::SetDiffuse(const Math::Vector3& diffuse) noexcept
{
	m_materialData.m_diffuse = diffuse;

	UpdateConstantBufferData();
}

const Math::Vector3& Material::GetDiffuse() const noexcept
{
	return m_materialData.m_diffuse;
}

void Material::SetSpecular(const Math::Vector3& specular) noexcept
{
	m_materialData.m_specular = specular;

	UpdateConstantBufferData();
}

const Math::Vector3 Material::GetSpecular() const noexcept
{
	return m_materialData.m_specular;
}

void Material::SetSpecularPower(float specularPower) noexcept
{
	m_materialData.m_specularPower = specularPower;

	UpdateConstantBufferData();
}

float Material::GetSpecularPower() noexcept
{
	return m_materialData.m_specularPower;
}

void Material::SetAmbient(const Math::Vector3& ambient) noexcept
{
	m_materialData.m_ambient = ambient;

	UpdateConstantBufferData();
}

const Math::Vector3& Material::GetAmbient() const noexcept
{
	return m_materialData.m_ambient;
}

void Material::SetEmissive(const Math::Vector3& emissive) noexcept
{
	m_materialData.m_emissive = emissive;

	UpdateConstantBufferData();
}

const Math::Vector3 Material::GetEmissive() const noexcept
{
	return m_materialData.m_emissive;
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

void Material::SetBlendMode(BLEND_MODE mode) noexcept
{
	m_materialData.m_blendMode = mode;

	CreatePipeline();
}

BLEND_MODE Material::GetBlendMode() const noexcept
{
	return m_materialData.m_blendMode;
}

void Material::DefineSettingToShader() noexcept
{
	Vector<D3D_SHADER_MACRO> defines;

	// �ϐ��̈ꎞ�I�ȕۑ��ꏊ�Ƃ��Ďg�p
	Map<String, String> macros;

	// �e�N�X�`���p�̃}�N����`���쐬
	for (auto& texture : m_textures)
	{
		auto& info = texture.second;
	
		auto name = "HAS_SRV_" + std::to_string(info.m_bindPoint);
		macros.emplace(std::move(name), info.m_texture ? "1" : "0");
	}
	
	constexpr auto NumNullObject = 1;
	defines.reserve(macros.size() + NumNullObject);

	for (auto& macro : macros)
	{
		defines.push_back(D3D_SHADER_MACRO(macro.first.c_str(), macro.second.c_str()));
	}

	// �I�[��`
	defines.push_back(D3D_SHADER_MACRO(NULL, NULL));

	auto& shaderPath = m_materialData.m_shaderPaths[PixelShader];
	m_shader.SetShader(PixelShader, shaderPath, defines.data());
}

void Material::ParametricAnalysis(bool isClear /* = true */) noexcept
{
	if (isClear)
	{
		m_textures.clear();
	}

	if (auto psShader = m_shader.GetShader(PixelShader))
	{
		auto&& srvPram = psShader->GetSRVBindDesc();
		for (int i = 0; i < srvPram.size(); ++i)
		{
			auto& info = m_textures[srvPram[i].Name];

			info.m_bindPoint = srvPram[i].BindPoint;
		}
	}
}

bool Material::CreateSampler() noexcept
{
	m_samplers.resize(2);
	m_samplers[0].Create();
	m_samplers[1].Create(D3D12_FILTER_ANISOTROPIC, D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	return true;
}

bool Material::CreateRootSinature() noexcept
{
	// �p�����[�^�ݒ�
	Array<CD3DX12_DESCRIPTOR_RANGE1, 3> descTblRanges = {};
	Array<CD3DX12_ROOT_PARAMETER1, 3> rootParams = {};

	//===�萔�o�b�t�@�p�ݒ�=====================================
	descTblRanges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[0].InitAsDescriptorTable(1, &descTblRanges[0]);
	//==========================================================

	//===�e�N�X�`���p�ݒ�=======================================
	descTblRanges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 16, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[1].InitAsDescriptorTable(1, &descTblRanges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	//===Sampler�p�ݒ�==========================================
	descTblRanges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 8, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
	rootParams[2].InitAsDescriptorTable(1, &descTblRanges[2], D3D12_SHADER_VISIBILITY_PIXEL);
	//==========================================================

	return m_rootSignature.Create(rootParams.size(), rootParams.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

bool Material::CreatePipeline() noexcept
{
	GraphicsPipelineStateDesc desc;
	desc.VS = m_shader.GetShader(VertexShader);
	desc.PS = m_shader.GetShader(PixelShader);
	desc.GS = m_shader.GetShader(GeometryShader);
	desc.BlendMode = m_materialData.m_blendMode;
	desc.RasterizerState = m_materialData.m_rasterizerState;
	desc.PrimitiveType = PRIMITIVE_TYPE_TRIANGLELIST;
	desc.NumRenderTargets = 1;

	return m_pipeline.Create(desc, &m_rootSignature);
}

bool Material::CreateConstantBuffer() noexcept
{
	if (m_constantBufferMaterial.Create(sizeof(ConstantBufferMaterial)))
	{
		UpdateConstantBufferData();
		return true;
	}

	return false;
}

void Material::CompileShader() noexcept
{
	for (int i = 0; i < ShaderType::NumAllType; ++i)
	{
		if (i == PixelShader)
		{
			// Define ���l�������R���p�C��
			DefineSettingToShader();
		}
		else
		{
			auto& shaderPath = m_materialData.m_shaderPaths[i];
			SetShader(static_cast<ShaderType>(i), shaderPath, false);
		}
	}
}

void Material::UpdateConstantBufferData() noexcept
{
	auto buffer = static_cast<ConstantBufferMaterial*>(m_constantBufferMaterial.GetCPUData());

	buffer->diffuse = m_materialData.m_diffuse;
	buffer->specular = m_materialData.m_specular;
	buffer->specularPower = m_materialData.m_specularPower;
	buffer->ambient = m_materialData.m_ambient;
}

void Material::UpdateProprietaryDataFile() noexcept
{
	// ����Ăяo�����K�p����p�����[�^�[�p�X���擾
	m_materialData.m_shaderPaths = m_shader.GetShaderPaths();

	// ���f�[�^�̏���
	m_materialData.m_texturePaths.clear();

	for (const auto& textureInfo : m_textures)
	{
		if (const auto texture = textureInfo.second.m_texture)
		{
			const auto& texturePath = texture->GetFilePath();
			m_materialData.m_texturePaths[textureInfo.first] = texturePath;
		}
	}

	m_materialData.SaveToFile(m_filePath);
}

void Material::UpdateResourceDataFile() noexcept
{
	auto resourceData = GetResourceData();
	auto& refResourcePaths = resourceData->m_refResourcePaths;

	// ���f�[�^�̏���
	refResourcePaths.clear();
	refResourcePaths.shrink_to_fit();

	for (const auto& textureInfo : m_textures)
	{
		if (const auto texture = textureInfo.second.m_texture)
		{
			const auto& texturePath = texture->GetFilePath();
			refResourcePaths.emplace_back(ResourcePath(Texture::TypeData.Hash, texturePath));
		}
	}

	m_resourceManager->UpdateResourceData(resourceData);
}