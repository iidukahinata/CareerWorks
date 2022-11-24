/**
* @file    D3D12Shader.cpp
* @brief
*
* @date	   2022/09/02 2022年度初版
*/


#include "D3D12Shader.h"

class ShaderCache
{
	COPY_PROHIBITED(ShaderCache)
	ShaderCache() = default;
public:

	static ShaderCache& Get()
	{
		static ShaderCache instance;
		return instance;
	}

	HRESULT Compile(StringView filePath, ID3DBlob** blob) noexcept;
	HRESULT Compile(StringView filePath, StringView entryPoint, StringView traget, ID3DBlob** blob, ID3DBlob** error, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept;
	HRESULT ReCompile(StringView filePath, StringView entryPoint, StringView traget, ID3DBlob** blob, ID3DBlob** error, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept;

private:

	void CreateListKey(StringView filePath, StringView entryPoint, StringView traget, String& key) noexcept;
	void CreateDefineKey(D3D_SHADER_MACRO* defines, String& key) noexcept;

	HRESULT CompileInternal(StringView filePath, StringView entryPoint, StringView traget, ID3DBlob** blob, ID3DBlob** error, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept;

private:

	using BlobMap = Unordered_Map<String, Microsoft::WRL::ComPtr<ID3DBlob>>;
	Unordered_Map<String, BlobMap> m_blobMapList;
};

HRESULT ShaderCache::Compile(StringView filePath, ID3DBlob** blob) noexcept
{
	String listKey, defKey;
	listKey = filePath;

	HRESULT hr = S_OK;
	if (m_blobMapList.contains(listKey) && m_blobMapList[defKey].contains(defKey))
	{
		*blob = m_blobMapList[listKey][defKey].Get();
	}
	else
	{
		hr = D3DReadFileToBlob(ToWstring(filePath).data(), blob);
		if (hr == S_OK)
		{
			// register shader data
			m_blobMapList[listKey][defKey] = *blob;
		}
	}

	return hr;
}

HRESULT ShaderCache::Compile(
	StringView filePath,
	StringView entryPoint,
	StringView traget,
	ID3DBlob** blob,
	ID3DBlob** error,
	D3D_SHADER_MACRO* defines /* = nullptr */
) noexcept
{
	String listKey, defKey;
	CreateListKey(filePath, entryPoint, traget, listKey);
	CreateDefineKey(defines, defKey);

	HRESULT hr = S_OK;

	if (m_blobMapList.contains(listKey) && m_blobMapList[listKey].contains(defKey))
	{
		*blob = m_blobMapList[listKey][defKey].Get();
	}
	else
	{
		hr = CompileInternal(filePath, entryPoint, traget, blob, error, defines);

		if (hr == S_OK)
		{
			// register shader data
			m_blobMapList[listKey][defKey] = *blob;
		}
	}

	return hr;
}

HRESULT ShaderCache::ReCompile(
	StringView filePath,
	StringView entryPoint,
	StringView traget,
	ID3DBlob** blob,
	ID3DBlob** error,
	D3D_SHADER_MACRO* defines
) noexcept
{
	String listKey, defKey;
	CreateListKey(filePath, entryPoint, traget, listKey);
	CreateDefineKey(defines, defKey);

	auto hr = CompileInternal(filePath, entryPoint, traget, blob, error, defines);
	if (FAILED(hr)) {
		return hr;
	}

	// register shader data
	m_blobMapList[listKey][defKey] = *blob;

	return hr;
}

void ShaderCache::CreateListKey(StringView filePath, StringView entryPoint, StringView traget, String& key) noexcept
{
	key = String(filePath) + entryPoint.data() + traget.data();
}

void ShaderCache::CreateDefineKey(D3D_SHADER_MACRO* defines, String& key) noexcept
{
	if (!defines) {
		return;
	}

	for (int i = 0;; ++i)
	{
		auto name = defines[i].Name;
		auto def = defines[i].Definition;

		if (name == NULL || def == NULL)
		{
			break;
		}

		key += name;
		key += def;
	}
}

HRESULT ShaderCache::CompileInternal(
	StringView filePath,
	StringView entryPoint,
	StringView traget,
	ID3DBlob** blob,
	ID3DBlob** error,
	D3D_SHADER_MACRO* defines
) noexcept
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif // DEBUG

	return D3DCompileFromFile(
		ToWstring(filePath).data(),
		defines,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint.data(), traget.data(),
		compileFlags, 0,
		blob, error);
}

D3D12Shader::D3D12Shader(
	StringView filePath,
	StringView entryPoint,
	StringView traget,
	D3D_SHADER_MACRO* defines /* = nullptr */
)
{
	Compile(filePath, entryPoint, traget, defines);
}

bool D3D12Shader::Compile(StringView filePath, ShaderType type, D3D_SHADER_MACRO* defines /* = nullptr */) noexcept
{
	return Compile(filePath, GetEntryPointFromShaderType(type), GetTargetFromShaderType(type), defines);
}

bool D3D12Shader::Compile(
	StringView filePath,
	StringView entryPoint,
	StringView traget,
	D3D_SHADER_MACRO* defines /* = nullptr */
) noexcept
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif // DEBUG

	if (GetExt(filePath) == "cso")
	{
		auto hr = ShaderCache::Get().Compile(filePath, &m_blob);
		if (FAILED(hr))
		{
			LOG_ERROR("シェーダコンパイルに失敗。: Shader.cpp");
			return false;
		}
	}
	else
	{
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		auto hr = ShaderCache::Get().Compile(
			filePath, entryPoint, traget,
			&m_blob, errorBlob.GetAddressOf(), defines);

		if (FAILED(hr))
		{
			ErrorHandle(hr, errorBlob.Get());
			return false;
		}
	}

	return true;
}

bool D3D12Shader::ReCompile(StringView filePath, ShaderType type, D3D_SHADER_MACRO* defines) noexcept
{
	return ReCompile(filePath, GetEntryPointFromShaderType(type), GetTargetFromShaderType(type), defines);
}

bool D3D12Shader::ReCompile(StringView filePath, StringView entryPoint, StringView traget, D3D_SHADER_MACRO* defines) noexcept
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif // DEBUG

	if (GetExt(filePath) != "cso")
	{
		Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
		auto hr = ShaderCache::Get().ReCompile(
			filePath, entryPoint, traget,
			&m_blob, errorBlob.GetAddressOf(), defines);

		if (FAILED(hr))
		{
			ErrorHandle(hr, errorBlob.Get());
			return false;
		}
	}

	return true;
}

Vector<D3D12_INPUT_ELEMENT_DESC> D3D12Shader::GetInputLayout() noexcept
{
	D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), IID_PPV_ARGS(m_reflection.ReleaseAndGetAddressOf()));

	D3D12_SHADER_DESC shaderDesc;
	m_reflection->GetDesc(&shaderDesc);

	Vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs;
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		m_reflection->GetInputParameterDesc(i, &paramDesc);

		D3D12_INPUT_ELEMENT_DESC inputElementDesc;
		ZeroMemory(&inputElementDesc, sizeof(inputElementDesc));
		inputElementDesc.SemanticName = paramDesc.SemanticName;
		inputElementDesc.SemanticIndex = paramDesc.SemanticIndex;
		inputElementDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				inputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}

		inputElementDescs.push_back(std::move(inputElementDesc));
	}

	return inputElementDescs;
}

Vector<D3D12_SHADER_INPUT_BIND_DESC> D3D12Shader::GetSRVBindDesc() noexcept
{
	D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), IID_PPV_ARGS(m_reflection.ReleaseAndGetAddressOf()));

	D3D12_SHADER_DESC shaderDesc;
	m_reflection->GetDesc(&shaderDesc);
	
	Vector<D3D12_SHADER_INPUT_BIND_DESC> srvBindDescs;
	for (int i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D12_SHADER_INPUT_BIND_DESC desc;
		m_reflection->GetResourceBindingDesc(i, &desc);

		if (desc.Type == D3D_SIT_TEXTURE)
		{
			srvBindDescs.emplace_back(desc);
		}
	}

	return srvBindDescs;
}

Vector<D3D12_SHADER_BUFFER_DESC> D3D12Shader::GetCBVBufferDesc() noexcept
{
	D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), IID_PPV_ARGS(m_reflection.ReleaseAndGetAddressOf()));

	D3D12_SHADER_DESC shaderDesc;
	m_reflection->GetDesc(&shaderDesc);

	Vector<D3D12_SHADER_BUFFER_DESC> cbvBufferDescs;
	for (int i = 0; i <= shaderDesc.ConstantBuffers; ++i)
	{
		auto cb = m_reflection->GetConstantBufferByIndex(i);

		D3D12_SHADER_BUFFER_DESC desc;
		cb->GetDesc(&desc);

		cbvBufferDescs.emplace_back(desc);
	}
	return cbvBufferDescs;
}

Vector<D3D12_SHADER_INPUT_BIND_DESC> D3D12Shader::GetSamplerBindDesc() noexcept
{
	D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), IID_PPV_ARGS(m_reflection.ReleaseAndGetAddressOf()));

	D3D12_SHADER_DESC shaderDesc;
	m_reflection->GetDesc(&shaderDesc);

	Vector<D3D12_SHADER_INPUT_BIND_DESC> samplerBindDescs;
	for (int i = 0; i < shaderDesc.BoundResources; ++i)
	{
		D3D12_SHADER_INPUT_BIND_DESC desc;
		m_reflection->GetResourceBindingDesc(i, &desc);

		if (desc.Type == D3D_SIT_SAMPLER)
		{
			samplerBindDescs.emplace_back(desc);
		}
	}

	return samplerBindDescs;
}

Vector<DXGI_FORMAT> D3D12Shader::GetRenderTargetsDesc() noexcept
{
	D3DReflect(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), IID_PPV_ARGS(m_reflection.ReleaseAndGetAddressOf()));

	D3D12_SHADER_DESC shaderDesc;
	m_reflection->GetDesc(&shaderDesc);

	Vector<DXGI_FORMAT> m_renderTargetFormats;
	for (int i = 0; i < shaderDesc.OutputParameters; ++i)
	{
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		m_reflection->GetOutputParameterDesc(i, &paramDesc);

		DXGI_FORMAT format;
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		m_renderTargetFormats.emplace_back(format);
	}
	return m_renderTargetFormats;
}

String D3D12Shader::GetEntryPointFromShaderType(ShaderType type)
{
	switch (type)
	{
	case VertexShader  : return "vs_main";
	case PixelShader   : return "ps_main";
	case GeometryShader: return "gs_main";
	case HullShader	   : return "hs_main";
	case DomainShader  : return "ds_main";
	case ComputeShader : return "cs_main";
	default			   : return String();
	}
}

String D3D12Shader::GetTargetFromShaderType(ShaderType type)
{
	switch (type)
	{
	case VertexShader  : return "vs_5_0";
	case PixelShader   : return "ps_5_0";
	case GeometryShader: return "gs_5_0";
	case HullShader	   : return "hs_5_0";
	case DomainShader  : return "ds_5_0";
	case ComputeShader : return "cs_5_0";
	default			   : return String();
	}
}

void D3D12Shader::ErrorHandle(HRESULT hr, ID3DBlob* errorBlob) const noexcept
{
	if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
	{
		LOG_ERROR("指定されたファイルが見つかりませんでした。: Shader.cpp");
	}

	if (errorBlob)
	{
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), error.begin());
		error += "\n";
		LOG_ERROR(error.data());
	}
}