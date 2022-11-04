/**
* @file	   TransformCBuffer.cpp
* @brief
*
* @date	   2022/11/02 2022”N“x‰”Å
*/


#include "TransformCBuffer.h"
#include "SubSystem/Scene/Component/Components/Camera.h"

void TransformCBuffer::Update(Camera* mainCamera) noexcept
{
	ASSERT(mainCamera);

	Update(mainCamera->GetViewMatrix().ToMatrixXM(), mainCamera->GetProjectionMatrix().ToMatrixXM());
}

void TransformCBuffer::Update(DirectX::XMMATRIX view, DirectX::XMMATRIX projection) noexcept
{
	m_view		 = view;
	m_projection = projection;

	m_viewProjection		= m_view * m_projection;
	m_viewProjectionInverse = DirectX::XMMatrixInverse(nullptr, m_viewProjection);
}

void TransformCBuffer::Bind(void* matrixBuffer, const DirectX::XMMATRIX& world) noexcept
{
	auto buffer					  = static_cast<ConstantBufferMatrix*>(matrixBuffer);
	buffer->world				  = world;
	buffer->worldView			  = world * m_view;
	buffer->worldViewProjection	  = world * m_viewProjection;
	buffer->view				  = m_view;
	buffer->viewProjection		  = m_viewProjection;
	buffer->viewProjectionInverse = m_viewProjectionInverse;
	buffer->projection			  = m_projection;
}