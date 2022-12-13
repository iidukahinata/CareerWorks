/**
* @file    FXAA.h
* @brief
*
* @date	   2022/12/12 2022îNìxèâî≈
*/
#pragma once


#include "IPostEffect.h"
#include "../GraphicsAPI/D3D12/D3D12Sampler.h"
#include "../GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12VertexBuffer.h"
#include "../GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "../GraphicsAPI/D3D12/D3D12RenderTexture.h"
#include "../GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class FXAA : public IPostEffect
{
	IN_DEVELOPMENT()
	SUB_CLASS(FXAA, IPostEffect)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;

	void Render() override;

	D3D12RenderTexture& GetTexture() override;

private:

	bool CreateRenderTextures(int width, int height) noexcept;
	bool CreateRenderingObjects(int width, int height) noexcept;
	bool CreatePipelineStates() noexcept;

private:

	// * render objects
	D3D12RenderTexture m_renderTexture;
	
	// * rendering objects
	D3D12RootSignature m_rootSignature;

	// * sprite mesh buffer
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	// * pipeline objects
	D3D12GraphicsPipelineState m_fxaaPipeline;

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

	struct ConstantBufferFXAA
	{
		 Math::Vector4 rcpFrame;
	};

	D3D12ConstantBuffer m_matrixConstantBuffer;
	D3D12ConstantBuffer m_fxaaConstantBuffer;
};