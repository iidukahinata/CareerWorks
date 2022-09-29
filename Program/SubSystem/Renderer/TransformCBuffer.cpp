/**
* @file	   TransformCBuffer.cpp
* @brief
*
* @date	   2022/09/18 2022”N“x‰”Å
*/


#include "TransformCBuffer.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

void TransformCBuffer::Initialize() noexcept
{
	m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix));
}

void TransformCBuffer::Update(Camera* mainCamera) noexcept
{
	SetProjection(mainCamera->GetProjectionMatrix().ToMatrixXM());
	SetView(mainCamera->GetViewMatrix().ToMatrixXM());

	CreateMatrixBufferData();
}

void TransformCBuffer::Bind() noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());

	buffer->worldView = buffer->world * buffer->view;
	buffer->worldViewProjection = buffer->world * buffer->viewProjection;

	m_constantBufferMatrix.VSSet(0);
}

void TransformCBuffer::SetWorld(const DirectX::XMMATRIX& world) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->world = world;
}

void TransformCBuffer::SetProjection(const DirectX::XMMATRIX& proj) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->projection = proj;
}

void TransformCBuffer::SetView(const DirectX::XMMATRIX& view) noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());
	buffer->view = view;
}

void TransformCBuffer::CreateMatrixBufferData() noexcept
{
	auto buffer = static_cast<ConstantBufferMatrix*>(m_constantBufferMatrix.GetCPUData());

	buffer->viewProjection = buffer->view * buffer->projection;
	buffer->viewProjectionInverse = DirectX::XMMatrixInverse(nullptr, buffer->viewProjection);
}