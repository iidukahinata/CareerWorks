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
class Material;

class SkyBox
{
public:

	void Initialize() noexcept;

	void Render(Camera* mainCamera) noexcept;

	void SetMaterial(Material* material) noexcept;
	Material* GetMaterial() const noexcept;

private:

	Material* m_material = nullptr;

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