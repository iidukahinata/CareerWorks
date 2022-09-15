/**
* @file    ShaderImporter.cpp
* @brief
*
* @date	   2022/08/07 2022”N“x‰”Å
*/


#include "ShaderImporter.h"
#include "SubSystem/Resource/ResourceManager.h"

ShaderImporter::ShaderImporter(ResourceManager* resourceManager) :
	m_resourceManager(resourceManager)
{
}

bool ShaderImporter::CreateShaderData(StringView filePath) noexcept
{
	auto shaderPath = ConvertProprietaryPath(filePath);
	FileSystem::Copy(filePath, shaderPath);
	m_resourceManager->CreateResourceData("Shader", filePath);
	return true;
}

String ShaderImporter::ConvertProprietaryPath(StringView filePath) noexcept
{
	return SHADER_DIRECTORY + FileSystem::GetFilePath(filePath);
}