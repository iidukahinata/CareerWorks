/**
* @file    Shader.cpp
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/


#include "Shader.h"

void Shader::ReCompile(ShaderType type, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept
{
	SetShader(type, m_shaderPaths[type], defines);
}

bool Shader::SetShader(ShaderType type, StringView path, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept
{
	if (path.empty())
	{
		return false;
	}

	m_shaderPaths[type] = path;
	return m_shaders[type].Compile(path, type, defines);
}

D3D12Shader* Shader::GetShader(ShaderType type) noexcept
{
	if (m_shaders.contains(type) && !m_shaderPaths[type].empty())
	{
		return &m_shaders[type];
	}
	else
	{
		return nullptr;
	}
}

bool Shader::HasShader() const noexcept
{
	return !m_shaders.empty();
}