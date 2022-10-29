/**
* @file    RenderObject.cpp
* @brief
*
* @date	   2022/10/25 2022”N“x‰”Å
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"

void RenderObject::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();

	m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix));
}

void RenderObject::OnRegister()
{
	IComponent::OnRegister();

	RegisterToRenderer();
}

void RenderObject::OnUnRegister()
{
	IComponent::OnUnRegister();

	UnRegisterFromRenderer();
}

void RenderObject::RegisterToRenderer() noexcept
{
	ASSERT(m_renderer);
	m_renderer->AddRenderObject(this);
}

void RenderObject::UnRegisterFromRenderer() noexcept
{
	ASSERT(m_renderer);
	m_renderer->RemoveRenderObject(this);
}