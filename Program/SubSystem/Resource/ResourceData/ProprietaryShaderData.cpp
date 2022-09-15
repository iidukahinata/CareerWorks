/**
* @file    ProprietaryShaderData.cpp
* @brief
*
* @date	   2022/09/02 2022�N�x����
*/


#include "ProprietaryShaderData.h"

String ProprietaryShaderData::ConvertProprietaryPath(StringView filePath) noexcept
{
	return (SHADER_DIRECTORY + FileSystem::GetFilePath(filePath));
}