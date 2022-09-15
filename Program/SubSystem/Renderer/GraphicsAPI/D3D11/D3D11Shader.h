/**
* @file    D3D11Shader.h
* @brief
*
* @date	   2022/07/06 2022年度初版
* @note
*						 D3D11Shader
*						   	 ↓
*		   -----------------------------------------
*	      ↓	                 ↓					   ↓
*	D3D11VertexShader	D3D11PixelShader	D3D11ComputeShader
*/
#pragma once


#include "D3D11DeviceChild.h"

/**	各シェーダーオブジェクト抽象クラス */
class D3D11Shader : public D3D11DeviceChild
{
public:

	/**
	* 指定シェーダーのコンパイルを行う
	* @param shader [入力] コンパイルを行うシェーダーファイル名
	* @param entrypoint [入力] 指定シェーダーの開始位置
	* @param traget [入力] シェーダー種類を指定
	*/
	bool CompileFromFile(StringView shader, D3D_SHADER_MACRO* macro, StringView entrypoint, StringView traget) noexcept;

	/** アクセス */
	ID3DBlob* GetBlob() const noexcept;

protected:

	// * コンパイルしたシェーダーデータを保持させる
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
};

class D3D11VertexShader : public D3D11Shader
{
public:

	/**
	* @param shader [入力] 何かしらの文字列である必要があります。
	* @param entrypoint [入力] 何かしらの文字列である必要があります。
	* @param traget [入力] 何かしらの文字列である必要があります。
	*/
	bool Create(StringView shader, D3D_SHADER_MACRO* macro = nullptr, StringView entrypoint = "main", StringView target = "vs_5_0") noexcept;

	/** デバイス設定するための関数です。*/
	void VSSet() const noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
};

class D3D11PixelShader : public D3D11Shader
{
public:

	/**
	* @param shader [入力] 何かしらの文字列である必要があります。
	* @param entrypoint [入力] 何かしらの文字列である必要があります。
	* @param traget [入力] 何かしらの文字列である必要があります。
	*/
	bool Create(StringView shader, D3D_SHADER_MACRO* macro = nullptr, StringView entrypoint = "main", StringView target = "ps_5_0") noexcept;

	/** デバイス設定するための関数です。*/
	void PSSet() const noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

class D3D11ComputeShader : public D3D11Shader
{
public:

	/**
	* @param shader [入力] 何かしらの文字列である必要があります。
	* @param entrypoint [入力] 何かしらの文字列である必要があります。
	* @param traget [入力] 何かしらの文字列である必要があります。
	*/
	bool Create(StringView shader, D3D_SHADER_MACRO* macro = nullptr, StringView entrypoint = "main", StringView target = "cs_5_0") noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;
};

class D3D11GeometryShader : public D3D11Shader
{
public:

	/**
	* @param shader [入力] 何かしらの文字列である必要があります。
	* @param entrypoint [入力] 何かしらの文字列である必要があります。
	* @param traget [入力] 何かしらの文字列である必要があります。
	*/
	bool Create(StringView shader, D3D_SHADER_MACRO* macro = nullptr, StringView entrypoint = "main", StringView target = "gs_5_0") noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
};