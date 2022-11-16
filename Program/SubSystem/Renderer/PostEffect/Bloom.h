/**
* @file    Bloom.h
* @brief
*
* @date	   2022/11/10 2022îNìxèâî≈
*/
#pragma once


#include "PostEffect.h"
#include "Blur.h"

class Bloom : public PostEffect
{
	IN_DEVELOPMENT()
	SUB_CLASS(Bloom)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;

	void Render() override;

	D3D12RenderTexture& GetTexture() override;

public:

	void SetIntensity(float intensity) noexcept;
	float GetIntensity() const noexcept;

	void SetThreshold(float threshold) noexcept;
	float GetThreshold() const noexcept;

	void SetClamp(float clamp) noexcept;
	float GetClamp() const noexcept;

	void SetDiffusion(float variance) noexcept;
	float GetDiffusion() const noexcept;

	void SetColor(const Math::Vector4& color) noexcept;
	const Math::Vector4& GetColor() const noexcept;

private:

	void LuminousPass() noexcept;
	void DualBlurPass() noexcept;
	void BloomPass() noexcept;

	bool CreateRenderTextures(int width, int height) noexcept;
	bool CreateRenderingObjects(int width, int height) noexcept;
	bool CreatePipelineStates() noexcept;

	void UpdateConstantBufferLuma() noexcept;
	void UpdateConstantBufferBloom() noexcept;

private:

	float m_intensity = 1.0f;
	float m_threshold = 1.0f;
	float m_clamp = 65472.0f;
	float m_diffusion = 10.0f;
	Math::Vector4 m_color = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	Array<Blur, 4> m_blurs;

	// * render objects
	D3D12RenderTexture m_renderTexture;
	D3D12RenderTexture m_luminousRenderTexture;

	// * rendering objects
	D3D12RootSignature m_rootSignature;

	// * sprite mesh buffer
	D3D12IndexBuffer m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	// * pipeline objects
	D3D12GraphicsPipelineState m_bloomPipeline;
	D3D12GraphicsPipelineState m_luminousPipeline;

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

	struct ConstantBufferLuma
	{
		float intensity;
		float threshold;
		float clamp;
	};

	struct ConstantBufferBloom
	{
		Math::Vector4 color;
	};

	D3D12ConstantBuffer m_matrixConstantBuffer;
	D3D12ConstantBuffer m_lumaConstantBuffer;
	D3D12ConstantBuffer m_bloomConstantBuffer;
};