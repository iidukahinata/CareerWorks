/**
* @file	   SkyBox.h
* @brief
*
* @date	   2022/10/28 2022îNìxèâî≈
*/
#pragma once


#include "GraphicsAPI/D3D12/D3D12Shader.h"
#include "GraphicsAPI/D3D12/D3D12Sampler.h"
#include "GraphicsAPI/D3D12/D3D12IndexBuffer.h"
#include "GraphicsAPI/D3D12/D3D12VertexBuffer.h"
#include "GraphicsAPI/D3D12/D3D12PipelineState.h"
#include "GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Camera;
class Texture;

class SkyBox
{
public:

	void Initialize() noexcept;

	void Render(Camera* mainCamera) noexcept;

	void SetTexture(Texture* texture) noexcept;
	Texture* GetTexture() const noexcept;

private:

	bool CreateRootSinature() noexcept;
	bool CreatePipeline() noexcept;

private:

	Texture* m_texture = nullptr;

	// * pipeline objects
	D3D12Sampler			   m_sampler;
	D3D12RootSignature		   m_rootSignature;
	D3D12GraphicsPipelineState m_pipeline;

	D3D12Shader m_vertexShader;
	D3D12Shader m_pixelShader;

	// * mesh objects
	D3D12IndexBuffer  m_indexBuffer;
	D3D12VertexBuffer m_vertexBuffer;

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	// * çsóÒï€éù
	D3D12ConstantBuffer m_constantBufferMatrix;
};