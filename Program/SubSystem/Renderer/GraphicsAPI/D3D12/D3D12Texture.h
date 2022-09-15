/**
* @file    D3D12Texture.h
* @brief
*
* @date	   2022/08/30 2022�N�x����
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

	/** Texture1D �p */
	bool Create(DXGI_FORMAT format, UINT width) noexcept;

	/** Texture2D �p */
	bool Create(DXGI_FORMAT format, UINT width, UINT height) noexcept;

	/** Texture3D �p */
	bool Create(DXGI_FORMAT format, UINT width, UINT height, UINT depth) noexcept;

	/**
	* @param images [����] �C���[�W�z��̐擪�|�C���^���w�肵�Ă��������B
	* @param imageSize [����] �C���[�W�z��T�C�Y���w�肵�Ă��������B
	* @param meta [����] �ǂݍ��܂ꂽ�e�N�X�`�� meta �f�[�^���w�肵�Ă��������B
	*/
	bool Create(const DirectX::Image* images, size_t imageSize, const DirectX::TexMetadata& meta) noexcept;

	void VSSet(UINT slot = 0) noexcept;
	void PSSet(UINT slot = 0) noexcept;
	void GSSet(UINT slot = 0) noexcept;

	/** �A�N�Z�X */
	D3D12ShaderResourceView& GetView() noexcept { return m_view; }

private:

	D3D12ShaderResourceView m_view;
};