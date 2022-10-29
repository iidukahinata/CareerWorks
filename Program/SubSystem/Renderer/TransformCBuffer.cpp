/**
* @file	   TransformCBuffer.cpp
* @brief
*
* @date	   2022/10/28 2022”N“x‰”Å
*/


#include "TransformCBuffer.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

void TransformCBuffer::Update(Camera* mainCamera) noexcept
{
	m_view		 = mainCamera->GetViewMatrix().ToMatrixXM();
	m_projection = mainCamera->GetProjectionMatrix().ToMatrixXM();

	m_viewProjection		= m_view * m_projection;
	m_viewProjectionInverse = DirectX::XMMatrixInverse(nullptr, m_viewProjection);
}

void TransformCBuffer::Bind(void* matrixBuffer, const DirectX::XMMATRIX& world) noexcept
{
	m_world = world;

	auto buffer					  = static_cast<ConstantBufferMatrix*>(matrixBuffer);
	buffer->world				  = m_world;
	buffer->worldView			  = m_world * m_view;
	buffer->worldViewProjection	  = m_world * m_viewProjection;
	buffer->view				  = m_view;
	buffer->viewProjection		  = m_viewProjection;
	buffer->viewProjectionInverse = m_viewProjectionInverse;
	buffer->projection			  = m_projection;
}