/**
* @file	   TransformCBuffer.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "TransformCBuffer.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

void TransformCBuffer::Initialize() noexcept
{
	m_constantBufferWorld.Create(sizeof(ConstantBufferWorld));
	m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix));
}

void TransformCBuffer::Update(Camera* mainCamera) noexcept
{
	SetProjection(mainCamera->GetProjectionMatrix().ToMatrixXM());
	SetView(mainCamera->GetViewMatrix().ToMatrixXM());
	SetEye(mainCamera->GetTransform().GetWoldPosition());

	m_constantBufferMatrix.VSSet(0);
}

void TransformCBuffer::SetWorld(const DirectX::XMMATRIX& world) noexcept
{
	auto buffer = static_cast<ConstantBufferWorld*>(m_constantBufferWorld.GetCPUData());
	buffer->world = world;

	m_constantBufferWorld.VSSet(1);
}

void TransformCBuffer::SetProjection(const DirectX::XMMATRIX& proj) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->proj = proj;
}

void TransformCBuffer::SetView(const DirectX::XMMATRIX& view) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->view = view;
}

void TransformCBuffer::SetEye(const DirectX::XMFLOAT3& eye) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->eye = eye;
}