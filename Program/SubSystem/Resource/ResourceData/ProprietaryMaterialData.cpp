/**
* @file    ProprietaryMaterialData.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include <assimp/scene.h>
#include "ProprietaryMaterialData.h"
#include "../ResourceImporter/ModelImporter/PMDHelper.h"

constexpr auto BASIC_SHADER_PATH = "Data/Resource/Shader/BasicShader.hlsl";

ProprietaryMaterialData::ProprietaryMaterialData()
{
	m_shaderPaths[VertexShader] = BASIC_SHADER_PATH;
	m_shaderPaths[PixelShader]  = BASIC_SHADER_PATH;

	m_albedo		  = Math::Vector3::Zero;
	m_metallic		  = 0.0f;
	m_smooth		  = 0.0f;
	m_emission		  = Math::Vector3::Zero;
	m_isInstancing	  = false;
	m_blendMode		  = BLEND_MODE::BLEND_MODE_NO_ALPHA;
	m_rasterizerState = RASTERIZER_STATE::NO_CULL;
}

void ProprietaryMaterialData::Serialized(FileStream* file) const
{
	file->Write(m_albedo);
	file->Write(m_metallic);
	file->Write(m_smooth);
	file->Write(m_emission);
	file->Write(m_blendMode);
	file->Write(m_rasterizerState);
	file->Write(m_isInstancing);

	for (int i = 0; i < ShaderType::NumAllType; ++i)
	{
		file->Write(m_shaderPaths[i]);
	}

	file->Write(m_textureInfos.size());
	for (auto& texture : m_textureInfos)
	{
		file->Write(texture.first);
		file->Write(texture.second.texturePath);
		file->Write(texture.second.bindPoint);
	}
}

void ProprietaryMaterialData::Deserialized(FileStream* file)
{
	file->Read(&m_albedo);
	file->Read(&m_metallic);
	file->Read(&m_smooth);
	file->Read(&m_emission);
	file->Read(&m_blendMode);
	file->Read(&m_rasterizerState);
	file->Read(&m_isInstancing);

	for (int i = 0; i < ShaderType::NumAllType; ++i)
	{
		file->Read(&m_shaderPaths[i]);
	}

	size_t size = 0;
	file->Read(&size);

	for (int i = 0; i < size; ++i)
	{
		String pram;
		file->Read(&pram);

		String path;
		file->Read(&path);

		uint32_t bindPoint;
		file->Read(&bindPoint);

		m_textureInfos[pram].texturePath = path;
		m_textureInfos[pram].bindPoint = bindPoint;
	}
}

ProprietaryMaterialData ProprietaryMaterialData::ConvertProprietaryData(aiMaterial* material) noexcept
{
	ProprietaryMaterialData materialData = {};

	materialData.m_shaderPaths[VertexShader] = BASIC_SHADER_PATH;
	materialData.m_shaderPaths[PixelShader]  = BASIC_SHADER_PATH;

	aiColor3D colorAlbedo(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, colorAlbedo);
	materialData.m_albedo.x = colorAlbedo.r;
	materialData.m_albedo.y = colorAlbedo.g;
	materialData.m_albedo.z = colorAlbedo.b;

	aiColor3D colorspecular(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, colorspecular);
	materialData.m_metallic = colorspecular.r;

	float power = 0.0f;
	material->Get(AI_MATKEY_SHININESS_STRENGTH, power);
	materialData.m_smooth = power;

	aiColor3D coloremissive(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, coloremissive);
	materialData.m_emission.x = coloremissive.r;
	materialData.m_emission.y = coloremissive.g;
	materialData.m_emission.z = coloremissive.b;

	return materialData;
}

ProprietaryMaterialData ProprietaryMaterialData::ConvertProprietaryData(PMDMaterial* material) noexcept
{
	ProprietaryMaterialData materialData = {};

	materialData.m_shaderPaths[VertexShader] = BASIC_SHADER_PATH;
	materialData.m_shaderPaths[PixelShader]  = BASIC_SHADER_PATH;

	materialData.m_albedo.x = material->m_diffuse.x;
	materialData.m_albedo.y = material->m_diffuse.y;
	materialData.m_albedo.z = material->m_diffuse.z;

	materialData.m_metallic = material->m_specular.x;
	materialData.m_smooth = material->m_specularity;

	materialData.m_emission.x = material->m_ambient.x;
	materialData.m_emission.y = material->m_ambient.y;
	materialData.m_emission.z = material->m_ambient.z;
	
	return materialData;
}

String ProprietaryMaterialData::ConvertProprietaryPath(StringView filePath) noexcept
{
	// ファイル拡張子を独自ファイル用に変更するため消去
	return (MATERIAL_DIRECTORY + FileSystem::GetFilePath(filePath.substr(0, filePath.find("."))) + MATERIAL_EXTENSION);
}