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

void ProprietaryMaterialData::Serialized(FileStream* file) const
{
	file->Write(m_albedo);
	file->Write(m_metallic);
	file->Write(m_smooth);
	file->Write(m_emission);
	file->Write(m_blendMode);
	file->Write(m_rasterizerState);

	for (int i = 0; i < ShaderType::NumAllType; ++i)
	{
		file->Write(m_shaderPaths[i]);
	}

	file->Write(m_texturePaths.size());
	for (auto& texture : m_texturePaths)
	{
		file->Write(texture.first);
		file->Write(texture.second);
	}
}

void ProprietaryMaterialData::Deserialization(FileStream* file)
{
	file->Read(&m_albedo);
	file->Read(&m_metallic);
	file->Read(&m_smooth);
	file->Read(&m_emission);
	file->Read(&m_blendMode);
	file->Read(&m_rasterizerState);

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

		m_texturePaths[pram] = path;
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

	float power;
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
	filePath = filePath.substr(0, filePath.find("."));

	return (MATERIAL_DIRECTORY + FileSystem::GetFilePath(filePath) + MATERIAL_EXTENSION);
}