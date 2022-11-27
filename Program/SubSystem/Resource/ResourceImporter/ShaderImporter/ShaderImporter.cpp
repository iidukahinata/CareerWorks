/**
* @file    ShaderImporter.cpp
* @brief
*
* @date	   2022/08/07 2022”N“x‰”Å
*/


#include "ShaderImporter.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Resource/Resources/3DModel/Shader.h"
#include "SubSystem/Resource/ResourceData/ProprietaryShaderData.h"

ShaderImporter::ShaderImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{
}

bool ShaderImporter::CreateShaderData(StringView filePath) noexcept
{
	auto shaderPath = ProprietaryShaderData::ConvertProprietaryPath(filePath);
	FileSystem::Copy(filePath, shaderPath);

	if (GetExt(filePath) == "hlsli")
	{
		return true;
	}

	return m_resourceManager->CreateResourceData<Shader>(shaderPath);
}