/**
* @file	   RenderStates.h
* @brief
*
* @date	   2022/07/06 2022年度初版
*/
#pragma once


#include <d3d11.h>
#include <wrl/client.h>

class D3D11RenderStates
{
	D3D11RenderStates() = default;
	COPY_PROHIBITED(D3D11RenderStates);
public:

	static D3D11RenderStates& Get() noexcept
	{
		static D3D11RenderStates instance;
		return instance;
	}

	bool Init();

	void SetBlendState(ID3D11BlendState* blendState);
	void SetRasterizerState(ID3D11RasterizerState* rasterizer);

public:

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_noCull;					// カリングなし
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cullClockWise;			// カリングあり
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_wireframeCull;			// ワイヤーフレーム
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_depthTiltBias;			// 深度傾斜バイアス

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_noAlphaBlend;				// アルファブレンドなし
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_alphaEnabledBlend;			// アルファブレンドあり
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_additiveBlend;				// 加算
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_additiveTransparencyBlend;	// 加算(透過)
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_subtractionBlend;			// 減算
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_multiplicationBlend;			// 乗算
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_screenCompositingBlend;		// スクリーン
};