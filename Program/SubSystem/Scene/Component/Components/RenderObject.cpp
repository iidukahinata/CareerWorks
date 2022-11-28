/**
* @file    RenderObject.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/IRenderer.h"
#include "SubSystem/Thread/RenderingThread/RenderingThread.h"

void RenderObject::RegisterToRenderer(IRenderObject* renderObject) noexcept
{
	if (m_isRegister)
	{
		return;
	}

	m_isRegister = true;

	if (IsInRenderingThread())
	{
		m_renderer->AddRenderObject(renderObject);
	}
	else
	{
		RegisterRenderCommand([this, renderObject] { m_renderer->AddRenderObject(renderObject); });
	}
}

void RenderObject::UnRegisterFromRenderer(IRenderObject* renderObject) noexcept
{
	if (!m_isRegister)
	{
		return;
	}

	m_isRegister = false;

	if (IsInRenderingThread())
	{
		m_renderer->RemoveRenderObject(renderObject);
	}
	else
	{
		RegisterRenderCommand([this, renderObject] { m_renderer->RemoveRenderObject(renderObject); });
	}
}