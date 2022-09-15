/**
* @file    Shader.h
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/
#pragma once


#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12Shader.h"

class Shader
{
public:

	void ReCompile(ShaderType type, D3D_SHADER_MACRO* defines = nullptr) noexcept;

	void SetShader(ShaderType type, StringView name, D3D_SHADER_MACRO* defines = nullptr) noexcept;
	D3D12Shader* GetShader(ShaderType type) noexcept;

	Array<String, ShaderType::NumAllType> GetShaderPaths() const noexcept { return m_shaderPaths; }

private:

	Map<UINT, D3D12Shader> m_shaders;

	Array<String, ShaderType::NumAllType> m_shaderPaths;
};