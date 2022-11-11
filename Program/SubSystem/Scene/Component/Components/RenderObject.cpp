/**
* @file    RenderObject.cpp
* @brief
*
* @date	   2022/11/04 2022”N“x‰”Å
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void RenderObject::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();
	ASSERT(m_renderer);

	m_constantBufferMatrix.Create(sizeof(ConstantBufferMatrix));
}

void RenderObject::OnRegister()
{
	IComponent::OnRegister();

	if (GetActive())
	{
		RegisterToRenderer();
	}
}

void RenderObject::OnUnRegister()
{
	IComponent::OnUnRegister();

	if (GetActive())
	{
		UnRegisterFromRenderer();
	}
}

void RenderObject::OnRemove()
{
	if (m_isRegister)
	{
		UnRegisterFromRenderer();

		m_renderCommandFance.BegineFrame();
	}

	IComponent::OnRemove();
}

void RenderObject::SetActive(bool active)
{
	if (GetActive() == active)
	{
		return;
	}

	IComponent::SetActive(active);

	if (active)
	{
		RegisterToRenderer();
	}
	else
	{
		UnRegisterFromRenderer();
	}
}

bool RenderObject::Erasable()
{
	return m_renderCommandFance.IsSingle();
}

void RenderObject::RegisterToRenderer() noexcept
{
	if (m_isRegister)
	{
		return;
	}

	m_isRegister = true;

	if (IsInRenderingThread())
	{
		m_renderer->AddRenderObject(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->AddRenderObject(this); });
	}
}

void RenderObject::UnRegisterFromRenderer() noexcept
{
	if (!m_isRegister)
	{
		return;
	}

	m_isRegister = false;

	if (IsInRenderingThread())
	{
		m_renderer->RemoveRenderObject(this);
	}
	else
	{
		RegisterRenderCommand([this] { m_renderer->RemoveRenderObject(this); });
	}
}