/**
* @file    D3D12RenderTexture.h
* @brief
*
* @date	   2022/08/30 2022�N�x����
*/
#pragma once


#include "D3D12View.h"

class D3D12RenderTexture : public D3D12DeviceChild
{
	COMPLETED_DEVELOPMENT()
public:

	bool Create(
		int width, int height,
		DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT depthFormat = DXGI_FORMAT_D32_FLOAT) noexcept;

	/** SetRenderTarget ����O�Ɋm�F���邽�� */
	void WaitUntilToAvailable();

	/** �f�o�C�X�ݒ肷�邽�߂̊֐��ł��B*/
	void SetRenderTarget() noexcept;
	void PSSet(UINT slot) noexcept;

	/** �S�Ă̗v�f���w��J���[�ɐݒ� */
	void Clear(const Math::Vector4& color) noexcept;

	/** �A�N�Z�X */
	D3D12RenderTargetView* GetRenderTargetView() noexcept;
	D3D12DepthStencilView* GetDepthStencilView() noexcept;
	D3D12ShaderResourceView* GetShaderResourceView() noexcept;

private:

	bool CreateRenderTarget(int width, int height, DXGI_FORMAT colorFormat) noexcept;
	bool CreateDepthStencil(int width, int height, DXGI_FORMAT depthFormat) noexcept;

private:

	Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTarget;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencil;
	
	D3D12ShaderResourceView m_shaderResourceView;
	D3D12RenderTargetView m_renderTargetView;
	D3D12DepthStencilView m_depthStencilView;

	D3D12_VIEWPORT m_viewport = {};
};