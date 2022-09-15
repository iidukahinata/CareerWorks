/**
* @file	   D3D11GrahicsDevice.h
* @brief
*
* @date	   2022/07/06 2022年度初版
*/
#pragma once


#include <d3d11.h>
#include <wrl/client.h>

class D3D11GraphicsDevice
{
	D3D11GraphicsDevice() = default;
	COPY_PROHIBITED(D3D11GraphicsDevice);
public:

	static D3D11GraphicsDevice& Get() noexcept
	{
		static D3D11GraphicsDevice instance;
		return instance;
	}

	bool Init(HWND hWnd, UINT screenWidth, UINT screenheight, bool isFullscreen);

	/** 画面クリア */
	void Clear(const Math::Vector4& color);

	void Present();

	ID3D11Device* GetDevice() noexcept;
	ID3D11DeviceContext* GetContext() noexcept;
	IDXGISwapChain* GetSwapChain() noexcept;

	// RenderTarget
	void SetRenderTarget(ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStencil);
	void SetRenderTargets(UINT numViews, ID3D11RenderTargetView** renderTarget, ID3D11DepthStencilView* depthStencil);
	ID3D11RenderTargetView* GetRenderTarget() noexcept;

	// viewport
	void SetViewport(float width, float height);
	void SetViewports(UINT numBffers, D3D11_VIEWPORT* viewports);

	// DepthStencil
	ID3D11DepthStencilView* GetDepthStencil() noexcept;

private:

	// Direct3D
	Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;

	// view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
};