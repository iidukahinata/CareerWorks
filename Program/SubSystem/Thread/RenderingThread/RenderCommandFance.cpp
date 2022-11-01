/**
* @file    RenderCommandFance.cpp
* @brief
*
* @date	   2022/11/01 2022îNìxèâî≈
*/


#include "RenderCommandFance.h"
#include "RenderingThread.h"

RenderCommandFance::RenderCommandFance()
{
	m_hEvent = CreateEvent(nullptr, true, false, nullptr);
	ASSERT(m_hEvent);
}

RenderCommandFance::~RenderCommandFance()
{
	CloseHandle(m_hEvent);
}

void RenderCommandFance::BegineFrame() noexcept
{
	++m_fanceValue;

	RegisterRenderCommand([this] {
		SetEvent(m_hEvent);
		--m_fanceValue;
	});
}

void RenderCommandFance::WaitForSingle() const noexcept
{
	while (!IsSingle())
	{
		if (WaitForSingleObject(m_hEvent, INFINITE) == WAIT_OBJECT_0)
		{
			ResetEvent(m_hEvent);
		}
	}
}

bool RenderCommandFance::IsSingle() const noexcept
{
	return m_fanceValue.load() == 0;
}