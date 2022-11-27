/**
* @file    ProprietaryShaderData.cpp
* @brief
*
* @date	   2022/09/02 2022îNìxèâî≈
*/


#include "ProprietaryShaderData.h"

void ProprietaryShaderData::Serialized(FileStream* file) const
{

}

void ProprietaryShaderData::Deserialized(FileStream* file)
{

}

String ProprietaryShaderData::ConvertProprietaryPath(StringView filePath) noexcept
{
	return (SHADER_DIRECTORY + FileSystem::GetFilePath(filePath));
}