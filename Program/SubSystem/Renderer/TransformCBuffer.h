/**
* @file	   TransformCBuffer.h
* @brief
*
* @date	   2022/08/02 2022îNìxèâî≈
*/
#pragma once


#include "GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Camera;

class TransformCBuffer
{
	COMPLETED_DEVELOPMENT()
public:

	void Initialize() noexcept;
	void Update(Camera* mainCamera) noexcept;

	void Bind() noexcept;

	void SetWorld(const DirectX::XMMATRIX& world) noexcept;
	void SetProjection(const DirectX::XMMATRIX& proj) noexcept;
	void SetView(const DirectX::XMMATRIX& view) noexcept;

	void CreateMatrixBufferData() noexcept;

private:

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

	D3D12ConstantBuffer m_constantBufferMatrix;
};