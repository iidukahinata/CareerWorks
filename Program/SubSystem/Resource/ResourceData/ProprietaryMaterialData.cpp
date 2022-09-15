/**
* @file    ProprietaryMaterialData.cpp
* @brief
*
* @date	   2022/09/06 2022年度初版
*/


#include <assimp/scene.h>
#include "ProprietaryMaterialData.h"
#include "../ResourceImporter/ModelImporter/PMDHelper.h"

constexpr auto BASIC_SHADER = "BasicShader.hlsl";

void ProprietaryMaterialData::Serialized(FileStream* file) const
{
	file->Write(m_diffuse);
	file->Write(m_specular);
	file->Write(m_specularPower);
	file->Write(m_ambient);
	file->Write(m_emissive);
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
	file->Read(&m_diffuse);
	file->Read(&m_specular);
	file->Read(&m_specularPower);
	file->Read(&m_ambient);
	file->Read(&m_emissive);
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

	materialData.m_shaderPaths[VertexShader] = BASIC_SHADER;
	materialData.m_shaderPaths[PixelShader] = BASIC_SHADER;

	aiColor3D colordiffuse(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_DIFFUSE, colordiffuse);
	materialData.m_diffuse.x = colordiffuse.r;
	materialData.m_diffuse.y = colordiffuse.g;
	materialData.m_diffuse.z = colordiffuse.b;

	aiColor3D colorspecular(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_SPECULAR, colorspecular);
	materialData.m_specular.x = colorspecular.r;
	materialData.m_specular.y = colorspecular.g;
	materialData.m_specular.z = colorspecular.b;

	aiColor3D colorambient(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_AMBIENT, colorambient);
	materialData.m_ambient.x = colorambient.r;
	materialData.m_ambient.y = colorambient.g;
	materialData.m_ambient.z = colorambient.b;

	aiColor3D coloremissive(0.f, 0.f, 0.f);
	material->Get(AI_MATKEY_COLOR_EMISSIVE, coloremissive);
	materialData.m_emissive.x = coloremissive.r;
	materialData.m_emissive.y = coloremissive.g;
	materialData.m_emissive.z = coloremissive.b;

	float power;
	material->Get(AI_MATKEY_SHININESS_STRENGTH, power);
	materialData.m_specularPower = power;

	return materialData;
}

ProprietaryMaterialData ProprietaryMaterialData::ConvertProprietaryData(PMDMaterial* material) noexcept
{
	ProprietaryMaterialData materialData = {};

	materialData.m_shaderPaths[VertexShader] = BASIC_SHADER;
	materialData.m_shaderPaths[PixelShader] = BASIC_SHADER;

	materialData.m_diffuse.x = material->m_diffuse.x;
	materialData.m_diffuse.y = material->m_diffuse.y;
	materialData.m_diffuse.z = material->m_diffuse.z;

	materialData.m_specular.x = material->m_specular.x;
	materialData.m_specular.y = material->m_specular.x;
	materialData.m_specular.z = material->m_specular.x;

	materialData.m_ambient.x = material->m_ambient.x;
	materialData.m_ambient.y = material->m_ambient.y;
	materialData.m_ambient.z = material->m_ambient.z;

	materialData.m_specularPower = material->m_specularity;
	
	return materialData;
}

String ProprietaryMaterialData::ConvertProprietaryPath(StringView filePath) noexcept
{
	// ファイル拡張子を独自ファイル用に変更するため消去
	filePath = filePath.substr(0, filePath.find("."));

	return (MATERIAL_DIRECTORY + FileSystem::GetFilePath(filePath) + MATERIAL_EXTENSION);
}