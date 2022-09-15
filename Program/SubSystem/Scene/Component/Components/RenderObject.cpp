/**
* @file    RenderObject.cpp
* @brief
*
* @date	   2022/08/02 2022”N“x‰”Å
*/


#include "RenderObject.h"
#include "SubSystem/Renderer/Renderer.h"

void RenderObject::Initialize()
{
	m_renderer = GetContext()->GetSubsystem<Renderer>();

	RegisterToRenderer();

	Do_Initialize();
}

void RenderObject::Remove()
{
	UnRegisterFromRenderer();

	Do_Remove();
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

void RenderObject::RegisterToRenderer()
{
	ASSERT(m_renderer);
	m_renderer->AddRenderObject(this);
}

void RenderObject::UnRegisterFromRenderer()
{
	ASSERT(m_renderer);
	m_renderer->RemoveRenderObject(this);
}