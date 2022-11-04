/**
* @file	   TransformCBuffer.h
* @brief
*
* @date	   2022/11/02 2022îNìxèâî≈
*/
#pragma once


#include "GraphicsAPI/D3D12/D3D12ConstantBuffer.h"

class Camera;

class TransformCBuffer
{
	COMPLETED_DEVELOPMENT()
public:

	void Update(Camera* mainCamera) noexcept;
	void Update(DirectX::XMMATRIX view, DirectX::XMMATRIX projection) noexcept;

	void Bind(void* matrixBuffer, const DirectX::XMMATRIX& world) noexcept;

private:

	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_viewProjection;
	DirectX::XMMATRIX m_viewProjectionInverse;
	DirectX::XMMATRIX m_projection;

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
};