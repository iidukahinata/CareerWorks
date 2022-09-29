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
	CLASS_DATA(Shader)
public:

	void ReCompile(ShaderType type, D3D_SHADER_MACRO* defines = nullptr) noexcept;

	bool SetShader(ShaderType type, StringView path, D3D_SHADER_MACRO* defines = nullptr) noexcept;
	D3D12Shader* GetShader(ShaderType type) noexcept;

	const Array<String, ShaderType::NumAllType>& GetShaderPaths() const noexcept { return m_shaderPaths; }

private:

	Map<UINT, D3D12Shader> m_shaders;

	Array<String, ShaderType::NumAllType> m_shaderPaths;
};