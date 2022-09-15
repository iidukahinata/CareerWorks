/**
* @file    Shader.cpp
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/


#include "Shader.h"

void Shader::ReCompile(ShaderType type, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept
{
	SetShader(type, m_shaderPaths[type], defines);
}

void Shader::SetShader(ShaderType type, StringView name, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept
{
	m_shaderPaths[type] = name;
	m_shaders[type].Compile(name, type, defines);
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