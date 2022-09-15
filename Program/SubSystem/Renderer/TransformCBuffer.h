/**
* @file	   TransformCBuffer.h
* @brief
*
* @date	   2022/08/02 2022年度初版
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

	void SetWorld(const DirectX::XMMATRIX& world) noexcept;
	void SetProjection(const DirectX::XMMATRIX& proj) noexcept;
	void SetView(const DirectX::XMMATRIX& view) noexcept;
	void SetEye(const DirectX::XMFLOAT3& eye) noexcept;

private:

	struct ConstantBufferWorld
	{
		DirectX::XMMATRIX world;//ワールド行列
	};

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX view;	//ビュー行列
		DirectX::XMMATRIX proj;	//プロジェクション行列
		DirectX::XMFLOAT3 eye;	//視点座標
	};

	D3D12ConstantBuffer m_constantBufferWorld;
	D3D12ConstantBuffer m_constantBufferMatrix;
};