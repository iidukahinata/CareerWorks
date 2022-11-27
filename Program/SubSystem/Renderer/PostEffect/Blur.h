/**
* @file    Blur.h
* @brief
*
* @date	   2022/11/10 2022年度初版
*/
#pragma once


#include "IPostEffect.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Blur : public IPostEffect
{
	IN_DEVELOPMENT()
	SUB_CLASS(Blur)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;
	void Initialize(PostProcessEffect* postProcessEffect, bool useluma = false) noexcept;

	void Render() override;

	void SetVariance(float variance) noexcept;
	float GetVariance() const noexcept;

	D3D12RenderTexture& GetTexture() override;

private:

	bool CreateRenderTextures(int width, int height) noexcept;
	bool CreateRenderingObjects(int width, int height) noexcept;
	bool CreatePipelineStates(bool useluna = false) noexcept;

	/** 重み付けテーブルの作成をして Buffer の更新を行う。*/
	void CalculateWeightTablesFromGaussian() noexcept;

private:

	float m_variance;

	// * render objects
	D3D12RenderTexture m_xBlurRenderTexture;
	D3D12RenderTexture m_yBlurRenderTexture;

	// * rendering objects
	D3D12Sampler m_sampler;
	D3D12RootSignature m_rootSignature;

	// * sprite mesh buffer
	D3D12IndexBuffer m_xBlurIndexBuffer;
	D3D12VertexBuffer m_xBlurVertexBuffer;
	D3D12IndexBuffer m_yBlurIndexBuffer;
	D3D12VertexBuffer m_yBlurVertexBuffer;

	// * pipeline objects
	D3D12GraphicsPipelineState m_xBlurPipeline;
	D3D12GraphicsPipelineState m_yBlurPipeline;

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX worldView;
		DirectX::XMMATRIX worldViewProjection;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX viewProjection;
		DirectX::XMMATRIX viewProjectionInverse;
		DirectX::XMMATRIX projection;
	};

	struct ConstantBufferBlur
	{
		float weights[8];
		Math::Vector2 screenSize;
	};

	D3D12ConstantBuffer m_xBlurMatrixConstantBuffer;
	D3D12ConstantBuffer m_yBlurMatrixConstantBuffer;
	D3D12ConstantBuffer m_blurConstantBuffer;
};