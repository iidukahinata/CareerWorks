/**
* @file    RenderObject.cpp
* @brief
*
* @date	   2022/10/03 2022”N“x‰”Å
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"

void RenderObject::OnInitialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();
}

void RenderObject::OnStart()
{
	RegisterToRenderer();
}

void RenderObject::OnStop()
{
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