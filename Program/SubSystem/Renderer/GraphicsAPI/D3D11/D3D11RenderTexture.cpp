/**
* @file    D3D11RenderTexture.cpp
* @brief   
*
* @date	   2022/06/25 2022年度初版
*/


#include "D3D11RenderTexture.h"

bool D3D11RenderTexture::Create(
	UINT widthSize,
	UINT heightSize,
	DXGI_FORMAT colorFormat /* = DXGI_FORMAT_R8G8B8A8_UNORM */,
	DXGI_FORMAT depthFormat /* = DXGI_FORMAT_D32_FLOAT */
) noexcept
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> renderTargetViewTexture;
	D3D11_TEXTURE2D_DESC renderTargetTextureDesc;
	ZeroMemory(&renderTargetTextureDesc, sizeof(renderTargetTextureDesc));
	renderTargetTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTargetTextureDesc.Format = colorFormat;
	renderTargetTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	renderTargetTextureDesc.Width = widthSize;
	renderTargetTextureDesc.Height = heightSize;
	renderTargetTextureDesc.CPUAccessFlags = 0;
	renderTargetTextureDesc.MipLevels = 1;
	renderTargetTextureDesc.ArraySize = 1;
	renderTargetTextureDesc.SampleDesc.Count = 1;
	renderTargetTextureDesc.SampleDesc.Quality = 0;

	HRESULT hr = GetDevice()->CreateTexture2D(&renderTargetTextureDesc, nullptr, renderTargetViewTexture.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("RenderTarget 用テクスチャの生成に失敗しました。");
		return false;
	}

	// レンダーターゲットビューの設定
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = renderTargetTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// レンダーターゲットビューの生成
	hr = GetDevice()->CreateRenderTargetView(renderTargetViewTexture.Get(), &renderTargetViewDesc, m_renderTarget.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("RenderTargetView の生成に失敗しました。");
		return false;
	}

	// シェーダリソースビューの設定
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = renderTargetTextureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// シェーダリソースビューの生成
	hr = GetDevice()->CreateShaderResourceView(renderTargetViewTexture.Get(), &srvDesc, m_texture.GetAddressOfShaderResourceView());
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView の生成に失敗しました。");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilTexture;
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
	ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
	depthStencilTextureDesc.Width = widthSize;
	depthStencilTextureDesc.Height = heightSize;
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.ArraySize = 1;
	depthStencilTextureDesc.Format = depthFormat;
	depthStencilTextureDesc.SampleDesc.Count = 1;
	depthStencilTextureDesc.SampleDesc.Quality = 0;
	depthStencilTextureDesc.SampleDesc = renderTargetTextureDesc.SampleDesc;
	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.CPUAccessFlags = 0;
	depthStencilTextureDesc.MiscFlags = 0;

	hr = GetDevice()->CreateTexture2D(&depthStencilTextureDesc, nullptr, depthStencilTexture.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("深度バッファ用テクスチャの生成に失敗しました。");
		return false;
	}

	// 深度バッファの設定
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Format = depthStencilTextureDesc.Format;
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// 深度バッファの生成
	hr = GetDevice()->CreateDepthStencilView(depthStencilTexture.Get(), &depthStencilDesc, m_depthStencil.GetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("深度バッファの生成に失敗しました。");
		return false;
	}

	// ビューポートの設定
	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = static_cast<float>(widthSize);
	m_viewport.Height = static_cast<float>(heightSize);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

	return true;
}

void D3D11RenderTexture::SetRenderTarget() noexcept
{
	GetContext()->OMSetRenderTargets(1, m_renderTarget.GetAddressOf(), m_depthStencil.Get());
	GetContext()->RSSetViewports(1, &m_viewport);
}

void D3D11RenderTexture::Clear(Math::Vector4 color) noexcept
{
	float clearColor[4] = { color.x, color.y, color.z, color.w };

	GetContext()->ClearRenderTargetView(m_renderTarget.Get(), clearColor);
	GetContext()->ClearDepthStencilView(m_depthStencil.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}