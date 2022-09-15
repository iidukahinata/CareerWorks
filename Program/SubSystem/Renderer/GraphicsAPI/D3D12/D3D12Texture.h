/**
* @file    D3D12Texture.h
* @brief
*
* @date	   2022/08/30 2022年度初版
*/
#pragma once


#include "D3D12View.h"
#include "D3D12Buffer.h"

class D3D12Texture : public D3D12Buffer
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(const D3D12_RESOURCE_DESC* desc) noexcept;
	bool Create(const DirectX::TexMetadata& meta) noexcept;

	/** Texture1D 用 */
	bool Create(DXGI_FORMAT format, UINT width) noexcept;

	/** Texture2D 用 */
	bool Create(DXGI_FORMAT format, UINT width, UINT height) noexcept;

	/** Texture3D 用 */
	bool Create(DXGI_FORMAT format, UINT width, UINT height, UINT depth) noexcept;

	/**
	* @param images [入力] イメージ配列の先頭ポインタを指定してください。
	* @param imageSize [入力] イメージ配列サイズを指定してください。
	* @param meta [入力] 読み込まれたテクスチャ meta データを指定してください。
	*/
	bool Create(const DirectX::Image* images, size_t imageSize, const DirectX::TexMetadata& meta) noexcept;

	void VSSet(UINT slot = 0) noexcept;
	void PSSet(UINT slot = 0) noexcept;
	void GSSet(UINT slot = 0) noexcept;

	/** アクセス */
	D3D12ShaderResourceView& GetView() noexcept { return m_view; }

private:

	D3D12ShaderResourceView m_view;
};