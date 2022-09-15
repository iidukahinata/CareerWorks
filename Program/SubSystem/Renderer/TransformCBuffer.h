/**
* @file	   TransformCBuffer.h
* @brief
*
* @date	   2022/08/02 2022�N�x����
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
		DirectX::XMMATRIX world;//���[���h�s��
	};

	struct ConstantBufferMatrix
	{
		DirectX::XMMATRIX view;	//�r���[�s��
		DirectX::XMMATRIX proj;	//�v���W�F�N�V�����s��
		DirectX::XMFLOAT3 eye;	//���_���W
	};

	D3D12ConstantBuffer m_constantBufferWorld;
	D3D12ConstantBuffer m_constantBufferMatrix;
};