/**
* @file    RenderCommandFance.cpp
* @brief
*
* @date	   2022/11/01 2022îNìxèâî≈
*/


#include "RenderCommandFance.h"
#include "RenderingThread.h"

void RenderCommandFance::BegineFrame() noexcept
{
	++m_fanceValue;

	RegisterRenderCommand([this] {

		--m_fanceValue;
		m_single.store(true);
		m_single.notify_one();

	});
}

void RenderCommandFance::WaitForSingle() noexcept
{
	while (!IsSingle())
	{
		m_single.wait(false);
		m_single = false;
	}
}

bool RenderCommandFance::IsSingle() const noexcept
{
	return m_fanceValue.load() == 0;
}