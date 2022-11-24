/**
* @file    D3D12Shader.h
* @brief
*
* @date	   2022/09/02 2022年度初版
*/
#pragma once


#include "D3D12DeviceChild.h"

class D3D12Shader : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	D3D12Shader() = default;
	D3D12Shader(StringView filePath, StringView entryPoint, StringView traget, D3D_SHADER_MACRO* defines = nullptr);

	bool Compile(StringView filePath, ShaderType type, D3D_SHADER_MACRO* defines = nullptr) noexcept;
	bool Compile(StringView filePath, StringView entryPoint, StringView traget, D3D_SHADER_MACRO* defines = nullptr) noexcept;

	bool ReCompile(StringView filePath, ShaderType type, D3D_SHADER_MACRO* defines = nullptr) noexcept;
	bool ReCompile(StringView filePath, StringView entryPoint, StringView traget, D3D_SHADER_MACRO* defines = nullptr) noexcept;

	/** アクセス */
	ID3DBlob* GetBlob() const noexcept { return m_blob; }

	Vector<D3D12_INPUT_ELEMENT_DESC>	 GetInputLayout()		noexcept;
	Vector<D3D12_SHADER_INPUT_BIND_DESC> GetSRVBindDesc()		noexcept;
	Vector<D3D12_SHADER_BUFFER_DESC>	 GetCBVBufferDesc()	    noexcept;
	Vector<D3D12_SHADER_INPUT_BIND_DESC> GetSamplerBindDesc()	noexcept;
	Vector<DXGI_FORMAT>					 GetRenderTargetsDesc() noexcept;

private:

	String GetEntryPointFromShaderType(ShaderType type);
	String GetTargetFromShaderType(ShaderType type);

	void ErrorHandle(HRESULT hr, ID3DBlob* errorBlob) const noexcept;

private:

	ID3DBlob* m_blob;
	Microsoft::WRL::ComPtr<ID3D12ShaderReflection> m_reflection;
};